int f() {
    int a = 2;
    int* b = &a;
    int** c = &b;
    **c = 12;
    return a;
}