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

#include "console.h"

void cons_init (console_t *cons);
void cons_write (console_t *cons, uint8_t *buf, uint8_t count);
void cons_out_char (console_t *cons, uint8_t ch);
void cons_out_raw (console_t *cons, uint8_t ch);
void cons_scroll (console_t *cons, uint8_t to_line);

static console_t console[NR_CONS];

void cons_init (console_t *cons)
{
  cons->addr = VGA_MEM_ADDR;
  cons->mem_size = ~0;               // 64k of VGA memory
  cons->cursor = 0;
  cons->line = 0;
  cons->col = 0;
  cons->prev_char = '\n';
  cons->attr = FORE(COL_WHITE) | BACK(COL_BLACK);
}

void cons_write (console_t *cons, uint8_t *buf, uint8_t count)
{
  uint8_t i;

  for (i = 0; i < count; i++)
        cons_out_char(cons, buf[i]);
}

void cons_out_char (console_t *cons, uint8_t ch)
{
  uint8_t tab_len;
  uint8_t i;

  if (ch < 0x20) {
        switch (ch) {
          case '\a':    break;          // bell is not implemented yet
          case '\b':
                if (cons->prev_char == '\n') break; //don't delete entered data
                if (cons->col == 0) {
                        cons->line--;
                        cons->col = VGA_TEXT_COLS - 1;
                } else
                        cons->col--;
                break;
          case '\t':
                tab_len = TAB_LEN - (console->col + 1) & TAB_MASK;
                for (i = 0; i < tab_len; i++)
                        cons_out_raw (cons, ' ');
                break;
          case '\n':
                cons->line++;
                cons->col = 0;
                cons_scroll (cons, cons->line);
                break;
          default:                      // not implemented, do-nothing-symbol, etc
                break;
        }
  } else {
        if (ch != 127)                  // not DEL code
                cons_out_raw (cons, ch);
  }
}

/* just write raw char to console without additional checks */
void cons_out_raw (console_t *cons, uint8_t ch)
{
  uint16_t vid_data;
  uint32_t vid_addr;

  vid_data = ch & (cons->attr << 8);
  vid_addr = cons->addr + (cons->line * VGA_TEXT_COLS + cons->col) * 2;
  
  vga_copy (vid_addr, vid_data);
  
  if (cons->col == VGA_TEXT_COLS - 1) {
        cons->col = 0;
        cons->line++;
        cons_scroll (cons, cons->line);
  } else
        cons->col++;
}

/* the last line it'll show is the to_line */
/* TODO: handle infinite scrolling */
void cons_scroll (console_t *cons, uint8_t to_line)
{
  uint16_t scroll_addr;
  
  if (to_line >= VGA_TEXT_LINES - 1) {
        scroll_addr = (to_line - VGA_TEXT_LINES) * VGA_TEXT_COLS;
        vga_scroll (scroll_addr);
  }
}

