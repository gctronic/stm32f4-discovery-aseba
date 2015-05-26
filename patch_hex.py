#!/usb/bin/env python

"""
this script patches the hex file to be usable with asebacmd whex

it removes the 04 and 05 type hex entries which are not correctly interpeted by
aseba
"""

import sys
import re

with open(sys.argv[1], 'r') as fin:
    hexfile = fin.read().splitlines(True)

incompatible_line = re.compile('\:\d{6}0[45]')
hexfile = [line for line in hexfile if not incompatible_line.match(line)]

with open(sys.argv[2], 'w') as fout:
    fout.writelines(hexfile)
