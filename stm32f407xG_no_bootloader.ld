/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * STM32F407xG memory setup.
 */
MEMORY
{
    flash : org = 0x08000000, len = 896k
    aseba_bytecode : org = 0x080e0000, len = 128k
    ram : org = 0x20000000, len = 112k
    ethram : org = 0x2001C000, len = 16k
    ccmram : org = 0x10000000, len = 64k
}

_aseba_bytecode_start = ORIGIN(aseba_bytecode);
_aseba_bytecode_end = ORIGIN(aseba_bytecode) + LENGTH(aseba_bytecode);

INCLUDE linkerscript.ld
