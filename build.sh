#! /bin/sh
# build.sh

mkdir -p build

CFLAGS="-Wall -Wimplicit-function-declaration -Werror=implicit-function-declaration \
-fpic -lm -fshort-wchar -mno-red-zone -fno-stack-protector -DEFI_FUNCTION_WRAPPER \
-I/usr/include/efi -I/usr/include/efi/x86_64 -Iinclude"

OBJS=""

for f in $(find src include -name "*.c"); do
    obj="build/$(basename "$f" .c).o"
    gcc $CFLAGS -c "$f" -o "$obj" || exit 1
    OBJS="$OBJS $obj"
done

ld \
-m elf_x86_64 \
-nostdlib \
-z nocombreloc \
-T /usr/lib/elf_x86_64_efi.lds \
-Bsymbolic \
-shared \
-L/usr/lib \
-o build/WaveBoot.so \
/usr/lib/crt0-efi-x86_64.o \
$OBJS \
-lgnuefi \
-lefi

objcopy \
-j .text \
-j .sdata \
-j .data \
-j .dynamic \
-j .rel \
-j .rela \
-j .reloc \
--target=efi-app-x86_64 \
build/WaveBoot.so \
build/WaveBoot.efi

cp build/WaveBoot.efi test/EFI/BOOT/BOOTX64.EFI