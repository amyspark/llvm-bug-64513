#include "io.h"
#include <stdlib.h>

/* Random number generator state
 */

prng_t prng_state_data = {0};
prng_t *prng_state = NULL;

void smallprng_srand_r (smallprng_t *x, uint32_t seed)
{
    uint32_t i;
    x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
    for (i = 0; i < 20; ++i)
        smallprng_rand_r (x);
}

void prng_srand_r (prng_t *x, uint32_t seed)
{
    int i;
    prng_rand_128_data_t dummy;
    smallprng_srand_r (&x->p0, seed);
    x->a[0] = x->a[1] = x->a[2] = x->a[3] = 0xf1ea5eed;
    x->b[0] = x->c[0] = x->d[0] = (seed = seed * 1103515245 + 12345);
    x->b[1] = x->c[1] = x->d[1] = (seed = seed * 1103515245 + 12345);
    x->b[2] = x->c[2] = x->d[2] = (seed = seed * 1103515245 + 12345);
    x->b[3] = x->c[3] = x->d[3] = (seed = seed * 1103515245 + 12345);
    for (i = 0; i < 20; ++i)
        prng_rand_128_r (x, &dummy);
}
