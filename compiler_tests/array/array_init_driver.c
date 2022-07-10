#include "stdio.h"
int init();
void printIt(int a){
    if(a==-1){
        printf("\n");
        return;
    }
    printf("%d",a);
}
int main(){
    return init();
}