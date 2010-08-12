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

; TODO: translate this code into C source and do printing with memcpy() func
;       'cause you'll be screwed up processing all special characters, attributes, scrolling, etc

GLOBAL cls
GLOBAL printchar
GLOBAL prnstr

%define VGA_BUF 0x0000A0000
%define TEXT_ATTR 0x07          ; black background, white fore

[SECTION .text]

; cls ();
; fill screen with spaces and set position pointer to 0
cls:
        cld
        mov eax, 0x0020         ; black, black spaces
        mov edi, VGA_BUF
        mov ecx, 2000
        rep stosw
        xor eax, eax
        mov [line], ax
        mov [col], ax
        ret

;===============================================================================
; printchar (unsigned char)
; esi - source addr (from function parameter)
; edi - destination addr (video buffer)
; cl  - column number of the current line
; ch  - line number
; ebx - zeros we need (xor is more efficient than the mov)
printchar:
        push ebp
        mov ebp, esp
        
        xor ebx, ebx
        ; calculate destination address
        mov edi, VGA_BUF
        mov ch, [line]
        mov al, ch
        mul byte [ncols]        ; multiply by number of columns
        mov cl, [col]
        add al, cl
        shl ax, 1               ; multiply by 2 'cause each char takes 2 bytes
        add di, ax
        mov ah, TEXT_ATTR

        mov al, [ebp+8]         ; extract char from stack
        cmp al, 0x0D            ; carriage return
        jz .CR
        cmp al, 0x0A            ; line feed
        jz .LF
        mov [edi], ax           ; print char
        inc cl
        cmp cl, [ncols]
        jnz .exit
.LF:                            ; works when '\n' encountered or line is carred
        inc ch
.CR:
        mov cl, bl              ; set column to 0
        jmp .exit     
.exit:
        mov [col], cl
        mov [line], ch
        mov esp, ebp
        pop ebp
        ret
;===============================================================================

;===============================================================================
; prnstr (unsigned char *)
; print null-terminated string
; just some explanations to make things clear:
; our regime is 80x25 so we have to operate with matrix of words
; with 80 columns and 25 rows
; esi - source addr (from function parameter)
; edi - destination addr (video buffer)
; cl  - column number of the current line
; ch  - line number
; ebx - zeros we need (xor is more efficient than the mov)
prnstr:
        push ebp
        mov ebp, esp

        xor ebx, ebx
        ; source address
        mov esi, [ebp+8]
        ; calculate destination address
        mov edi, VGA_BUF
        mov ch, [line]
        mov al, ch
        mul byte [ncols]        ; multiply by number of columns
        mov cl, [col]
        add al, cl
        shl ax, 1               ; multiply by 2 'cause each char takes 2 bytes
        add di, ax
        mov ah, TEXT_ATTR
        cld
next:
        lodsb
        cmp al, 0x0D            ; carriage return
        jz .CR
        cmp al, 0x0A            ; line feed
        jz .LF
        test al, al
        jz .exit
        stosw
        inc cl
        cmp cl, [ncols]
        jnz next
.LF:                            ; works when '\n' encountered or line is carred
        inc ch
.CR:
        mov cl, bl              ; set column to 0
        jmp .exit     
.exit:
        mov [col], cl
        mov [line], ch
        mov esp, ebp
        pop ebp
        ret
;===============================================================================

setpos:
        jmp $                   ; TODO

[SECTION .data]
ncols:  db 80
nlines: db 25
line:   db 0x0
col:    db 0x0








