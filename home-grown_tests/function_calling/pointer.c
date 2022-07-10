int fish(int * a){
    return a[1];
}
int ptr(){
    int a = 10;
    int b = 20;
    return fish(&b);
}