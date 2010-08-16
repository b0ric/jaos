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

/*
 * A few words about how key_map[] works (in keymap.c) ans it's MACROs:
 * 1) we read 8-bit scancode from keyboard
 * 2) if it is break code we set break flag and convert it to make code
 * 3) thus i = value * KEYMAP_COLS + mod_index is in index in key_map[] array where
 *    mod_index - is a 'columns' selector to map the scancode if modifiers
 *    (like SHIFT. ALT, CTRL, NUM, etc) were pressed
 * 4) real symbol is coded in low byte of keycode_t and modifiers are coded in
 *    high byte of keycode_t type (keycode_t is a word size). Here is how 
 *    modifiers are coded:
 *    |ALT|SHIFT|CTRL|SPEC|0|0|0|0|
 *      0    1    2    3   4 5 6 7
 *
 *    ALT    - ALT pressed
 *    SHIFT  - SHIFT pressed
 *    CTRL   - CONTROL pressed
 *    SPEC   - SPECIAL key (F1, F2, ..., KeyPad(KP) keys, etc)
 *    NUM    - NUM LOCK is active
 *    CAPS   - CAPS LOCK is active
 *    SCROLL - SCROLL LOCK is active
 *
 * 5) such modifiers as NUM_LOCK, CAPS_LOCK and SCROLL_LOCK are processed
 *    explicitly in 'keybd.c'
 */

#define BREAK 0x80      // mask to check whether it is break or make code

/* modifier bits */
#define SPECIAL 0x0800

/* modifiers field macros */
#define A(key)    (0x80 | (key))        // ALT (prints extended ASCII symbols)
#define S(key)    (key)
#define C(key)    (key)
#define SP(key)   (SPECIAL | (key))     // SPECIAL key

/* check for modifiers macros */
#define ISSPECIAL(key) (SPECIAL & (key))

/* escape code to mark escape sequence */
#define ESC     0xE0

/* SPECIAL field keycodes (stored in low byte of keycode_t) */
#define CAPS    0x01    // NUM, CAPS, SCROLL LOCK
#define NUM     0x02
#define SCROLL  0x04
#define ESCKEY  0x10    // ESCape key
#define LALT    0x11
#define RALT    0x12
#define LSHIFT  0x13
#define RSHIFT  0x14
#define LCTRL   0x15
#define RCTRL   0x16
#define F1      0x20    // functional keys
#define F2      0x21
#define F3      0x22
#define F4      0x23
#define F5      0x24
#define F6      0x25
#define F7      0x26
#define F8      0x27
#define F9      0x28
#define F10     0x29
#define KP0     0x30    // KeyPad keys
#define KP1     0x31
#define KP2     0x32
#define KP3     0x33
#define KP4     0x34
#define KP5     0x35
#define KP6     0x36
#define KP7     0x37
#define KP8     0x38
#define KP9     0x39
#define KPDOT   0x3A

/* types and prototypes */
typedef uint8_t scancode_t;
typedef uint16_t keycode_t;

void keybd_handle_irq (void);

extern keycode_t key_map[];

#endif /* _KEYBD_H */

