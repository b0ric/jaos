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

%ifndef BOOTLOADER_ASH
%define BOOTLOADER_ASH

; desired video mode
%define VM_VGA_TEXT 0x03                        ; VGA: 80x25, font 9x16, 720x400, 16bit

; memory related constants
; size multipliers
%define X1KB 1024
%define X64K 0xFFFF

%define LOW_MEM_BASE16 0x0500
%define LOW_MEM_BASE32 0x00000500

; kernel related constants
; kernel size rounded up to 512-byte boundary
%define KERN_SECT_SIZE 8

; address to read raw kernel to
; will be calculated at 16-bit mode (as KERN_RAW_ADDR_HI<<4+KERN_RAW_ADDR_LO)
%define KERN_ELF_ADDR_HI 0x0500
%define KERN_ELF_ADDR_LO 0x0000

; these addresses are also defined for 32-bit mode in entry.asm
%define KERN_CODE_ADDR_HI 0x0100
%define KERN_CODE_ADDR_LOW 0x0000
%define KERN_DATA_ADDR_HI 0x0200                ; .bss and .data together
%define KERN_DATA_ADDR_LOW 0x0000

; ELF format constants
; used to calculate size and position of .text and .data segments
; NOTE: we have to be very aware here because we work in 16-bit mode
;	but ELF flags and types are 32-bit in size

; ELF header offsets
%define ELF_PHDRS_OFF 28                         ; offset of program headers
%define ELF_PHDRS_SIZE (ELF_PHDRS_OFF + 14)      ; size of one program header
%define ELF_PHDRS_NUM (ELF_PHDRS_SIZE + 2)       ; number of program headers

; ELF program header offsets
%define ELF_PHDR_TYPE 0                         ; segment type offset
%define ELF_PHDR_SEG_OFF (ELF_PHDR_TYPE + 4)    ; segment offset in program header
%define ELF_PHDR_DATA_SIZE (ELF_PHDR_SEG_OFF + 12)      ; segment data size in file
%define ELF_PHDR_SEG_SIZE (ELF_PHDR_DATA_SIZE + 4)      ; segment data size in memory
%define ELF_PHDR_SEG_FLAGS (ELF_PHDR_SEG_SIZE + 4)      ; segment flags

; ELF program header types
%define ELF_PT_LOAD 0x0001                      ; loadable type of segment
; ELF program header types
%define ELF_PF_X 0x0001	                        ; executable segment

; kernel entry point
%define CODE_SEG_ENTRY 0x1000

%endif ; BOOTLOADER_ASH

