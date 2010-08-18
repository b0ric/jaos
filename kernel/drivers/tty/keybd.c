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

#include "tty.h"
#include "keybd.h"

#define KBD_IO 0x60		// keyboard IO port (data and command)
#define KBDI_IO 0x64		// keyboard IO interface port (data and command)
/* keyboard status registar flags */
#define OBF 0x01                // output buffer full
#define IBF 0x02                // input buffer full
/* keyboard commands */
#define RESET 0xFF
#define RESEND 0xFE
#define DISABLE 0xF5
#define ENABLE 0xF4
#define ECHO 0xEE
#define SET_LEDS 0xED

#define KBD_BUF_SZ 32           // software buffer fo keyboard scancodes

static scancode_t kb_buffer[KBD_BUF_SZ] = {0};

/* NumLock, ScrollLock and CapsLock status */
static uint8_t locks;

/* modifiers */
static uint8_t shift = 0, lshift = 0, rshift = 0;
static uint8_t alt = 0, lalt = 0, ralt = 0;
static uint8_t ctrl = 0, lctrl = 0, rctrl = 0;

/* misc */
static uint8_t esc = 0;

void set_leds (uint8_t leds);

void init_kb (void)
{
  locks = NUM;
  set_leds (locks);
}

void keybd_handle_irq (void)
{
  struct tty_t *tty_active;
  scancode_t sc;
  keycode_t kc;
  uint8_t modshift;             // modifier keys shift in key_map
  
  /* it's pretty tricky what we've done here but to cut long story short 
     set shift if CAPS is off and drop it if CAPS is on and also take alt 
     into account. Resulting value selects key_map column */
  modshift = (locks & CAPS ^ shift) | (alt << 1);
  
  sc = inb (KBD_IO);
  
  /* process break codes only for CTRL, ALT and SHIFT modifiers */
  if (sc & BREAK) {
        kc = key_map[(sc & ~BREAK) * KEYMAP_COLS + modshift];
        if (ISSPECIAL(kc))
                kc &= ~SPECIAL;
                /* here we have to distinugish left and right modifiers
                   'cause user can push them simultaneously and then release
                   one but other will be still pressed */
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
                        default:
                                break;
                }
                /* we won't differ left and right modifiers */
                shift = lshift | rshift;
                alt = lalt | ralt;
                ctrl = lctrl | rctrl;
  /* process make codes */
  } else if (sc != ESC) {
        kc = key_map[sc * KEYMAP_COLS + modshift];
        /* process special keys */
        if (ISSPECIAL(kc)) {
                kc &= ~SPECIAL;
                switch (kc) {
                    case LALT:
                                lalt = 1;
                                break;
                    case RALT:
                                ralt = 1;
                                break;
                    case LSHIFT:
                                lshift = 1;
                                break;
                    case RSHIFT:
                                rshift = 1;
                                break;
                    case LCTRL:
                                lctrl = 1;
                                break;
                    case RCTRL:
                                rctrl = 1;
                                break;
                    case CAPS:
                                locks ^= CAPS;
                                set_leds (locks);
                                break;
                    case NUM:
                                locks ^= NUM;
                                set_leds (locks);
                                break;
                    case SCROLL:
                                locks ^= SCROLL;
                                set_leds (locks);
                                break;
                    case KPDOT:
                                if (locks & NUM)
                                        kc = '.';
                                else
                                        kc = SP(DEL);
                                break;
                    case KP0:
                                if (locks & NUM)
                                        kc = '0';
                                else
                                        kc = SP(INS);
                                break;
                    case KP1:
                                if (locks & NUM)
                                        kc = '1';
                                else
                                        kc = SP(END);
                                break;
                    case KP2:
                                if (locks & NUM)
                                        kc = '2';
                                else
                                        kc = SP(DOWN);
                                break;
                    case KP3:
                                if (locks & NUM)
                                        kc = '3';
                                else
                                        kc = SP(PGDOWN);
                                break;
                    case KP4:
                                if (locks & NUM)
                                        kc = '4';
                                else
                                        kc = SP(LEFT);
                                break;
                    case KP5:
                                if (locks & NUM)
                                        kc = '5';
                                else
                                        return ;
                                break;
                    case KP6:
                                if (locks & NUM)
                                        kc = '6';
                                else
                                        kc = SP(RIGHT);
                                break;
                    case KP7:
                                if (locks & NUM)
                                        kc = '7';
                                else
                                        kc =  SP(HOME);
                                break;
                    case KP8:
                                if (locks & NUM)
                                        kc = '8';
                                else
                                        kc = SP(UP);
                                break;
                    case KP9:
                                if (locks & NUM)
                                        kc = '9';
                                else
                                        kc = SP(PGUP);
                                break;
                    default:
                                break;
                }
                shift = lshift | rshift;
                alt = lalt | ralt;
                ctrl = lctrl | rctrl;
        }
        if (ctrl) kc = C(kc);           // apply control if active
        tty_active = &tty[active];
        tty_active->inbuf[tty_active->pos++] = kc;
        tty_echo (kc);
  } else {
        esc = 1;
  }
}

void set_leds (uint8_t leds)
{
  outb (KBD_IO, SET_LEDS);
  outb (KBD_IO, leds);
}

