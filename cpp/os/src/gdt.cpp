#include "gdt.hpp"

GlobalDescriptorTable::GlobalDescriptorTable()
  : null_segment_selector(0, 0, 0)
  , unused_segment_selector(0, 0, 0)
  , code_segment_selector(0, 64 * 1024 * 1024, 0x9A)
  , data_segment_selector(0, 64 * 1024 * 1024, 0x92)
{
  u32 i[2];
  i[0] = (u32)this;
  i[1] = sizeof(GlobalDescriptorTable) << 16;
  asm volatile("lgdt (%0)" : : "p"(((u8*)i) + 2));
}

GlobalDescriptorTable::~GlobalDescriptorTable() {}

u16
GlobalDescriptorTable::get_data_segment_selector() const
{
  return (u8*)&data_segment_selector - (u8*)this;
}

u16
GlobalDescriptorTable::get_code_segment_selector() const
{
  return (u8*)&code_segment_selector - (u8*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(u32 base,
                                                            u32 limit,
                                                            u8 flags)
{
  u8* target = (u8*)this;

  if (limit <= 65536) {
    // 16-bit espace d'adressage
    target[6] = 0x40;
  } else {
    // 32-bit espace d'adressage
    // Maintenant, nous devons squeeze la limite (32 bits) en 2.5 (20 bits).
    // Ça se fait en éliminant les 12 bits les moins significatifs, mais ça
    // n'est légal que s'ils sont tous égaux à 1, donc ils sont implicitement
    // toujours là.

    if ((limit & 0xFFF) != 0xFFF) {
      limit = (limit >> 12) - 1;
    } else {
      limit = limit >> 12;
    }

    target[6] = 0xC0;
  }

  // Encode la limite
  target[0] = limit & 0xFF;
  target[1] = (limit >> 8) & 0xFF;
  target[6] |= (limit >> 16) & 0xF;

  // Encode la base
  target[2] = base & 0xFF;
  target[3] = (base >> 8) & 0xFF;
  target[4] = (base >> 16) & 0xFF;
  target[7] = (base >> 24) & 0xFF;

  // Type
  target[5] = flags;
}

u32
GlobalDescriptorTable::SegmentDescriptor::base() const
{
  u8* target = (u8*)this;
  u32 result = target[7];
  result = (result << 8) + target[4];
  result = (result << 8) + target[3];
  result = (result << 8) + target[2];
  return result;
}

u32
GlobalDescriptorTable::SegmentDescriptor::limit() const
{
  u8* target = (u8*)this;
  u32 result = target[6] & 0xF;
  result = (result << 8) + target[1];
  result = (result << 8) + target[0];

  if ((target[6] & 0xC0) == 0xC0) {
    result = (result << 12) | 0xFFF;
  }

  return result;
}
