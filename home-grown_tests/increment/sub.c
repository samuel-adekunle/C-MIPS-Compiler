int sub(){
    float a = 1.0;
    a--;
    if(a==0.0){
        char a = 'z';
        a--;
        if(a=='y'){
            unsigned b = 20;
            b--;
            if(b==19u){
                double z = 30.0;
                z--;
                if(z==29.0){
                    float y = 342.0f;
                    --y;
                    if(y==341.0f){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}