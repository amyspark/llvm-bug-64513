#ifndef FOO_H
#define FOO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t uint32x4 __attribute__ ((vector_size(16)));
typedef uint8_t  uint8x16 __attribute__ ((vector_size(16)));

typedef struct
{
    uint32_t a, b, c, d;
} smallprng_t;

typedef struct
{
    uint32x4 a, b, c, d;
    smallprng_t p0;
} prng_t;

extern prng_t prng_state_data;
extern prng_t *prng_state;

#pragma omp threadprivate(prng_state_data)
#pragma omp threadprivate(prng_state)

static __inline__ uint32_t
smallprng_rand_r (smallprng_t *x)
{
    uint32_t e = x->a - ((x->b << 27) + (x->b >> (32 - 27)));
    x->a = x->b ^ ((x->c << 17) ^ (x->c >> (32 - 17)));
    x->b = x->c + x->d;
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}

/* Generate 4 bytes (32-bits) of random data */
static __inline__ uint32_t
prng_rand_r (prng_t *x)
{
    return smallprng_rand_r (&x->p0);
}

static inline uint32_t
prng_rand (void)
{
    return prng_rand_r (prng_state);
}

/* Set the 32-bit seed for PRNG */
void prng_srand_r (prng_t *prng, uint32_t seed);

static inline void
prng_srand (uint32_t seed)
{
    if (!prng_state)
    {
        /* Without setting a seed, PRNG does not work properly (is just
         * returning zeros). So we only initialize the pointer here to
         * make sure that 'prng_srand' is always called before any
         * other 'prng_*' function. The wrongdoers violating this order
         * will get a segfault. */
        prng_state = &prng_state_data;
    }
    prng_srand_r (prng_state, seed);
}

typedef union
{
    uint8_t  b[16];
    uint32_t w[4];
    uint8x16 vb;
    uint32x4 vw;
} prng_rand_128_data_t;

/* Generate 16 bytes (128-bits) of random data */
static inline void
prng_rand_128_r (prng_t *x, prng_rand_128_data_t *data)
{
    uint32x4 e = x->a - ((x->b << 27) + (x->b >> (32 - 27)));
    x->a = x->b ^ ((x->c << 17) ^ (x->c >> (32 - 17)));
    x->b = x->c + x->d;
    x->c = x->d + e;
    x->d = e + x->a;
    data->vw = x->d;
}

#ifdef __cplusplus
}
#endif

#endif
