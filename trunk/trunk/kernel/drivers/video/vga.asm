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

%include "vga.ash"

GLOBAL init_vga
GLOBAL enable_cursor
GLOBAL disable_cursor
GLOBAL set_cursor

; void init_ga ()
init_vga:
        ; select memory map (size and base)
        mov dx, GRAPH_ADDR
        mov al, GRAPH_MISC
        out dx, al
        mov dx, GRAPH_DATA
        in al, dx
        and al, MEM_MAP_SEL_RESET       ; drop desired bits to zero
        or al, MEM_MAP_128K             ; set them desired value
        out dx, al
        
        ; where to map CRTC register IO ports (answer: to 0x3Dx)
        mov dx, EXT_MISC_OUT_RD
        in al, dx
        and al, IOAS_RESET
        or al, IOAS_03DX
        mov dx, EXT_MISC_OUT_WR
        out dx, al
        
        ret

; TODO: test!!
; void enable_cursor ()
enable_cursor:
        mov dx, CRTC_ADDR
        mov al, CRTC_CUR_START
        out dx, al 
        mov dx, CRTC_DATA
        in al, dx
        and al, CUR_DISABLE_RESET
        out dx, al
        
        ret

; TODO: test!
; void disable_cursor ()
disable_cursor:
        mov dx, CRTC_ADDR
        mov al, CRTC_CUR_START
        out dx, al 
        mov dx, CRTC_DATA
        in al, dx
        and al, CUR_DISABLE_RESET
        or al, CUR_DISABLE
        out dx, al

        ret

; void set_cursor (uint16_t addr)
set_cursor:
        push ebp
        mov ebp, esp

        mov bx, [ebp+8]                 ; 16-bit cursor address from stack
        mov dx, CRTC_ADDR
        mov al, CRTC_CUR_LOC_HIGH
        out dx, al
        mov ax, bx
        and ax, 0xFF00
        shr ax, 8
        mov dx, CRTC_DATA
        out dx, al
                
        mov dx, CRTC_ADDR
        mov al, CRTC_CUR_LOC_LOW
        out dx, al
        mov ax, bx
        and ax, 0x00FF
        mov dx, CRTC_DATA
        out dx, al
        
        mov esp, ebp
        pop ebp
        
        ret

