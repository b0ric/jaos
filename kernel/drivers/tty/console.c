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

#define VGA_MEM_ADDR 0xA0000

/* Video mode defenitions */
#define VGA_TEXT_LINES 25
#define VGA_TEXT_COLS 80

/* VGA color attributes (suffix L = Light, D = Dark)*/
#define COL_BLACK 0x00
#define COL_BLUE 0x01
#define COL_GREEN 0x02
#define COL_CYAN 0x03
#define COL_RED 0x04
#define COL_MAGENTA 0x05
#define COL_BROWN 0x06
#define COL_LGRAY 0x07
#define COL_DGRAY 0x08
#define COL_LBLUE 0x09
#define COL_LGREEN 0x0A
#define COL_LCYAN 0x0B
#define COL_LRED 0x0C
#define COL_LMAGENTA 0x0D
#define COL_YELLOW 0x0E
#define COL_WHITE 0x0F

#define FORE(val) (val)
#define BACK(val) ((val) << 4)

console_t cons[NR_CONS];

static void cons_out_char (console_t *cons, uint8_t ch);
static void cons_out_raw (console_t *cons, uint8_t ch);
static void cons_scroll (console_t *cons, uint8_t to_line);

void cons_init (console_t *cons)
{
  cons->addr = VGA_MEM_ADDR;
  cons->mem_size = 0x7FFF;              // 32k of VGA memory
  cons->cursor = 0;
  cons->line = 0;
  cons->col = 0;
  cons->prev_char = '\n';
  cons->attr = FORE(COL_LGRAY) | BACK(COL_BLACK);
}

void cons_write (struct tty_t *term)
{
  uint8_t i;
  console_t *active;

  for (i = 0; i < term->count; i++)
        cons_out_char(&cons[term->idx], term->outbuf[i]);
  /* change cursor position */
  active = &cons[term->idx];
  active->cursor = active->line * VGA_TEXT_COLS + active->col;
  vga_set_cursor (active->cursor);
}

void cons_out_char (console_t *cons, uint8_t ch)
{
  uint8_t tab_len;
  uint8_t i;

  if (ch < 0x20) {
        switch (ch) {
          case '\0': return;
          case '\a': break;             // bell is not implemented yet
          case '\b':
                if (cons->prev_char == '\n') break; //don't delete entered data
                if (cons->col == 0) {
                        cons->line--;
                        cons->col = VGA_TEXT_COLS - 1;
                } else
                        cons->col--;
                break;
          case '\t':
                tab_len = TAB_LEN - (cons->col + 1) & TAB_MASK;
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

  vid_data = ch | (cons->attr << 8);
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

