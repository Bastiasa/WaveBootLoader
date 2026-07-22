#! /bin/sh
python3 png2c.py

mkdir -p include/img/
cp -r images/img/* include/img/