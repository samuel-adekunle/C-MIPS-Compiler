void printIt(int a);
int init(){
    int a[2][3][2] = {{{1,2},{3,4},{8,9}},{{1,2},{3,-7},{8,9}}};
    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 2; x++){
            printIt(a[0][y][x]);
        }
        printIt(-1);
    }
    printIt(-1);
    printIt(-1);
    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 2; x++){
            printIt(a[1][y][x]);
        }
        printIt(-1);
    }
    return 1;
}