#include <stdio.h>

void array_print(int size, int arr[]){
	int i = 0;
	while(i < size){
		printf("arr[%d] = %d\n", i, arr[i]);
		i = i + 1;
	}
}

void array_init(int size, int arr[]){
	int i = 0;
	while(i < size){
		arr[i] = size - i;
		i = i + 1;
	}
}

void bubble_sort(int size, int arr[]){
	int i = 0, j = 0, lim = 0, tmp = 0;
	while(i < size){
		lim = size - i - 1;
		j = 0;
		while(j < lim){
			if(arr[j] > arr[j + 1]){
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j = j + 1;
		}
		i = i + 1;
	}
}

int main(void){
	int sz = 10;
	int arr[10];

	array_init(10, arr);

	printf("Before sorting:\n");

	array_print(10, arr);

	printf("Applying bubble sort...\n");

	bubble_sort(10, arr);

	printf("After sorting:\n");

	array_print(10, arr);

	return 0;
}
