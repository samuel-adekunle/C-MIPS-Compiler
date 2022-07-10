float drive() {
	float a = 1.0f;
	float b = 2.0f;

	while(a < 16.0f) a = a * b;
	if (a != 16.0f) {
		return -1.0f;
	}

	if (a > 16.0f) {
		return -3.0f;
	}

	if (a == 16.0f) {
		a = a - b; // 14
		a = a + 10.0f; // 24

		a = a / 6.0f; // 4
		a = a / b; // 2.0
	}
	if (a >= 2.0f) {
		if (a <= 2.0f) {
			return 12.0f;
		}
		return -2.0f;
	}
}