int f() {
	int a = 1;
	switch (a);
	switch (a) {}
	switch (a) {
	case 1:
		a = a + 1;
	case 2:
		a = a + 1;
	case 3:
		break;
	case 4:
		return -1;
	}

	switch (a) {
	case 1:
	case 2:
		return -2;
	case 4:
		return -3;
	default:
		return 12;
	}
}