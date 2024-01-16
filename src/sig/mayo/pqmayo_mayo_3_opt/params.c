// SPDX-License-Identifier: Apache-2.0

#include <mayo.h>

#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (M_MAX == 64))
static const unsigned char f_tail_64[] = F_TAIL_64;
#endif
#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (M_MAX == 96))
static const unsigned char f_tail_96[] = F_TAIL_96;
#endif
#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (M_MAX == 128))
static const unsigned char f_tail_128[] = F_TAIL_128;
#endif

#define MAYO_GEN_PARAMS(nm) \
  const mayo_params_t nm = { \
    .m = PARAM_JOIN2(nm, m), \
    .n = PARAM_JOIN2(nm, n), \
    .o = PARAM_JOIN2(nm, o), \
    .k = PARAM_JOIN2(nm, k), \
    .q = PARAM_JOIN2(nm, q), \
    .f_tail = PARAM_JOIN2(nm, f_tail_arr), \
    .m_bytes = PARAM_JOIN2(nm, m_bytes), \
    .O_bytes = PARAM_JOIN2(nm, O_bytes), \
    .v_bytes = PARAM_JOIN2(nm, v_bytes), \
    .r_bytes = PARAM_JOIN2(nm, r_bytes), \
    .P1_bytes = PARAM_JOIN2(nm, P1_bytes), \
    .P2_bytes = PARAM_JOIN2(nm, P2_bytes), \
    .P3_bytes = PARAM_JOIN2(nm, P3_bytes), \
    .csk_bytes = PARAM_JOIN2(nm, csk_bytes), \
    .esk_bytes = PARAM_JOIN2(nm, esk_bytes), \
    .cpk_bytes = PARAM_JOIN2(nm, cpk_bytes), \
    .epk_bytes = PARAM_JOIN2(nm, epk_bytes), \
    .sig_bytes = PARAM_JOIN2(nm, sig_bytes), \
    .salt_bytes = PARAM_JOIN2(nm, salt_bytes), \
    .sk_seed_bytes = PARAM_JOIN2(nm, sk_seed_bytes), \
    .digest_bytes = PARAM_JOIN2(nm, digest_bytes), \
    .pk_seed_bytes = PARAM_JOIN2(nm, pk_seed_bytes), \
    .name = #nm \
  };

#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (N_MAX == 66))
MAYO_GEN_PARAMS(MAYO_1);
#endif
#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (N_MAX == 78))
MAYO_GEN_PARAMS(MAYO_2);
#endif
#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (N_MAX == 99))
MAYO_GEN_PARAMS(MAYO_3);
#endif
#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (N_MAX == 133))
MAYO_GEN_PARAMS(MAYO_5);
#endif
