int func1(int a, int b, float c){

}

int func2(int a, float b, char c){

}

void func3(int f[]){
  printf("%d\n", f[0]);
  f[1] = 2;
}

void func4(int f[3]){
  printf("%d\n", f[0]);
  f[1] = 4;
}

int main(){
  int a[3];
  a[0] = 3;
  func3(a);
  printf("%d\n", a[1]);
  func4(a);
  printf("%d\n", a[1]);


  return 0;
}
