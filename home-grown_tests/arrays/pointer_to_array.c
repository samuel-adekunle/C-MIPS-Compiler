int drive(){
    int *a[5];
    int b = 10;
    a[0] = &b;
    return *a[0];//a[0][0];
}