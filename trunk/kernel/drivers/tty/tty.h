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

#define TTY_INBUF_SZ 32

struct tty_t;

/* returns error or number of read/written symbols, etc */
typedef uint8_t (*tty_open_t)(void);
typedef uint8_t (*tty_read_t)(struct tty_t *term);
typedef uint8_t (*tty_write_t)(struct tty_t *term);
typedef uint8_t (*tty_close_t)(void);

struct tty_t {
  tty_open_t open;
  tty_read_t read;
  tty_write_t write;
  tty_close_t close;
  uint16_t inbuf[TTY_INBUF_SZ];
};

#endif /* _TTY_H */

