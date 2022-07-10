double drive()
{
	double a = 1.0;
	double b = 2.0;

	while ( a < 16.0 )
		a = a * b;
	if ( a != 16.0 )
	{
		return -1.0;
	}

	if ( a > 16.0 )
	{
		return -3.0;
	}
	if ( a == 16.0 )
	{
		a = a - b;	  // 14

		a = a + 10.0; // 24

		a = a / 6.0; // 4
		a = a / b;	 // 2.0
	}

	if ( a >= 2.0 )
	{
		if( a <= 2.0 )
		{
			return 12.0;
		}
		return -2.0;
	}
}