/* considerar que os parametros sao apenas unsigned, ok? */
/* minha versao porca do https://en.wikipedia.org/wiki/Euclidean_algorithm */
int mdc(int dividendo, int divisor){
	if(divisor > dividendo){
		return mdc(divisor, dividendo);
	}
	int temp = dividendo % divisor;
	if(temp == 0){
		return divisor;
	}
	return mdc(temp, divisor);
}

int main(void){
	int num1 = 77;
	int num2 = 14;
	int res = mdc(num1, num2);

	return 0;
}
