int add(){
    float a = 1.0;
    a++;
    if(a==2.0){
        char a = 'a';
        a++;
        if(a=='b'){
            unsigned b = 20;
            b++;
            if(b==21u){
                double z = 30.0;
                z++;
                if(z==31.0){
                    float y = 342.0f;
                    y++;
                    if(y==343.0f){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}