#include <stdio.h>

int arr[10];

int main(void){
	int i, j;
	int tmp, lim;

	printf("Before sorting:\n");

	i = 0;
	while(i < 10){
		arr[i] = 10 - i;
		printf("arr[%d] = %d\n", i, arr[i]);
		i = i + 1;
	}

	printf("Applying bubble sort...\n");

	i = 0;
	while(i < 10){
		lim = 9 - i;
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

	printf("After sorting:\n");

	i = 0;
	while(i < 10){
		printf("arr[%d] = %d\n", i, arr[i]);
		i = i + 1;
	}

	return 0;
}
