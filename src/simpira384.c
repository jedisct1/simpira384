#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC target("sse2")
#pragma GCC target("aes")
#endif

#include <immintrin.h>
#include <stdint.h>

#include "simpira384.h"

#define SIMPIRA_F(C, B, X, Z)                                                \
    _mm_aesenc_si128(                                                        \
        _mm_aesenc_si128((X),                                                \
                         _mm_set_epi32(0x00 ^ (C) ^ (B), 0x10 ^ (C) ^ (B),   \
                                       0x20 ^ (C) ^ (B), 0x30 ^ (C) ^ (B))), \
        (Z))

#define SIMPIRA384_ROUND(INV, STATE, Z, R)         \
    STATE[(((INV) ? 20 - (R) : (R)) + 1) % 3] ^=   \
        SIMPIRA_F(((INV) ? 20 - (R) : (R)) + 1, 3, \
                  STATE[((INV) ? 20 - (R) : (R)) % 3], (Z))

#define SIMPIRA384_PERMUTE(INV, STATE)           \
    do {                                         \
        const __m128i z = _mm_setzero_si128();   \
        SIMPIRA384_ROUND((INV), (STATE), z, 0);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 1);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 2);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 3);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 4);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 5);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 6);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 7);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 8);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 9);  \
        SIMPIRA384_ROUND((INV), (STATE), z, 10); \
        SIMPIRA384_ROUND((INV), (STATE), z, 11); \
        SIMPIRA384_ROUND((INV), (STATE), z, 12); \
        SIMPIRA384_ROUND((INV), (STATE), z, 13); \
        SIMPIRA384_ROUND((INV), (STATE), z, 14); \
        SIMPIRA384_ROUND((INV), (STATE), z, 15); \
        SIMPIRA384_ROUND((INV), (STATE), z, 16); \
        SIMPIRA384_ROUND((INV), (STATE), z, 17); \
        SIMPIRA384_ROUND((INV), (STATE), z, 18); \
        SIMPIRA384_ROUND((INV), (STATE), z, 19); \
        SIMPIRA384_ROUND((INV), (STATE), z, 20); \
    } while (0)

static inline void
_simpira384_permute(__m128i state[3])
{
    SIMPIRA384_PERMUTE(0, state);
}

static inline void
_simpira384_permute_inv(__m128i state[3])
{
    SIMPIRA384_PERMUTE(1, state);
}

void
simpira384_permute(uint8_t state_u8[48])
{
    __m128i state[3];

    state[0] = _mm_loadu_si128((const __m128i *) (const void *) &state_u8[0]);
    state[1] = _mm_loadu_si128((const __m128i *) (const void *) &state_u8[16]);
    state[2] = _mm_loadu_si128((const __m128i *) (const void *) &state_u8[32]);
    _simpira384_permute(state);
    _mm_storeu_si128((__m128i *) (void *) &state_u8[0], state[0]);
    _mm_storeu_si128((__m128i *) (void *) &state_u8[16], state[1]);
    _mm_storeu_si128((__m128i *) (void *) &state_u8[32], state[2]);
}

void
simpira384_permute_inv(uint8_t state_u8[48])
{
    __m128i state[3];

    state[0] = _mm_loadu_si128((const __m128i *) (const void *) &state_u8[0]);
    state[1] = _mm_loadu_si128((const __m128i *) (const void *) &state_u8[16]);
    state[2] = _mm_loadu_si128((const __m128i *) (const void *) &state_u8[32]);
    _simpira384_permute_inv(state);
    _mm_storeu_si128((__m128i *) (void *) &state_u8[0], state[0]);
    _mm_storeu_si128((__m128i *) (void *) &state_u8[16], state[1]);
    _mm_storeu_si128((__m128i *) (void *) &state_u8[32], state[2]);
}
