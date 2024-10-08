// Memory segment companion functions

/*
  Author: Martin Eden
  Last mod.: 2024-10-05
*/

#include "me_MemorySegment.h"

#include <me_BaseTypes.h>

#include <stdio.h> // fwrite() for Print()
#include <string.h> // strlen() for FromAsciiz()
#include <me_Console.h> // <Console> for PrintWrappings()

using
  me_MemorySegment::TMemorySegment;

/*
  Cast ASCII structure to memory segment

  What is called "string" in C is a pointer to zero-terminated
  sequence of bytes.

  This function describes span of that sequence.

  Zero byte is not counted.
*/
TMemorySegment me_MemorySegment::Freetown::FromAsciiz(
  TChar * Asciiz
)
{
  TMemorySegment Result;

  Result.Start.Addr = (TUint_2) Asciiz;
  Result.Size = strlen(Asciiz);

  return Result;
}

/*
  Wrapper for typical use case: FromAsciiz("Literal")
*/
TMemorySegment me_MemorySegment::Freetown::FromAsciiz(
  const TChar * Asciiz
)
{
  return FromAsciiz((TChar *) Asciiz);
}

/*
  Return true if segments intersect

  Empty segment don't intersect anything.
*/
TBool me_MemorySegment::Freetown::Intersects(
  TMemorySegment A,
  TMemorySegment B
)
{
  // Empty segment don't intersect anything. Even same empty segment.
  if ((A.Size == 0) || (B.Size == 0))
    return false;

  TUint_2 A_Start = A.Start.Addr;
  TUint_2 B_Start = B.Start.Addr;

  if (A_Start < B_Start)
  {
    TUint_2 A_Stop = A_Start + A.Size - 1;
    if (A_Stop >= B_Start)
      return true;
  }
  else // if (A_Start => B_Start)
  {
    TUint_2 B_Stop = B_Start + B.Size - 1;
    if (B_Stop >= A_Start)
      return true;
  }

  return false;
}

/*
  Compare for equality

  If trivial check (equal spans) is passed, we'll compare data.

  Segments may intersect, we don't care.
*/
TBool me_MemorySegment::Freetown::AreEqual(
  TMemorySegment A,
  TMemorySegment B
)
{
  // No equality for different sizes
  if (A.Size != B.Size)
    return false;

  // Equality for same span
  if (A.Start.Addr == B.Start.Addr)
    return true;

  // Data comparison
  for (TUint_2 Offset = 0; Offset < A.Size; ++Offset)
    if (A.Bytes[Offset] != B.Bytes[Offset])
      return false;

  return true;
}

/*
  Print uncooked contents of memory segment to stdout
*/
void me_MemorySegment::Freetown::Print(
  TMemorySegment MemSeg
)
{
  // First implementation was in [me_ParseInteger] demo
  fwrite(MemSeg.Bytes, MemSeg.Size, 1, stdout);
}

/*
  [Debug] Print state and data to stdout
*/
void me_MemorySegment::Freetown::PrintWrappings(
  TMemorySegment MemSeg
)
{
  Console.Print("TMemorySegment");
  Console.Print("(");
  Console.Indent();

  Console.Write("Start ");
  Console.Print(MemSeg.Start.Addr);
  Console.Newline();

  Console.Write("Size ");
  Console.Print(MemSeg.Size);
  Console.Newline();

  Console.Write("Data ( ");

  for (TUint_2 Offset = 0; Offset < MemSeg.Size; ++Offset)
    Console.Print(MemSeg.Bytes[Offset]);

  Console.Write(" ) ");

  Console.Unindent();
  Console.Print(")");
}

/*
  2024-05-23 GetByte
  2024-05-25 PrintWrappings, PrintMem, CopyMemTo, Spawn, Kill
  2024-05-27 CopyMemFrom
  2024-05-30 CloneFrom
  2024-06-07 IsEqualTo
  2024-10-05 [+] Freetown. Memory-changing functions moved to their Freetown
*/