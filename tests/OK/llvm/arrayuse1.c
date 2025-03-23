int main(){
	int a[10];
	int i = 0;
	while(i < 10){
	  a[i] = i;
	  i = i + 1;
	}
	i = 0;
	while(i < 10){
	  printf("a[%d] = %d\n", i, a[i]);
	  i = i + 1;
	}
	return 0;
}
