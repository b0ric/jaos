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

#include "keybd.h"

#define KBD_IO 0x60		// keyboard IO port (data and command)
#define KBDI_IO 0x64		// keyboard IO interface port (data and command)
#define KBD_BUF_SZ 32           // software buffer fo keyboard scancodes

static scancode_t kb_buffer[KBD_BUF_SZ] = {0};

/* NumLock, ScrollLock and CapsLock status */
static uint8_t locks;

/* modifiers */
static uint8_t lshift = 0, rshift = 0;
static uint8_t lalt = 0, ralt = 0;
static uint8_t lctrl = 0, rctrl = 0;

/* misc */
static uint8_t esc = 0;

void keybd_handle_irq (void)
{
  scancode_t sc;
  keycode_t kc;
  
  sc = inb (KBD_IO);
  if (sc & BREAK) {
        kc = keymap[sc & ~BREAK];
        if (ISSPECIAL(kc))
                kc &= ~SPECIAL;
                /* we won't differ left and right modifiers */
                switch (kc) {
                        case LALT:
                                lalt = 0;
                                break;
                        case RALT:
                                ralt = 0;
                                break;
                        case LSHIFT:
                                lshift = 0;
                                break;
                        case RSHIFT:
                                rshift = 0;
                                break;
                        case LCTRL:
                                lctrl = 0;
                                break;
                        case RCTRL:
                                rctrl = 0;
                                break;
                        case default:
                                break;
                }
  } else if (sc != ESC) {
        kc = keymap[sc];
  } else {
        esc = 1;
  }
}

