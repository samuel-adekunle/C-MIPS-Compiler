int f() {
    int a = 2;
    while (a != 15) {
        a = a + 1;
    }
    if (1 == 0) {
        return -1;
    }
    if (a == 2) {
        return 2;
    }
    else if (a == 15) {
        if (a > 2) {
            if (a > 30) {
                return 3;
            }
            else if (a <= 16) {
                if (a >= 15) {
                    int d = -6;
                    d = d << 1;
                    return -d;
                }
            }
            else {
                return 6;
            }

        }
        else {
            return 4;
        }
    }
    else {
        return 5;
    }

}