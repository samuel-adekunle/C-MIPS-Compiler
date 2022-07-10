int f(int* p)
{
    p[1] = 100;
    return 10;
}

int drive()
{
    int x1 = 1;
    int x2 = 2;
    int * x = &x2;
    x[1] = 13;
    int * a = x;
    f(a);
    return x[1];
}
