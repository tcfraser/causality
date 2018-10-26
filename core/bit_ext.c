
/**
 * compress_branch_hd, compress_linear_hd are algorithms found in 
 * [Hacker's Delight p.143 7-4 Compress, or Generalized Extract]
 * and the C implementation found here are provided by "harold" at
 * https://stackoverflow.com/a/28283007
 *
 *      3.14 805306368     0.00     0.00  compress_linear_hd
 *     42.60 805306368     0.00     0.00  compress_branch_hd
 */


unsigned compress_branch_hd(unsigned x, unsigned m) {

    unsigned mk, mp, mv, t;
    int i;

    x = x & m;           // Clear irrelevant bits.
    mk = ~m << 1;        // We will count 0's to right.

    for (i = 0; i < 5; i++) {
       mp = mk ^ (mk << 1);             // Parallel prefix.
       mp = mp ^ (mp << 2);
       mp = mp ^ (mp << 4);
       mp = mp ^ (mp << 8);
       mp = mp ^ (mp << 16);
       mv = mp & m;                     // Bits to move.
       m = m ^ mv | (mv >> (1 << i));   // Compress m.
       t = x & mv;
       x = x ^ t | (t >> (1 << i));     // Compress x.
       mk = mk & ~mp;
    }
    return x;
}

unsigned compress_linear_hd(unsigned x, unsigned m) {
   unsigned r, s, b;    // Result, shift, mask bit.

   r = 0;
   s = 0;
   do {
      b = m & 1;
      r = r | ((x & b) << s);
      s = s + b;
      x = x >> 1;
      m = m >> 1;
   } while (m != 0);
   return r;
}
