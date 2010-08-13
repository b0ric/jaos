/*
 * Copyright (c) 2010 Borisov Alexandr
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Borisov Alexandr <b0ric.alex@gmail.com>
 */

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <stdint.h>
#include "tty.h"

#define NR_CONS 1

#define VGA_MEM_ADDR 0xA0000

/* Video mode defenitions */
#define VGA_TEXT_LINES 25
#define VGA_TEXT_COLS 80

/* VGA color attributes (suffix L = Light, D = Dark)*/
#define COL_BLACK 0x00
#define COL_BLUE 0x01
#define COL_GREEN 0x02
#define COL_CYAN 0x03
#define COL_RED 0x04
#define COL_MAGENTA 0x05
#define COL_BROWN 0x06
#define COL_LGRAY 0x07
#define COL_DGRAY 0x08
#define COL_LBLUE 0x09
#define COL_LGREEN 0x0A
#define COL_LCYAN 0x0B
#define COL_LRED 0x0C
#define COL_LMAGENTA 0x0D
#define COL_YELLOW 0x0E
#define COL_WHITE 0x0F

#define FORE(val) (val)
#define BACK(val) ((val) << 4)

#define TAB_LEN 8               // only power of 2
#define TAB_MASK (TAB_LEN-1)    // to determine actual TAB length

typedef struct _console_t {
  uint32_t addr;                // starting display address for console
  uint16_t mem_size;            // size of memory available for console
  uint16_t cursor;              // cursor position
  uint8_t line;                 // current output line (next char will go here)
  uint8_t col;                  // current output column (next char will go here)
  uint8_t attr;                 // current output attribute
  uint8_t prev_char;            // previous character sent to console
  struct tty_t *tty;            // associated tty structure
} console_t;


#endif /* _CONSOLE_H */

