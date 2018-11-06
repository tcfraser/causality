/* --- BV macro's --- */
#define BP_1 "%c%c%c%c%c%c%c%c"
#define BV_1(i) \
    (((i) & 0x80ll) ? '1' : '0'), \
    (((i) & 0x40ll) ? '1' : '0'), \
    (((i) & 0x20ll) ? '1' : '0'), \
    (((i) & 0x10ll) ? '1' : '0'), \
    (((i) & 0x08ll) ? '1' : '0'), \
    (((i) & 0x04ll) ? '1' : '0'), \
    (((i) & 0x02ll) ? '1' : '0'), \
    (((i) & 0x01ll) ? '1' : '0')

#define BP_2 BP_1 " " BP_1
#define BP_3 BP_2 " " BP_1
#define BP_4 BP_3 " " BP_1
#define BP_5 BP_4 " " BP_1
#define BP_6 BP_5 " " BP_1
#define BP_7 BP_6 " " BP_1
#define BP_8 BP_7 " " BP_1

#define BV_2(i) BV_1((i) >> 8), BV_1(i)
#define BV_3(i) BV_2((i) >> 8), BV_1(i)
#define BV_4(i) BV_3((i) >> 8), BV_1(i)
#define BV_5(i) BV_4((i) >> 8), BV_1(i)
#define BV_6(i) BV_5((i) >> 8), BV_1(i)
#define BV_7(i) BV_6((i) >> 8), BV_1(i)
#define BV_8(i) BV_7((i) >> 8), BV_1(i)
/* --- end macros --- */
