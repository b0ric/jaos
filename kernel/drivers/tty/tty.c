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

#include <term.h>
#include "tty.h"
#include "console.h"
#include "keybd.h"

struct tty_t tty[NR_TTYS];
uint8_t active;                 // active tty index

void tty_nop (struct tty_t *term);
void init_tty (struct tty_t *term);

static uint8_t init_tty_once_ = 1;

void init_tty_once (void)
{
  init_vga ();
  init_kb ();
  init_tty_once_ = 0;
}

/* init standart terminal (monitor & keyboard) */
void init_tty (struct tty_t *term)
{
  static i = 0;                 // console number associated with this terminal

  if (init_tty_once_)
        init_tty_once ();

  term->idx = i++;
  cons_init (&cons[term->idx]);
  term->open = tty_nop;
  term->close = tty_nop;

  term->read = tty_nop;
  *term->inbuf = '\0';
  term->pos = 0;

  term->write = cons_write;
  *term->outbuf = '\0';
  term->count = 0;
}

void tty_nop (struct tty_t *term)
{
  // do nothing
}

void tty_echo (keycode_t kc)
{
  uint8_t ch;

  if (ISSPECIAL(kc)) {
        
  } else {
        if (ISALT(kc) || ISSHIFT(kc) || ISCTRL(kc)) {
        
        } else {
                ch = kc & 0x00FF;
                cons_out_char (&cons[tty[active].idx], ch);
        }
  }
}

