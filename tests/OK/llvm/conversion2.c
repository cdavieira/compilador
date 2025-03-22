#include <stdio.h>

int main(void) {
	int a = 'a';
	printf("%d\n", a);

	a = 1.0;
	printf("%d\n", a);

	float b = 'b';
	printf("%f\n", b);

	b = 1;
	printf("%f\n", b);

	char c = 97;
	printf("%c\n", c);

	c = 10.0;
	printf("%c|||\n", c);

	return a + b + c;
}
