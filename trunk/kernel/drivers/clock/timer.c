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

#include "timer.h"

/* PIT ports */
#define PIT_CH0_DATA 0x40
#define PIT_CH1_DATA 0x41
#define PIT_CH2_DATA 0x42
#define PIT_CMD 0x43
/* PIT channel 0 initialization command */
#define PIT_CH0_INIT 0x34       // ch0, both bytes, rate generator, hex
/* PIT frequency divider */
#define FREQ_DIVIDER 29833      // = 29833 or 1193182 Hz / 29833 ~ 40 Hz

uint32_t timer;
static struct watchdog_t *wdog_hd;

void init_timer (void)
{
  outb (PIT_CMD, PIT_CH0_INIT);
  outb (PIT_CH0_DATA, FREQ_DIVIDER & 0x00FF);
  outb (PIT_CH0_DATA, (FREQ_DIVIDER & 0xFF00) >> 8);
  
  wdog_hd = (struct watchdog_t *) 0;
}

void timer_handle_irq (void)
{
  timer++;
  
  /* process watchdog timers */
  if (wdog_hd) {
        wdog_hd->exp_time--;
        if (!wdog_hd->exp_time) {       // timer expired
                (*wdog_hd->handler) ();
                wdog_hd = wdog_hd->next;
        }
  }
}

/* add watchdog timer with experation time calculated from present moment
   i.e. just difference in time, not an absolute value. Also this time is
   calculated in ticks that is 25ms long by default */
void add_watchdog_timer (struct watchdog_t *wdog_new)
{
  struct watchdog_t *wd = wdog_hd, *wdog_tmp;

  while (wd->exp_time < wdog_new->exp_time)     // look for right position in the chain
        wd = wd->next;
  wdog_tmp = wd->next;
  wd->next = wdog_new;
  wdog_new->next = wdog_tmp;
}

// TODO: complete this function
void delete_watchdog_timer (struct watchdog_t *wdog)
{
  //#error TODO: complete this function
}

uint32_t get_uptime ()
{
  return timer;
}

