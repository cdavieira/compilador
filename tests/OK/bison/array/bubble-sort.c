#include <stdio.h>

int main(void){
	int a[30], i, iterations, number_elements;
	int temp;

	/* inicializando array */
	i = 0;
	while(i < 30){
		a[i] = 30 - i;
		i = i + 1;
	}

	/* printando array nao sortido */
	i = 0;
	while(i < 30){
		printf("a[%d] = %d\n", i, a[i]);
		i = i + 1;
	}

	/* bubble sort */
	iterations = 0;
	while(iterations < 30){
		i = 0;
		number_elements = 30 - iterations;
		while(i < number_elements){
			if(a[i] > a[i+1]){
				temp = a[i+1];
				a[i+1] = a[i];
				a[i] = temp;
			}
			i = i + 1;
		}
		iterations = iterations + 1;
	}

	/* printando array sortido */
	i = 0;
	while(i < 30){
		printf("a[%d] = %d\n", i, a[i]);
		i = i + 1;
	}

	return 0;
}
