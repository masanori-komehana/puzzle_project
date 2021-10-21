#!/bin/bash

source .env/bin/activate

echo build process start.
echo ----------------------------------------

sphinx-autobuild -b html source build/html

echo ----------------------------------------
echo build process end.
