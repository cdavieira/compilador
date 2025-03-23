int a;

void func1(){
  a = 2;
  float b = 2;
  printf("func1: a=%d\n", a);
  printf("func1: b=%f\n", b);
}

void func2(char d){
  float b = 2;
  float c = 3;
  printf("func2: a=%d\n", a);
  printf("func2: b=%f\n", b);
  printf("func2: c=%f\n", c);
  printf("func2: d=%c\n", d);
}

int main(){
  a = 1;
  float b = 1.5;
  printf("main: a=%d\n", a);
  printf("main: b=%f\n", b);
  return 0;
}
