#!/bin/bash

source .env/bin/activate

make clean
make html

read -p "Open HTML File? (y/N): " yn
case "$yn" in
  [yY]*) 
      echo open build/html/index.html via Firefox...
      firefox build/html/index.html > /dev/null 2>&1 &
      ;;
  *) ;;
esac

echo build process end.
