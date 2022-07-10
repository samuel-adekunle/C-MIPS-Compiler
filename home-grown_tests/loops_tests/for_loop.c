int f()
{
	int a = 0;
	for (int i = 0; i < 2; i = i + 1)
	{
		if (i == 1)
		{
			a = a + 1;
		}
	}
	// two expression statements
	int i;
	for (i = 0; i < 10; )
	{

		i = i + 1;
	}
	a = i + a;

	// two expressions statements one declaration
	for (i = 0; i < 10; i = i + 1)
	{
		a = i + a;
	}
	// declaration and expression statement
	for (int i = 0; i < 10; ) {
		a = a - i;
		i = i + 1;
	}
	// declaration, expression statement, expression
	for (int i = 0; i < 10; i = i + 1) {
		a = a + i;
		if (i == 2) {
			continue;
		}
		else if (i > 2) {
		}
	}
	return a - 44;
}