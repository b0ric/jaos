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
static uint8_t bufpos = 0;

void keybd_handle_irq (void)
{
  scancode_t scode;
  kb_buffer[bufpos % KBD_BUF_SZ] = inb (KBD_IO);
  bufpos++;
}


