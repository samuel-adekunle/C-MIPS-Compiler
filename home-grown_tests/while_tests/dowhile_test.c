int f() {
	int a = 10;
	int b = 7;

	do
	{
		b = b + 1;
		a = a - 1;
	} while (a > 5);

	return b;
}