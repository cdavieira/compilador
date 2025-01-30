int main(void){
	int counter = 1;
	int val;

	while(counter < 10){
		val = 0;
		while(val != 5){
			val = val + 1;
		}
		counter = counter * 2;
	}

	return 0;
}
