int f() {
    int a = 1;
    int b = 2;
    int c = a == b;
    while (c) {
        return c;
    }
    return 12;
}