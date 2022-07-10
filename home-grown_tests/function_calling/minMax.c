int max(int a, int b, int c, int d, int e, int f, int g, int h){
    int largest = a;
    if(b>largest){
        largest = b;
    }
    if(c>largest){
        largest = c;
    }
    if(d>largest){
        largest = d;
    }
    if(e>largest){
        largest = f;
    }
    if(g>largest){
        largest = h;
    }
    if(h>largest){
        largest = h;
    }
    return largest;
}

int drive(int a){
    for(int i = 0; i <= max(1,2,15,2,100,200,23,a); i=i+1){
        if(i == 240){
            return i / 20;
        }
    }
    return 1;
}