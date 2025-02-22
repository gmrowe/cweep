#!/usr/bin/env bash

set -xe
clang -o mines -Wall -Wextra -Werror -g main.c -Iraylib/src -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL libraylib.a
