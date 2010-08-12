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

#ifndef MEM_H
#define MEM_H

#include <stdint.h>

typedef struct {
        uint32_t base;
        uint32_t size;
} MemInfo;

/* LDT and GDT entries */
typedef struct {
        uint16_t limit_lo;      // limit low 16  bits
        uint16_t base_lo;       // base low 16 bits
        uint8_t base_mid;       // base middle 8 bits
        uint8_t type;           // |P|PL|S|TYPE
        uint8_t other;          // |G|D|L|A|LIMT
        uint8_t base_hi;        // base hi 8 bits
} seg_sel; 

/* some macros to setup segment fields */
#define SET_SEG_LIMIT(seg,limit)                                                \
                (seg.limit_lo = ((limit) & 0x0000FFFF));                        \
                (seg.type = (seg.type | (((limit) & 0x000F0000) >> 16)));

#define SET_SEG_BASE(seg,base)                                                  \
                (seg.base_lo = ((base) & 0x0000FFFF));                          \
                (seg.base_mid = ((base) & 0x00FF0000) >> 16);                   \
                (seg.base_hi = ((base) & 0xFF000000) >> 24);


/* code and data segment types */
#define SEG_DATA 0x00
#define SEG_CODE 0x08

/* code and data common */
#define ACCESS          0x01    // segment was accessed

/* data segment type */
#define D_READ          0x00
#define D_WRITE         0x02
#define D_EXDOWN        0x04    // expand-down segment

/* code segment type */
#define C_EXEC          0x00
#define C_READ          0x02
#define C_CONFORM       0x04

/* system segment types */
#define S_LDT           0x02
#define S_TASK_GATE     0x05
#define S_TSS_AVL       0x09    // 32-bit
#define S_TSS_BUSY      0x0B    // 32-bit
#define S_CALL_GATE     0x0C    // 32-bit
#define S_INT_GATE      0x0E    // 32-bit
#define S_TRAP_GATE     0x0F    // 32-bit


/* S (System) flag */
#define SEG_SYS 0x00
#define SEG_USR 0x10

/* DPL (Descriptor Previlege Level) flags */
#define SEG_LEVEL_0     0x00
#define SEG_LEVEL_1     0x20
#define SEG_LEVEL_2     0x40
#define SEG_LEVEL_3     0x50

/* P (Present) flag */
#define SEG_PRESENT     0x80
#define SEG_NOTPRESENT  0x00

/* segment other field flags */
#define SEG_OTH 0xC0            // G = 1, D/B = 1, L = 0, AVL = 0

#endif /* MEM_H */

