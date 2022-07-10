int f() {
    int a = 12;
    int* b = &a;
    int** c = &b;
    int*** d = &c;
    int** e = *d;
    int* f;
    int not_used = 34234324;
    f = &not_used;
    f = *e;
    return *f;
}