#!/bin/bash

#Currently there is only Support for ATmega32/ATmega32L
#so please do not change the 'ATMEGA32' Macro.

g++ -D ATMEGA32 -I ./Header/ -g -o mdx ./Source/*.cpp