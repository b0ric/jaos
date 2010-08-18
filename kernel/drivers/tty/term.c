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

#include <stdint.h>
#include <term.h>
#include "tty.h"

void init_terms (uint8_t idx)
{
  uint8_t i;

  for (i = 0; i < NR_TTYS; i++)
        init_tty (&tty[idx]);
}

void kprint (uint8_t *str)
{
  uint8_t cnt;
  uint8_t *ch  = str;
  struct tty_t *active_tty = &tty[active];

  for (cnt = 0; *ch != '\0'; ch++, cnt++)
        ;
  copy_mem (active_tty->outbuf, str, cnt);
  active_tty->count = cnt;
  active_tty->write(active_tty);
}


