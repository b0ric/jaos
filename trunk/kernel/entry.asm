;
; Copyright (c) 2010 Borisov Alexandr
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License version 2 as
; published by the Free Software Foundation;
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
;
; Author: Borisov Alexandr <b0ric.alex@gmail.com>
;

[BITS 32]

GLOBAL _start
EXTERN kmain
EXTERN mem
EXTERN reboot

; here we need to setup new GDT table which is different from bootloader GDT
; kernel also uses flat memory model because the most of C compilers assume this
; approach

; kernel TSS
%define KTSS_LOW 0x0600
%define KTSS_LIM_LOW 0x0067

; constants to read memory map
%define MEM_MAP_POS 0x00007DA6
%define MEM_MAP_SIZE 24

[SECTION .text]
_start:
        ; interrupt now are disabled by bootloader
        ; overrided bootloader GDT
        lgdt [gdtr]
        jmp dword kcode:krun

krun:
        ; setup kernel segments
        xor eax, eax
        mov ax, kdata
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        mov esp, 0xFFFF

        xor eax, eax
        mov ax, ktss
        ltr ax

        cld
        ; move memory map to respective position
        mov esi, MEM_MAP_POS
        mov edi, mem
        mov ecx, dword MEM_MAP_SIZE
        rep movsb

        ; zero tss space
        xor eax, eax
        mov edi, KTSS_LOW
        mov ecx, KTSS_LIM_LOW
        rep stosb

        call kmain
        ;int 0x21

        jmp $

        jmp reboot              ; no need to call it, it won't return(:

[SECTION .data]
; Global Discriptor Table
;
;  31                    19      1615     12       08             00
; -------------------------------------------------------------------
; | Base 24:32 |G|DB|0|0| Lim 16:19|P|DPL |S| Type  | Base    16:23 |
; -------------------------------------------------------------------
; |        Base Address 0:15       |         Segment Limit          |
; -------------------------------------------------------------------
;  31                               15                            00
gdtr:                   ; new one to override bootloader configuration
        dw gdtend-1
        dd gdt 
ALIGN 16
gdt:
gdt0:                   ; NULL-descriptor
        dw 0x0
        dw 0x0
        db 0x0
        db 0x0
        db 0x0
        db 0x0
kcode   equ $-gdt
        dw 0xFFFF       ; limit 0:15
        dw 0x0          ; base 0:15
        db 0x0          ; base 16:23
        db 0x9A         ; P = 1, DPL = 00, S = 1, Type = 1010 (Exec, Read, Non-conforming)
        db 0xCF         ; G = 1, D/B = 1, 0, 0, Limit 16:19 
        db 0x0          ; base 24:32
kdata   equ $-gdt
        dw 0xFFFF       ; limit 0:15
        dw 0x0          ; base 0:15
        db 0x0          ; base 16:23
        db 0x92         ; P = 1, DPL = 00, S = 1, Type = 0010 (Read, Write)
        db 0xCF         ; G = 1, D/B = 1, 0, 0, Limit 16:19 
        db 0x0          ; base 24:32
ktss    equ $-gdt
        dw KTSS_LIM_LOW ; limit 0:15
        dw KTSS_LOW     ; base 0:15
        db 0x0          ; base 16:23
        db 0x89         ; P = 1, DPL = 00, S = 0, Type = 1011 (32-bit TSS, Busy)
        db 0x00         ; G = 0, 0, 0, AVL = 0, Limit 16:19
        db 0x0          ; base 24:32
code    equ $-gdt       ; unprivileged flat code segment
        dw 0xFFFF       ; limit 0:15
        dw 0x0          ; base 0:15
        db 0x0          ; base 16:23
        db 0xFA         ; P = 1, DPL = 11, S = 1, Type = 1010 (Exec, Read, Non-conforming)
        db 0xCF         ; G = 1, D/B = 1, 0, 0, Limit 16:19 
        db 0x0          ; base 24:32
data    equ $-gdt       ; unpriviledged flat data segment
        dw 0xFFFF       ; limit 0:15
        dw 0x0          ; base 0:15
        db 0x0          ; base 16:23
        db 0xF2         ; P = 1, DPL = 11, S = 1, Type = 0010 (Read, Write)
        db 0xCF         ; G = 1, D/B = 1, 0, 0, Limit 16:19 
        db 0x0          ; base 24:32
gdtend:

