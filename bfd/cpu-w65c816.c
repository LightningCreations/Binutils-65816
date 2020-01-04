

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"

extern const bfd_arch_info_type bfd_w65c816_arch;

const bfd_arch_info_type bfd_w65c816_arch = {
        16,
        24,
        8,
        bfd_arch_w65c816,
        bfd_mach_w65c816,
        "w65c816",
        "65816",
        TRUE,
        bfd_default_compatible,
        bfd_default_scan,
        bfd_arch_default_fill,
        0,
};
