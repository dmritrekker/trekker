#!/bin/bash

mkdir -p build
make clean

# Create html pages
make html

sphinx-autobuild source/ build/html/

