int *fish(int a);
int drive(){
    int a[2][3];
    a[1][2] = 2;
    a[0][2] = 1;
    a[0][2]++;
    return ++a[0][2];
}