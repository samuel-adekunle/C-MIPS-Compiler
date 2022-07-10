int f() {
    int a = 4096;
    int b = 0;
    while (a != 2) {
        a = a >> 1;
        b = b + 1;
    }
    while (b >= 11) {
        int c = -1;
        c = c << 1;
        c = -c;
        if (c == 2) {
            return 12;
        }
        return 2;
    }
    return b;
}