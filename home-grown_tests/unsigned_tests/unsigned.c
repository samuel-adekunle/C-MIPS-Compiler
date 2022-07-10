int go(){
    unsigned a = 1<<20;
    unsigned b = 1048576u;

    if(a == b){
        unsigned c = 1u << 31;
        unsigned d = 2147483648u;
        if(c==d){
            unsigned res = d >> 31u;
            if(res == 1){

                unsigned z = 10u;
                unsigned y = 9u;
                unsigned res = 11u;
                if(res == (z|y)){
                    return 0;
                }
            }
        }
    }
    return 1;

}