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

%ifndef _VGA_ASH
%define _VGA_ASH

; VGA registers IO ports
; external registers
%define EXT_MISC_OUT_RD 0x3CC
%define EXT_MISC_OUT_WR 0x3C2
; graphics registers
%define GRAPH_ADDR 0x3CE
%define GRAPH_DATA 0x3CF
; CRT Controller registers (can be mapped to different IO ports!)
%define CRTC_ADDR 0x3D4
%define CRTC_DATA 0x3D5

; VGA registers indexes defenitions
%define GRAPH_MISC 0x06
%define CRTC_CUR_START 0x0A
%define CRTC_CUR_LOC_HI 0x0E
%define CRTC_CUR_LOC_LO 0x0F
%define CRTC_START_ADDR_HI 0x0C
%define CRTC_START_ADDR_LO 0x0D

; memory map bits (VGA_GRAPH_MISC register)
%define MEM_MAP_SEL_RESET 0xF3
%define MEM_MAP_128K 0x00
%define MEM_MAP_64K 0x04
%define MEM_MAP_32K_LOW 0x08
%define MEM_MAP_32K_HIGH 0x0C

; Miscellaneous Output Register (EXT_MISC_OUT) Input/Output Address Select reset
%define IOAS_RESET 0xFE
%define IOAS_03BX 0x00
%define IOAS_03DX 0x01

; Cursor Start Register (CRTC_CUR_START) Cursor Disable
%define CUR_DISABLE_RESET 0xDF
%define CUR_ENABLE 0x00
%define CUR_DISABLE 0x01

%endif ; _VGA_ASH

