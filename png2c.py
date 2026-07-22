from PIL import Image
from pathlib import Path

INPUT_DIR = Path("images")
OUTPUT_DIR = INPUT_DIR / "img"

OUTPUT_DIR.mkdir(exist_ok=True)

for image_path in INPUT_DIR.glob("*.png"):

    name = image_path.stem

    img = Image.open(image_path).convert("RGBA")

    width, height = img.size

    pixels = []

    for r, g, b, a in img.getdata():

        # Premultiplicar RGB por alpha para evitar halos en los bordes
        pr = (r * a) // 255
        pg = (g * a) // 255
        pb = (b * a) // 255

        value = (
            (a << 24) |
            (pr << 16) |
            (pg << 8)  |
            pb
        )

        pixels.append(f"0x{value:08X}")

    h_path = OUTPUT_DIR / f"{name}.h"
    c_path = OUTPUT_DIR / f"{name}.c"

    guard = f"{name.upper()}_H"

    with open(h_path, "w") as f:

        f.write(f"#ifndef {guard}\n")
        f.write(f"#define {guard}\n\n")

        f.write("#include <efi.h>\n")
        f.write("#include <image_metadata.h>\n\n")

        f.write(f"extern UINT32 {name}_pixels[];\n")
        f.write(f"extern WAVE_IMAGE_METADATA {name}_metadata;\n\n")

        f.write("#endif\n")

    with open(c_path, "w") as f:

        f.write(f'#include "{name}.h"\n\n')

        f.write(
            f"UINT32 {name}_pixels[{len(pixels)}] = {{\n"
        )

        for i in range(0, len(pixels), 8):
            f.write("    ")
            f.write(", ".join(pixels[i:i+8]))
            f.write(",\n")

        f.write("};\n\n")

        f.write(
            f"WAVE_IMAGE_METADATA {name}_metadata = {{\n"
        )
        f.write(f"    .Width = {width},\n")
        f.write(f"    .Height = {height},\n")
        f.write(f"    .Pixels = {name}_pixels\n")
        f.write("};\n")

    print(f"Generated {name}")