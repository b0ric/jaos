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

#ifndef _TTY_H
#define _TTY_H

#include <stdint.h>

struct tty_t;

#define NR_TTYS 1
#define TTY_BUF_SZ 256

/* returns error or number of read/written symbols, etc */
typedef void (*tty_open_t)(struct tty_t *term);
typedef void (*tty_read_t)(struct tty_t *term);
typedef void (*tty_write_t)(struct tty_t *term);
typedef void (*tty_close_t)(struct tty_t *term);

struct tty_t {
  /* IO members */
  uint8_t idx;                          // terminal index (and associated console)
  tty_open_t open;                      // open tty function
  tty_close_t close;                    // close tty function
  
  /* input members */
  tty_read_t read;                      // read tty function
  uint16_t inbuf[TTY_BUF_SZ];           // input buffer for read
  
  /* output members */
  tty_write_t write;                    // write tty function
  uint8_t outbuf[TTY_BUF_SZ];           // output buffer to write to tty
  uint8_t count;                        // number of chars to write to tty
};

extern struct tty_t tty[];
extern uint8_t active;

#endif /* _TTY_H */

