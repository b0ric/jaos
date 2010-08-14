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

#include <mem.h>

MemInfo mem[3];

int kmain ()
{
  init_vga ();
  prnstr ("Starting kernel routines...\n");
  prnstr ("Initializing 8259A PIC controller\n");  
  init_pic ();
  prnstr ("Building and loading IDT table\n");  
  load_idt ();
  prnstr ("Initializing 8253 PIT controller\n");
  init_timer ();
  prnstr ("Enabling interrupts\n");  
  enable_ints ();

  return 0x1f;
}

