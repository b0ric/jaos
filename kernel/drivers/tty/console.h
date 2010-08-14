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

#define NR_CONS (NR_TTYS)

#define TAB_LEN 8               // only power of 2
#define TAB_MASK (TAB_LEN-1)    // to determine actual TAB length

typedef struct {
  uint32_t addr;                // starting display address for console
  uint16_t mem_size;            // size of memory available for console
  uint16_t cursor;              // cursor position
  uint8_t line;                 // current output line (next char will go here)
  uint8_t col;                  // current output column (next char will go here)
  uint8_t attr;                 // current output attribute
  uint8_t prev_char;            // previous character sent to console
} console_t;

extern console_t cons[];

void cons_init (console_t *cons);
void cons_write (struct tty_t *term);

#endif /* _CONSOLE_H */

