#include <stdio.h>

int compare_int(int num1, int num2){
	return num1 > num2;
}

void bubble_sort_inplace(int arr[], int size, int (*compare)(int, int)){
	int iterations = 0;
	int i;
	int temp;
	int number_elements;

	while(iterations < size){
		i = 0;
		number_elements = size - iterations;
		while(i < number_elements){
			if(compare(arr[i], arr[i+1])){
				temp = arr[i+1];
				arr[i+1] = arr[i];
				arr[i] = temp;
			}
			i = i + 1;
		}
		iterations = iterations + 1;
	}
}

void init_array(int arr[], int size, int val){
	int i = 0;
	while(i < size){
		arr[i] = size - i;
		i = i + 1;
	}
}

void print_array(int arr[], int size){
	int i = 0;
	while(i < 30){
		printf("a[%d] = %d\n", i, arr[i]);
		i = i + 1;
	}
}

int main(void){
	int a[30];

	/* inicializando array */
	init_array(a, 30, 0);

	/* printando array nao sortido */
	print_array(a, 30);

	/* bubble sort */
	bubble_sort_inplace(a, 30, compare_int);

	/* printando array sortido */
	print_array(a, 30);

	return 0;
}
