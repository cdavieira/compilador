//a = 1
//b = 0
//c = 0
//d = 1.500000
//e = 1
//f = 1
//g = 0
//h = 0
//i = 1

int main(void){
	int a = 1;
	int b = 0;
	int c = a < b;

	float d = 1.5;
	int e = 2 > d;

	int f = a == 1;
	int g = b != 0;

	int h = f && g;
	int i = f || g;

	printf("a = %d\n", a);
	printf("b = %d\n", b);
	printf("c = %d\n", c);
	printf("d = %f\n", d);
	printf("e = %d\n", e);
	printf("f = %d\n", f);
	printf("g = %d\n", g);
	printf("h = %d\n", h);
	printf("i = %d\n", i);

	return 0;
}
