/* TIc80 Simulator.
   Copyright (C) 1997 Free Software Foundation, Inc.
   Contributed by Cygnus Support.

This file is part of GDB, the GNU debugger.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */


#include "sim-main.h"

#ifdef HAVE_STRING_H
#include <string.h>
#else
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#endif


tic80_control_regs
tic80_index2cr (int index)
{
  switch (index)
    {
    case 0x0000: return EPC_CR;
    case 0x0001: return EIP_CR;
    case 0x0002: return CONFIG_CR;
    case 0x0004: return INTPEN_CR;
    case 0x0006: return IE_CR;
    case 0x0008: return FPST_CR;
    case 0x000A: return PPERROR_CR;
    case 0x000D: return PKTREQ_CR;
    case 0x000E: return TCOUNT_CR;
    case 0x000F: return TSCALE_CR;
    case 0x0010: return FLTOP_CR;
    case 0x0011: return FLTADR_CR;
    case 0x0012: return FLTTAG_CR;
    case 0x0013: return FLTDLT_CR;
    case 0x0014: return FLTDTH_CR;
    case 0x0015: return FLT005_CR;
    case 0x0016: return FLT006_CR;
    case 0x0017: return FLT007_CR;
    case 0x0018: return FLT008_CR;
    case 0x0019: return FLT009_CR;
    case 0x001a: return FLT010_CR;
    case 0x001b: return FLT011_CR;
    case 0x001c: return FLT012_CR;
    case 0x001d: return FLT013_CR;
    case 0x001e: return FLT014_CR;
    case 0x001f: return FLT015_CR;
    case 0x0020: return SYSSTK_CR;
    case 0x0021: return SYSTMP_CR;
    case 0x0030: return MPC_CR;
    case 0x0031: return MIP_CR;
    case 0x0033: return ECOMCNTL_CR;
    case 0x0034: return ANASTAT_CR;
    case 0x0039: return BRK1_CR;
    case 0x003A: return BRK2_CR;
    case 0x0200: return ITAG0_CR;
    case 0x0201: return ITAG1_CR;
    case 0x0202: return ITAG2_CR;
    case 0x0203: return ITAG3_CR;
    case 0x0204: return ITAG4_CR;
    case 0x0205: return ITAG5_CR;
    case 0x0206: return ITAG6_CR;
    case 0x0207: return ITAG7_CR;
    case 0x0208: return ITAG8_CR;
    case 0x0209: return ITAG9_CR;
    case 0x020a: return ITAG10_CR;
    case 0x020b: return ITAG11_CR;
    case 0x020c: return ITAG12_CR;
    case 0x020d: return ITAG13_CR;
    case 0x020e: return ITAG14_CR;
    case 0x020f: return ITAG15_CR;
    case 0x0300: return ILRU_CR;
    case 0x0400: return DTAG0_CR;
    case 0x0401: return DTAG1_CR;
    case 0x0402: return DTAG2_CR;
    case 0x0403: return DTAG3_CR;
    case 0x0404: return DTAG4_CR;
    case 0x0405: return DTAG5_CR;
    case 0x0406: return DTAG6_CR;
    case 0x0407: return DTAG7_CR;
    case 0x0408: return DTAG8_CR;
    case 0x0409: return DTAG9_CR;
    case 0x040a: return DTAG10_CR;
    case 0x040b: return DTAG11_CR;
    case 0x040c: return DTAG12_CR;
    case 0x040d: return DTAG13_CR;
    case 0x040e: return DTAG14_CR;
    case 0x040f: return DTAG15_CR;
    case 0x0500: return DLRU_CR;
    case 0x4000: return IN0P_CR;
    case 0x4001: return IN1P_CR;
    case 0x4002: return OUTP_CR;
    default: return SCRATCH_CR;
    }
}



#if defined(WITH_TRACE)
/* Tracing support routines */

static char tic80_trace_buffer[1024];
static int  tic80_size_name;

#define SIZE_HEX	8
#define SIZE_DECIMAL	12

/* Initialize tracing by calculating the maximum name size */
static void
tic80_init_trace (void)
{
  int i;
  int len, max_len = 0;

  for (i = 0; i < (int)nr_itable_entries; i++) {
    len = strlen (itable[i].name);
    if (len > max_len)
      max_len = len;
  }

  tic80_size_name = max_len + sizeof(":m") - 1 + sizeof (":s") - 1;
}

/* Trace the result of an ALU operation with 2 integer inputs and an integer output */
char *
tic80_trace_alu3 (int indx,
		  unsigned32 result,
		  unsigned32 input1,
		  unsigned32 input2)
{
  char buf1[SIZE_DECIMAL+10], buf2[SIZE_DECIMAL+10], bufr[SIZE_DECIMAL+10];

  if (!tic80_size_name)
    tic80_init_trace ();

  sprintf (bufr, "(%ld)", (long) (signed32) result);
  sprintf (buf1, "(%ld)", (long) (signed32) input1);
  sprintf (buf2, "(%ld)", (long) (signed32) input2);

  sprintf (tic80_trace_buffer, "%-*s 0x%.*lx %-*s 0x%.*lx %-*s => 0x%.*lx %-*s",
	   tic80_size_name, itable[indx].name,
	   SIZE_HEX, input1, SIZE_DECIMAL, buf1,
	   SIZE_HEX, input2, SIZE_DECIMAL, buf2,
	   SIZE_HEX, result, SIZE_DECIMAL, bufr);

  return tic80_trace_buffer;
}

/* Trace the result of an ALU operation with 1 integer input and an integer output */
char *
tic80_trace_alu2 (int indx,
		  unsigned32 result,
		  unsigned32 input)
{
  char bufi[SIZE_DECIMAL+10], bufr[SIZE_DECIMAL+10];

  if (!tic80_size_name)
    tic80_init_trace ();

  sprintf (bufr, "(%ld)", (long) (signed32) result);
  sprintf (bufi, "(%ld)", (long) (signed32) input);

  sprintf (tic80_trace_buffer, "%-*s 0x%.*lx %-*s %*s => 0x%.*lx %-*s",
	   tic80_size_name, itable[indx].name,
	   SIZE_HEX, input, SIZE_DECIMAL, bufi,
	   SIZE_HEX + SIZE_DECIMAL + 3, "",
	   SIZE_HEX, result, SIZE_DECIMAL, bufr);

  return tic80_trace_buffer;
}

/* Trace the result of an FPU operation with 2 integer inputs and an integer output */
void
tic80_trace_fpu3 (SIM_DESC sd,
		  sim_cpu *cpu,
		  sim_cia cia,
		  int indx,
		  sim_fpu result,
		  sim_fpu input1,
		  sim_fpu input2)
{
  if (!tic80_size_name)
    tic80_init_trace ();

  trace_one_insn (sd, cpu, cia.ip, 1,
		  itable[indx].file, itable[indx].line_nr, "fpu",
		  "%-*s %*f %*f => %*f",
		  tic80_size_name, itable[indx].name,
		  SIZE_HEX + SIZE_DECIMAL + 3, sim_fpu_2d (input1),
		  SIZE_HEX + SIZE_DECIMAL + 3, sim_fpu_2d (input2),
		  SIZE_HEX + SIZE_DECIMAL + 3, sim_fpu_2d (result));
}

/* Trace the result of an FPU operation with 1 integer input and an integer output */
void
tic80_trace_fpu2 (SIM_DESC sd,
		  sim_cpu *cpu,
		  sim_cia cia,
		  int indx,
		  sim_fpu result,
		  sim_fpu input)
{
  if (!tic80_size_name)
    tic80_init_trace ();

  trace_one_insn (sd, cpu, cia.ip, 1,
		  itable[indx].file, itable[indx].line_nr, "fpu",
		  "%-*s %*f %-*s => %*f",
		  tic80_size_name, itable[indx].name,
		  SIZE_HEX + SIZE_DECIMAL + 3, sim_fpu_2d (input),
		  SIZE_HEX + SIZE_DECIMAL + 3, "",
		  SIZE_HEX + SIZE_DECIMAL, sim_fpu_2d (result));
}

/* Trace the result of an FPU operation with 1 integer input and an integer output */
void
tic80_trace_fpu2i (SIM_DESC sd,
		   sim_cpu *cpu,
		   sim_cia cia,
		   int indx,
		   unsigned32 result,
		   sim_fpu input1,
		   sim_fpu input2)
{
  char bufr[SIZE_DECIMAL+10];

  if (!tic80_size_name)
    tic80_init_trace ();

  sprintf (bufr, "(%ld)", (long) (signed32) result);

  trace_one_insn (sd, cpu, cia.ip, 1,
		  itable[indx].file, itable[indx].line_nr, "fpu",
		  "%-*s %*f %*f => 0x%.*lx %-*s",
		  tic80_size_name, itable[indx].name,
		  SIZE_HEX + SIZE_DECIMAL + 3, sim_fpu_2d (input1),
		  SIZE_HEX + SIZE_DECIMAL + 3, sim_fpu_2d (input2),
		  SIZE_HEX, result, SIZE_DECIMAL, bufr);
}

/* Trace the result of a NOP operation */
char *
tic80_trace_nop (int indx)
{
  if (!tic80_size_name)
    tic80_init_trace ();

  sprintf (tic80_trace_buffer, "%s", itable[indx].name);
  return tic80_trace_buffer;
}

/* Trace the result of a data sink with one input */
char *
tic80_trace_sink1 (int indx, unsigned32 input)
{
  char buf[SIZE_DECIMAL+10];

  if (!tic80_size_name)
    tic80_init_trace ();

  sprintf (buf, "(%ld)", (long) (signed32) input);
  sprintf (tic80_trace_buffer, "%-*s 0x%.*lx %-*s",
	   tic80_size_name, itable[indx].name,
	   SIZE_HEX, input, SIZE_DECIMAL, buf);

  return tic80_trace_buffer;
}

/* Trace the result of a data sink with two inputs */
char *
tic80_trace_sink2 (int indx, unsigned32 input1, unsigned32 input2)
{
  char buf1[SIZE_DECIMAL+10], buf2[SIZE_DECIMAL+10];

  if (!tic80_size_name)
    tic80_init_trace ();

  sprintf (buf1, "(%ld)", (long) (signed32) input1);
  sprintf (buf2, "(%ld)", (long) (signed32) input2);
  sprintf (tic80_trace_buffer, "%-*s 0x%.*lx %-*s 0x%.*lx %-*s",
	   tic80_size_name, itable[indx].name,
	   SIZE_HEX, input1, SIZE_DECIMAL, buf1,
	   SIZE_HEX, input2, SIZE_DECIMAL, buf2);

  return tic80_trace_buffer;
}

/* Trace the result of a data sink with two inputs */
char *
tic80_trace_sink3 (int indx, unsigned32 input1, unsigned32 input2, unsigned32 input3)
{
  char buf1[SIZE_DECIMAL+10], buf2[SIZE_DECIMAL+10], buf3[SIZE_DECIMAL+10];

  if (!tic80_size_name)
    tic80_init_trace ();

  sprintf (buf1, "(%ld)", (long) (signed32) input1);
  sprintf (buf2, "(%ld)", (long) (signed32) input2);
  sprintf (buf3, "(%ld)", (long) (signed32) input3);

  sprintf (tic80_trace_buffer, "%-*s 0x%.*lx %-*s 0x%.*lx %-*s 0x%.*lx %-*s",
	   tic80_size_name, itable[indx].name,
	   SIZE_HEX, input1, SIZE_DECIMAL, buf1,
	   SIZE_HEX, input2, SIZE_DECIMAL, buf2,
	   SIZE_HEX, input3, SIZE_DECIMAL, buf3);

  return tic80_trace_buffer;
}

/* Trace the result of a conditional branch operation */
char *
tic80_trace_cond_br (int indx,
		     int jump_p,
		     unsigned32 cond,
		     unsigned32 target)
{
  char buf[SIZE_DECIMAL+10];

  if (!tic80_size_name)
    tic80_init_trace ();

  sprintf (buf, "(%ld)", (long) (signed32) cond);

  if (jump_p)
    sprintf (tic80_trace_buffer,
	     "%-*s 0x%.*lx %*s 0x%.*lx %-*s => 0x%.*lx",
	     tic80_size_name, itable[indx].name,
	     SIZE_HEX, target, SIZE_DECIMAL, "",
	     SIZE_HEX, cond, SIZE_DECIMAL, buf,
	     SIZE_HEX, target);
  else
    sprintf (tic80_trace_buffer,
	     "%-*s 0x%.*lx %*s 0x%.*lx %-*s => [fallthrough]",
	     tic80_size_name, itable[indx].name,
	     SIZE_HEX, target, SIZE_DECIMAL, "",
	     SIZE_HEX, cond, SIZE_DECIMAL, buf);

  return tic80_trace_buffer;	       
}

/* Trace the result of a unconditional branch operation */
char *
tic80_trace_ucond_br (int indx,
		      unsigned32 target)
{
  if (!tic80_size_name)
    tic80_init_trace ();

  sprintf (tic80_trace_buffer,
	     "%-*s 0x%.*lx %*s => 0x%.*lx",
	     tic80_size_name, itable[indx].name,
	     SIZE_HEX, target, (SIZE_DECIMAL*2) + SIZE_HEX + 4, "",
	     SIZE_HEX, target);

  return tic80_trace_buffer;	       
}

/* Trace the result of a load or store operation with 2 integer addresses
   and an integer output or input */
char *
tic80_trace_ldst (int indx,
		  int st_p,
		  int m_p,
		  int s_p,
		  unsigned32 value,
		  unsigned32 input1,
		  unsigned32 input2)
{
  char buf1[SIZE_DECIMAL+10], buf2[SIZE_DECIMAL+10], bufr[SIZE_DECIMAL+10];
  char name[40];

  if (!tic80_size_name)
    tic80_init_trace ();

  strcpy (name, itable[indx].name);
  if (m_p)
    strcat (name, ":m");

  if (s_p)
    strcat (name, ":s");

  sprintf (bufr, "(%ld)", (long) (signed32) value);
  sprintf (buf1, "(%ld)", (long) (signed32) input1);
  sprintf (buf2, "(%ld)", (long) (signed32) input2);

  sprintf (tic80_trace_buffer, "%-*s 0x%.*lx %-*s 0x%.*lx %-*s %s 0x%.*lx %-*s",
	   tic80_size_name, name,
	   SIZE_HEX, input1, SIZE_DECIMAL, buf1,
	   SIZE_HEX, input2, SIZE_DECIMAL, buf2,
	   (!st_p) ? "=>" : "<=",
	   SIZE_HEX, value, SIZE_DECIMAL, bufr);

  return tic80_trace_buffer;
}

#endif /* WITH_TRACE */
