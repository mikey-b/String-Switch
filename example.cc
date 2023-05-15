#include<cstdio>

enum STATES: unsigned char {
    INIT,
    b, ba, be, c, ca, cad, bed, bad,
    X,
    cad_accept, bed_accept, bad_accept,
};

constexpr STATES table[7][9] = {
    { X, ba, X, X, ca, X, X, X, X},                         // a
    { b, X, X, X, X, X, X, X, X},                           // b
    { c, X, X, X, X, X, X, X, X},                           // c
    { X, X, bad, bed, X, cad, X, X, X},                     // d
    { X, be, X, X, X, X, X, X, X},                          // e
    { X, X, X, X, X, X,cad_accept,bed_accept,bad_accept},   // NUL
    { X, X, X, X, X, X, X, X, X}                            // Anything else
};

constexpr int charclass(char ic) {
    // Limit the table down to the characters you actually need. With one 'other value (5 in this example)
    if (ic == 0) return 5;
    if ((ic > 'e') || (ic < 'a')) return 6;
    return (ic - 'a');
}

constexpr int index(const char* s) {
    auto pos = INIT;
    do { pos = table[charclass(*s++)][pos]; } while(pos < X);
    return pos - X; // Lets shift the output to be 0,1,2,3
}

int main(int argc, char** argv) {
    //** constexpr works fine, use this i to see everything comptime away. **
    // const char *const i = "bed";

    // Or use argv
    if (argc < 2) return 0;
    const char* i = argv[1];

    //** switch does still have an if-chain, testing for 0,1,2,3. **
    
    switch(index(i)) {
        case index("cad"): { printf("found cad\n"); break; }
        case index("bed"): { printf("found bed\n"); break; }
        case index("bad"): { printf("found bad\n"); break; }
        default: { printf("Unknown\n"); break; }
    }
    

    //** Alternatively, a jump table will now work. **
    /*
    static const void* jmptbl[] = { &&Ldefault, &&Lcad, &&Lbed, &&Lbad };
    goto* jmptbl[index(i)];

    Lcad: { printf("found cad\n"); goto Ldone; }
    Lbed: { printf("found bed\n"); goto Ldone; }
    Lbad: { printf("found bad\n"); goto Ldone; }
    Ldefault: { printf("Unknown\n"); goto Ldone; }
    Ldone: {}
    */
}