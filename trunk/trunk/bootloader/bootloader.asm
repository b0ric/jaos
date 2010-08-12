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

[BITS 16]
[ORG 0x7C00]

%include "bootloader.ash"

        xor ax, ax
        mov ds, ax
        mov es, ax
        cli
        mov ss, ax
        mov sp, 0xFFFF
        sti

        ; print hello message
        mov ax, start
        call print

; set video mode
        xor ah, ah
        mov al, VM_VGA_TEXT
        int 0x10

; we have to retry reading sectors at least three times
; 'cause motor should have enough time to spin up
readsect:
        ; reset first floppy drive (0x00 DL value)
        xor ah, ah
        mov dl, [drive]
        int 0x13

        ; read 8 sectors (4k) from drive
        mov ah, 0x02
        mov al, KERN_SECT_SIZE          ; 8 sectors
        mov cx, 0x0002                  ; CHS = (0, 0, 2) and C=[0:1023], H=[0:1], S=[1:63] - sector to start from
        mov dh, 0x00                    ;  ^  = (   ^   )
        mov dl, [drive]
        mov bx, KERN_ELF_ADDR_HI        ; sectors will be read to ES:BX = KERN_ELF_ADDR_HI:KERN_ELF_ADDR_LO
        mov es, bx
        mov bx, KERN_ELF_ADDR_LO
        int 0x13
        jc readsect                     ; retry if error

        ; ds register should contain KERN_RAW_ADDR_HI (and it's already in es)
        mov ax, es
        mov ds, ax
        ; number of ELF program headers
        mov dx, [ds:KERN_ELF_ADDR_LO+ELF_PHDRS_NUM]
        ; we store offset of current Phdr in bp
        mov ax, [ds:KERN_ELF_ADDR_LO+ELF_PHDRS_OFF]
        mov bp, ax

readseg:
        ; is segment loadable?
        mov ax, [ds:bp+ELF_PHDR_TYPE]
        and ax, ELF_PT_LOAD
        jz nextseg

        ; number of bytes to copy
        mov cx, [ds:bp+ELF_PHDR_DATA_SIZE]
        ; we won't deal with segments 0 bytes long
        jcxz nextseg

        ; setup copy position to ds:di
        mov ax, [ds:bp+ELF_PHDR_SEG_OFF]
        mov si, ax

        ; is segment executable?
        mov ax, [ds:bp+ELF_PHDR_SEG_FLAGS]
        and ax, ELF_PF_X
        jz data

code:
        ; address to copy code to
        mov ax, KERN_CODE_ADDR_HI
        mov es, ax
        mov di, KERN_CODE_ADDR_LOW
        ; copy block of code
        cld
        rep movsb
        jmp nextseg

data:        
        ; address to copy data to
        mov ax, KERN_DATA_ADDR_HI
        mov es, ax
        mov di, KERN_DATA_ADDR_LOW
        ; determine size in memory (surplus will be zeroed)
        mov ax, [ds:bp+ELF_PHDR_SEG_SIZE]   
        sub ax, cx
        ; copy block of data
        cld
        rep movsb
        ; zero .bss region
        mov cx, ax
        xor ax, ax
        rep stosb

nextseg:
        ; dec number of processed ELF segments
        dec dx
        jz memmap
        ; get program header size
        mov ax, [ds:KERN_ELF_ADDR_LO+ELF_PHDRS_SIZE]
        ; add it to bp to make it point to next header
        add bp, ax
        jmp readseg

memmap:
        ; firstly clear data segments
        xor ax, ax
        mov ds, ax
        mov es, ax
        ; obtain low memory map
        int 0x12
        jc memmap
        mov dx, X1KB
        mul dx
        mov [low_mem], dword LOW_MEM_BASE32 ; low_mem base
        mov [low_mem+4], ax
        mov [low_mem+6], dx
        ; extended memory map
        mov ax, 0xE801
        int 0x15
        jc a20line
        mov dx, X1KB
        mul dx
        mov [hi_mem], dword 0x00100000  ; 1MB base
        mov [hi_mem+4], ax
        mov [hi_mem+6], dx
        mov ax, bx
        mov dx, X64K
        mul dx
        mov [ext_mem], dword 0x01000000 ; 16MB base
        mov [ext_mem+4], ax
        mov [ext_mem+6], dx

; A20 line has to be enabed in order to access all the 4Gb of memory
a20line:
        ; quick trick to check whether A20 line is enabled 
        mov ax, [bootsig]       ; address of boot signature - 55AA
        mov bx, 0xFFFF
        mov es, bx
        mov bx, [es:0x7E0E]     ; address of boot signature + 1Mb
        cmp ax, bx
        jne prot

        ; enable A20 line (this method won't work on all systems, especially old ones)
        mov ax, 0x2401
        int 0x15
        jnc prot
        jmp $

prot:
        ; copy GDT to the lowest memory boundary 
        xor ax, ax        
        ; DS is already zeroed
        mov es, ax
        mov si, gdt
        mov di, LOW_MEM_BASE16
        mov cx, gdtsize
        cld
        rep movsb
        ; set gdtr fields properly
        mov ax, LOW_MEM_BASE16
        add ax, gdtsize
        dec ax
        mov [gdtr], ax
        mov [gdtr+2], dword LOW_MEM_BASE32
        
        ; switch to the protected mode
        cli
        ; load Global Descriptor Table
        lgdt [gdtr]
        mov eax, cr0
        or eax, 1               ; set PE bit
        mov cr0, eax
        ; save code segment celector and pass it to kernel
        mov ax, datasel
        ; jump to kernel
        jmp dword codesel:CODE_SEG_ENTRY


; print null-terminated string
; eax   pointer to null-terminated string
print:
        pusha
        cld
        mov si, ax              ; beginning of the array
        mov ah, 0x0E            ; print char interrupt code
        mov bh, 0x00            ; page number
        mov bl, 0x07            ; text attribute 0x07 is lightgrey font on black background

nextchar:
        lodsb
        test al, al
        jz .exit
        int 0x10
        jmp nextchar
.exit
        popa
        ret

start:  db 'Starting bootloader...', 0x0D, 0x0A, 0
; first floppy drive
drive:  db 0x00

; Global Discriptor Table
;
;  31                    19      1615     12       08               00
; -------------------------------------------------------------------
; | Base 24:32 |G|DB|0|0| Lim 16:19|P|DPL |S| Type  | Base    16:23 |
; -------------------------------------------------------------------
; |        Base Address 0:15       |         Segment Limit          |
; -------------------------------------------------------------------
;  31                               15                            00
gdtr:
        dw gdtend-1
        dd gdt
gdt:
gdt0:                   ; NULL-descriptor
        dw 0x0
        dw 0x0
        db 0x0
        db 0x0
        db 0x0
        db 0x0
codesel equ $-gdt
        dw 0xFFFF       ; limit 0:15
        dw 0x0          ; base 0:15
        db 0x0          ; base 16:23
        db 0x9A         ; P = 1, DPL = 00, S = 1, Type = 1010 (Exec, Read, Non-conforming)
        db 0xCF         ; G = 1, D/B = 1, 0, 0, Limit 16:19 
        db 0x0          ; base 24:32
datasel equ $-gdt
        dw 0xFFFF       ; limit 0:15
        dw 0x0          ; base 0:15
        db 0x0          ; base 16:23
        db 0x92         ; P = 1, DPL = 00, S = 1, Type = 0010 (Read, Write)
        db 0xCF         ; G = 1, D/B = 1, 0, 0, Limit 16:19 
        db 0x0          ; base 24:32
gdtend:
gdtsize equ $-gdt

; upper limit for the code
TIMES (446 - 24) - ($ - $$) db 0x00

; memory map
; placed here to obtain constant address in memory and will be read by the kernel
mem:
low_mem:
        dd 0x0
        dd 0x0
hi_mem:
        dd 0x0
        dd 0x0
ext_mem:
        dd 0x0
        dd 0x0

; Master Boot Record Area
TIMES 510 - ($ - $$) db 0x00

; bootable media signature
bootsig:
db 0x55, 0xAA

