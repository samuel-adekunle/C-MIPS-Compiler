unsigned int drive()
{
	unsigned a = 1;
	unsigned b = 2;

	while ( a < 16 )
		a = a * b;
	
	return a;
	if ( a != 16 )
	{
		return 1u;
	}

	if ( a > 16 )
	{
		return 3u;
	}
	if ( a == 16 )
	{
		a = a - b;	  // 14

		a = a + 10; // 24

		a = a / 6; // 4
		a = a / b;	 // 2.0
	}

	if ( a >= 2 )
	{
		if( a <= 2 )
		{
			return 12u;
		}
		return 2u;
	}
	return 1u;
}