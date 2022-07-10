int getIt(int ** a){
    return **a;
}
int f() {
    int a = 12;
    int** b;
    int * f = &a;
    b = &f;
    return getIt(b);
}
