#pragma once

#include "e4platf/types.h"

namespace e4 {

// primary tag bits
constexpr Word TAG1_TAG_BITS = 2;
constexpr Word TAG1_VALUE_BITS = BITS_PER_WORD - TAG1_TAG_BITS;
constexpr Word TAG1_VALUE_MASK = (~0ULL) << TAG1_TAG_BITS;

enum class PrimaryTag: Word {
    Header = 0,
    Boxed,
    Cons,
    Immediate,
};

// TODO: There's IMMED1 and IMMED2 on E///VM
enum class ImmediateTag: Word {
    Atom = 0,  //(0 << PRIMARY_SIZE) | IMMED1,
    SmallInt = 1,
    ShortPid = 2,
    ShortPort = 4,
    FpRegister = 6,
    Catch = 8,
    XRegister = 10,
    YRegister = 12,
    Special = 14,  // includes nil,noval,rip
};
constexpr Word IMM1_TAG_BITS = 4;
constexpr Word IMM1_VALUE_BITS = BITS_PER_WORD - IMM1_TAG_BITS
                                 - TAG1_TAG_BITS;

static constexpr Word BOXED_TAG_BITS = 4;
static constexpr Word BOXED_VALUE_BITS
        = BITS_PER_WORD - BOXED_TAG_BITS - TAG1_TAG_BITS;


} // ns e4