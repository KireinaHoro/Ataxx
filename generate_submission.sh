#!/bin/bash

( cat gameprocess.h gameai.h botzonerunner.h gameprocess.cpp gameai.cpp botzonerunner.cpp | sed -e '/#include "/d' && cat fake_main )> submission.cpp
g++ submission.cpp -o submission -Wall -Werror -Wextra -Wconversion -pthread -O2 -std=c++1z

