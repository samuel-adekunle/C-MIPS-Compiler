int drive(){
    int a[2][2][3][3];
    a[0][1][1][0] = 2;
    a[0][0][0][1] = 4;
    a[1][0][1][1] = a[0][0][0][1] + a[0][1][1][0];
    a[1][0][1][1]++;
    return  a[1][0][1][1];
}