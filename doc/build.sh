#!/bin/bash

mkdir -p build
make clean

# Create html pages
make html


# Create trekker manual page
# make man
# mv build/man/trekker.1 build/man/trekker


