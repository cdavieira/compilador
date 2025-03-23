int func1(){
	int a[10];
	int i = 5;
	while(i < 10){
	  a[i] = i;
	  i = i + 1;
	}
	i = 5;
	while(i < 10){
	  printf("func1: a[%d] = %d\n", i, a[i]);
	  i = i + 1;
	}
	return 0;
}

int main(){
	int a[10];
	int i = 0;
	while(i < 10){
	  a[i] = i;
	  i = i + 1;
	}
	i = 0;
	while(i < 10){
	  printf("main: a[%d] = %d\n", i, a[i]);
	  i = i + 1;
	}
	func1();
	return 0;
}
