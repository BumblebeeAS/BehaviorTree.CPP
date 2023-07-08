#!/bin/bash

find . -type d \( -path ./3rdparty \) -prune -name '*.h' -or -name '*.hpp' -or -name '*.cpp' | xargs clang-format-10 -i -style=file $1
