#!/bin/bash

mpiexec -n 2 valgrind --leak-check=full --track-origins=yes  \
    pdtest -r 1 -c 2 -x 4 -m 10 -b 5 -s 1 -f ../EXAMPLE/g20.rua
