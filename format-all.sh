#!/bin/sh

find . \( -name '*.cpp' -or -name '*.h' -or -name '*.frag' -or -name '*.vert' \) -exec clang-format --style=file -i {} +;

