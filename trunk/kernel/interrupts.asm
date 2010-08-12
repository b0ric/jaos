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

GLOBAL init_pic
GLOBAL load_idt
GLOBAL enable_ints
GLOBAL disable_ints
EXTERN timer_handle_irq
EXTERN keybd_handle_irq
EXTERN printchar

; interrupt descriptor flag
%define INTR_GATE_FLAG 0x8F00
; 8259A PIC ports
%define MASTER_CMD 0x20
%define MASTER_DATA 0x21
%define SLAVE_CMD 0xA0
%define SLAVE_DATA 0xA1
; PIC Command Words (for master and slave)
%define PIC_ICW1 0x11           ; edge-triggered, 8 byte call, cascade, ICW4
%define PIC_MASTER_VEC 0x20
%define PIC_SLAVE_VEC 0x28
%define PIC_MASTER_ICW3 0x04    ; slave at second input
%define PIC_SLAVE_ICW3 0x02     ; slave input
%define PIC_ICW4 0x01           ; SFNM = 0, non-buffered, normal EOI, 8086
; interrupts masks
%define PIC_MASTER_MASK 0xFC
%define PIC_SLAVE_MASK 0xFF

%macro save 0
        pushad
        push ds
        push es
        push fs
        push gs
%endmacro ; restore

%macro restore 0
        pop gs
        pop fs
        pop es
        pop ds
        popad
%endmacro ; restore

%macro nohandler 0
        iret
%endmacro ; nohandler

%macro build_int_gate 2
        mov eax, (%1)
        mov ebx, eax
        and eax, 0x0000FFFF
        and ebx, 0xFFFF0000
        shr ebx, 16
        mov [idt+(%2)*8+0], ax
        mov [idt+(%2)*8+2], cs
        mov [idt+(%2)*8+4], word 0x8E00
        mov [idt+(%2)*8+6], bx
%endmacro

%macro pic_eoi 1
        mov al, 0x20
        out MASTER_CMD, al
%if ((%1) >= 8)
        out SLAVE_CMD, al
%endif
%endmacro ; pic_eoi


[SECTION .text]
; initialize 8259A PIC
init_pic:
        ; master init
        ; ICW1
        mov al, PIC_ICW1
        out MASTER_CMD, al
        ; ICW2
        mov al, PIC_MASTER_VEC
        out MASTER_DATA, al
        ; ICW3
        mov al, PIC_MASTER_ICW3
        out MASTER_DATA, al
        ; ICW4
        mov al, PIC_ICW4
        out MASTER_DATA, al
        ; mask
        mov al, PIC_MASTER_MASK
        out MASTER_DATA, al

        ; slave init
        ; ICW1
        mov al, PIC_ICW1
        out SLAVE_CMD, al
        ; ICW2
        mov al, PIC_SLAVE_VEC
        out SLAVE_DATA, al
        ; ICW3
        mov al, PIC_SLAVE_ICW3
        out SLAVE_DATA, al
        ; ICW4
        mov al, PIC_ICW4
        out SLAVE_DATA, al
        ; mask
        mov al, PIC_SLAVE_MASK
        out SLAVE_DATA, al

	ret

; build IDT table and load it to IDTR
load_idt:
        build_int_gate (int_div_zero), 0
        build_int_gate (int_debug), 1
        build_int_gate (int_nmi), 2
        build_int_gate (int_breakpt), 3
        build_int_gate (int_overflow), 4
        build_int_gate (int_bound), 5
        build_int_gate (int_inv_opcode), 6
        build_int_gate (int_fpu_not_avail), 7
        build_int_gate (int_db_fault), 8
        ; int 0x09 is reserved
        build_int_gate (int_inv_tss), 10
        build_int_gate (int_seg_not_present), 11
        build_int_gate (int_stack_fault), 12
        build_int_gate (int_gen_prot), 13
        build_int_gate (int_pg_fault), 14
        ; int 0xFF is reserved
        build_int_gate (int_fpu_err), 16
        build_int_gate (int_align_check), 17
        build_int_gate (int_mach_check), 18
        build_int_gate (int_simd_fp), 19
        ; int 0x13 - int 0x1F is reserved
        build_int_gate (int_hw00), 32
        build_int_gate (int_hw01), 33
        ; int 0x22 is cascade, never raised
        build_int_gate (int_hw03), 35
        build_int_gate (int_hw04), 36
        build_int_gate (int_hw05), 37
        build_int_gate (int_hw06), 38
        build_int_gate (int_hw07), 39
        build_int_gate (int_hw08), 40
        build_int_gate (int_hw09), 41
        build_int_gate (int_hw10), 42
        build_int_gate (int_hw11), 43
        build_int_gate (int_hw12), 44
        build_int_gate (int_hw13), 45
        build_int_gate (int_hw14), 46
        build_int_gate (int_hw15), 47

        lidt [idtr]
        ret

; enable PIC interrupts
enable_ints:
	sti
	ret

; disable PIC interrupts
disable_ints:
	cli
	ret

; we assume that exeptions and interrupts cannot happen in kernel, but only in user processes
; TODO: write handlers that communicate with current user process

ALIGN 8
int_div_zero:           ; int 0x00
        nohandler

ALIGN 8        
int_debug:              ; int 0x01
        nohandler

ALIGN 8
int_nmi:                ; int 0x02
        nohandler

ALIGN 8
int_breakpt:            ; int 0x03
        nohandler

ALIGN 8
int_overflow:           ; int 0x04
        nohandler

ALIGN 8
int_bound:              ; int 0x05
        nohandler

ALIGN 8
int_inv_opcode:         ; int 0x06
        nohandler

ALIGN 8
; TODO: write appropriate handler to save x87 registers
int_fpu_not_avail:      ; int 0x07
        nohandler

ALIGN 8
int_db_fault:           ; int 0x08
        pop eax
        nohandler

ALIGN 8
int_inv_tss:            ; int 0x0A
        pop eax
        nohandler

ALIGN 8
int_seg_not_present:    ; int 0x0B
        pop eax
        nohandler

ALIGN 8
; TODO: don't use stack in this handler (otherwise will be int_db_fault)
int_stack_fault:        ; int 0x0C
        pop eax
        nohandler

ALIGN 8
int_gen_prot:           ; int 0x0D
        pop eax
        nohandler

ALIGN 8
int_pg_fault:           ; int 0x0E
        pop eax
        nohandler

ALIGN 8
int_fpu_err:            ; int 0x10
        nohandler

ALIGN 8
int_align_check:        ; int 0x11
        pop eax
        nohandler

ALIGN 8
int_mach_check:         ; int 0x12
        nohandler

ALIGN 8
int_simd_fp:            ; int 0x13
        nohandler

ALIGN 8
; interrupts 0x09, 0x0F and 0x14 - 0x19 are reserved
; we'll just halt the CPU if it happens but next HW int will resume normal execution
int_reserved:
        hlt

ALIGN 8
int_hw00:               ; int 0x20 - timer
        save
        call timer_handle_irq
        pic_eoi 0
        restore
	iret

ALIGN 8
int_hw01:
        save
        call keybd_handle_irq
        pic_eoi 1
        restore
        iret

ALIGN 8
int_hw02:
        jmp $

ALIGN 8
int_hw03:
        jmp $

ALIGN 8
int_hw04:
        jmp $

ALIGN 8
int_hw05:
        jmp $

ALIGN 8
int_hw06:
        jmp $

ALIGN 8
int_hw07:
        jmp $

ALIGN 8
int_hw08:
        jmp $

ALIGN 8
int_hw09:
        jmp $

ALIGN 8
int_hw10:
        jmp $

ALIGN 8
int_hw11:
        jmp $

ALIGN 8
int_hw12:
        jmp $

ALIGN 8
int_hw13:
        jmp $

ALIGN 8
int_hw14:
        jmp $

ALIGN 8
int_hw15:
        jmp $

[SECTION .data]
; Interrupt Gate
;
;  31                    19      1615     12       08      05      00
; -------------------------------------------------------------------
; |          Offset 16:31          |P|DPL |0 D 1 1 0 | 0 0 0 |      |
; -------------------------------------------------------------------
; |        Segment Selector        |          Offset 0:15           |
; -------------------------------------------------------------------
;  31                               15                            00

idtr:
        dw idtend-1
        dd idt
ALIGN 8
idt:
        TIMES (47 * 8) db 0x0
idtend:

