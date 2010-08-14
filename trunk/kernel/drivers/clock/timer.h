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

#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>

typedef void (*watchdog_func_t)(void);

struct watchdog_t {
  struct watchdog_t *next;      // next timer in the chain
  uint32_t exp_time;            // expiration time (time to wait after previous watchdog timer)
  watchdog_func_t handler;
};

extern uint32_t timer;

void init_timer ();
void timer_handle_irq (void);
void add_watchdog_timer (struct watchdog_t *wdog_new);
uint32_t get_uptime ();

#endif /* _TIMER_H */

