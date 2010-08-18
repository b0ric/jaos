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

#ifndef _KEYBD_H
#define _KEYBD_H

#include <stdint.h>
#include <unistd.h>

#define KEYMAP_COLS 5

/* mask to check whether it is break or make code */
#define BREAK 0x80

/* modifiers field macros */
#define Alt(key)  (0x80  | (key))       // Alternate (prints extended ASCII symbols)
#define A(key)    (ALT   | (key))       // ALT flag (for special keys)
#define S(key)    (SHIFT | (key))       // SHIFT flag
#define C(key)    (CTRL  | (key))       // CTRL flag
#define AS(key)   (SHIFT | ALT | (key)) // ALT - SHIFT
#define SP(key)   (SPECIAL | (key))     // SPECIAL key

/* escape code to mark escape sequence */
#define ESC     0xE0

/* types and prototypes */
typedef uint8_t scancode_t;

void init_kb (void);
void keybd_handle_irq (void);

extern keycode_t key_map[];

#endif /* _KEYBD_H */

