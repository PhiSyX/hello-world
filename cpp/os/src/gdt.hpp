#ifndef __GDT_H__
#define __GDT_H__

#include "types.hpp"

class GlobalDescriptorTable
{
public:
  class SegmentDescriptor
  {
  private:
    /// `_lo` signifie les bits les moins significatifs.
    u16 limit_lo;
    u16 base_lo;

    /// `_hi` signifie les bits les plus significatifs.
    u8 base_hi;
    u8 type;
    u8 flags_limit_hi;
    u8 base_vhi;

  public:
    SegmentDescriptor(u32 base, u32 limit, u8 type);

    u32 Base() const;
    u32 Limit() const;
  } __attribute__((packed)); // L'attribut 'packed' indique à GCC de ne pas
                             // modifier l'alignement de la structure et de ne
                             // pas effectuer d'optimisations.

  SegmentDescriptor null_segment_selector;
  SegmentDescriptor unused_segment_selector;
  SegmentDescriptor code_segment_selector;
  SegmentDescriptor data_segment_selector;

public:
  GlobalDescriptorTable();
  ~GlobalDescriptorTable();

  u16 CodeSegmentSelector() const;
  u16 DataSegmentSelector() const;
};

#endif
