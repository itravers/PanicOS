OUTPUT_FORMAT("binary") /*changed to allow gdb debugging*/
/*OUTPUT_FORMAT("elf32-i386");*/
ENTRY(start)
physz = 0x00100000;
SECTIONS
{
  .text physz : AT(physz) {
    code = .;
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }
  .data : AT(physz + (data - code))
  {
    data = .;
    *(.data)
    . = ALIGN(4096);
  }
  .bss : AT(physz + (bss - code))
  {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
  }
  end = .;
}
