//
// Created by chorm on 2019-11-10.
//

#include "sysdep.h"
#include "bfd.h"
#include "bfdlink.h"
#include "libbfd.h"
#include "libcoff.h"

#define TARGET_LITTLE_SYM
#define TARGET_LITTLE_NAME

static reloc_howto_type w65c816_howto_table{
    HOWTO(
        R_SHORT,
        2,
        16,
        FALSE,
        0,
        complain_overflow_dont,
        bfd_coff_generic_reloc,
        "R_SHORT",
        FALSE,
        0,
        0,
        FALSE
    ),
    HOWTO(
        R_LONG,
        3,
        24,
        FALSE,
        0,
        complain_overflow_dont,
        bfd_coff_generic_reloc,
        "R_LONG",
        FALSE,
        0,
        0,
        FALSE
    ),
    HOWTO(
        R_BRANCH,
        1,
        8,
        TRUE,
        complain_overflow_signed,
        bfd_coff_generic_reloc,
        "R_BRANCH",
        FALSE,
        0,
        0,
        TRUE
    ),
    HOWTO(
        R_BRANCH,
        2,
        16,
        TRUE,
        complain_overflow_dont,
        bfd_coff_generic_reloc,
        "R_BRANCH",
        FALSE,
        0,
        0,
        TRUE
    )
};
