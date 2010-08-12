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

GLOBAL inb
GLOBAL inw
GLOBAL inl
GLOBAL outb
GLOBAL outw
GLOBAL outl
GLOBAL reboot

%define KBD_IO 0x60		; keyboard IO port (data and command)
%define KBDI_IO 0x64		; keyboard IO interface port (data and command)

; IO related functions 

; uin8_t inb (uint16_t port)
inb:
        push ebp
        mov ebp, esp

        xor eax, eax
        mov dx, [ebp+8]
        in al, dx

        mov esp, ebp
        pop ebp
        ret

; uint16_t inw (uint16_t port)
inw:
        push ebp
        mov ebp, esp

        xor eax, eax
        mov dx, [ebp+8]
        in ax, dx

        mov esp, ebp
        pop ebp
        ret

; uint32_t inl (uint16_t port)
inl:
        push ebp
        mov ebp, esp

        xor eax, eax
        mov dx, [ebp+4]
        in eax, dx

        mov esp, ebp
        pop ebp
        ret

; void outb (uint16_t port, uint8_t val)
outb:
        push ebp
        mov ebp, esp

        mov dx, [ebp+8]
        mov al, [ebp+16]
        out dx, al

        mov esp, ebp
        pop ebp
        ret

; void outw (uint16_t port, uint16_t val)
outw:
        push ebp
        mov ebp, esp

        mov dx, [ebp+8]
        mov ax, [ebp+16]
        out dx, ax

        mov esp, ebp
        pop ebp
        ret

; void outl (uint16_t port, uint32_t val)
outl:
        push ebp
        mov ebp, esp

        mov dx, [ebp+8]
        mov eax, [ebp+16]
        out dx, eax

        mov esp, ebp
        pop ebp
        ret

reboot:
	cli
kbwait:
        in al, KBDI_IO
        test al, 0x02
        jnz kbwait

        mov al, 0xFE
        out KBDI_IO, al
        ; if above doesn't work
        hlt


