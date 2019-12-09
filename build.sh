#!/bin/bash

#Currently there is only Support for ATmega32/ATmega32L
#so please do not change the 'ATMEGA32' Macro.

echo "Compiling MDX..."
echo -ne '#####                     (33%)\r'

g++ -D ATMEGA32 -I ./Header/ -g -o mdx ./Source/*.cpp

echo -ne '#############             (66%)\r'

sleep 0.2

echo -ne '#######################   (100%)\r'
echo -ne '\n'
echo "Done!"

exit 0