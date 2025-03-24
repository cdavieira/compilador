int func1(int a, int b, float c){

}

int func2(int a, float b, char c){

}

void func3(int f[]){
  printf("%d\n", f[0]);
}

int main(){
  int a[3];
  a[0] = 3;
  func3(a);


  return 0;
}
