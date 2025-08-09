/* ----------------------------------------------------------------------- *
 *
 *   Copyright 1996-2024 The NASM Authors - All Rights Reserved
 *   See the file AUTHORS included with the NASM distribution for
 *   the specific copyright holders.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *     NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------- */

/*
 * assemble.h - header file for stuff private to the assembler
 */

#ifndef NASM_ASSEMBLE_H
#define NASM_ASSEMBLE_H

#include "nasm.h"
#include "iflag.h"

extern iflag_t cpu, cmd_cpu;
void set_cpu(const char *cpuspec);

extern bool in_absolute;        /* Are we in an absolute segment? */
extern struct location absolute;

int64_t increment_offset(int64_t delta);
void process_insn(insn *instruction);

bool process_directives(char *);
void process_pragma(char *);

void asm_revalidate_cpu(void);

/* Is this a compile-time absolute constant? */
static inline bool op_compile_abs(const struct operand * const op)
{
    if (op->opflags & OPFLAG_UNKNOWN)
        return true;            /* Be optimistic in pass 1 */
    if (op->opflags & OPFLAG_RELATIVE)
        return false;
    if (op->wrt != NO_SEG)
        return false;

    return op->segment == NO_SEG;
}

/* Is this a compile-time relative constant? */
static inline bool op_compile_rel(const insn * const ins,
                                  const struct operand * const op)
{
    if (op->opflags & OPFLAG_UNKNOWN)
        return true;            /* Be optimistic in pass 1 */
    if (!(op->opflags & OPFLAG_RELATIVE))
        return false;
    if (op->wrt != NO_SEG)      /* Is this correct?! */
        return false;

    return op->segment == ins->loc.segment;
}

#endif
