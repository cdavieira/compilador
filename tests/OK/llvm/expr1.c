//a = 3
//b = 6
//c = 9
//d = 1.500000
//e = 4.000000
//f = 5.500000
//h = 6
//i = 36
//j = 1
//k = 0
int main(){
  int a = 1 + 2; //3
  int b = 3 + a; //6
  int c = a + b; //9
  float d = 1.5; //1.5
  float e = 2.5 + d; //4.0
  float f = e + d; //5.5
  // char *g = "Carlos Daniel\nHello world";
  int h = c - 3; //6
  int i = h * b; //36
  int j = h / b; //1
  int k = h % b; //0
  printf("a = %d\n", a);
  printf("b = %d\n", b);
  printf("c = %d\n", c);
  printf("d = %f\n", d);
  printf("e = %f\n", e);
  printf("f = %f\n", f);
  printf("h = %d\n", h);
  printf("i = %d\n", i);
  printf("j = %d\n", j);
  printf("k = %d\n", k);
  return k;
}
