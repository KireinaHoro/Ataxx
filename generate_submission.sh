#!/bin/bash
( cat gameprocess.h gameai.h botzonerunner.h gameprocess.cpp gameai.cpp botzonerunner.cpp | sed -e '/#include "/d' && cat fake_main )> a.cpp
g++ a.cpp -o a -Wall -Werror -Wextra -Wconversion

