int f() {
	int a = 1;
	int b = 2;
	int c = a == b ? 1 : 0;
	if (c) {
		return -1;
	}
	return 12;
}