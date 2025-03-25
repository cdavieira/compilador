void func2(){
  int a = 3;
  printf("%d\n", a);
}
void func1(){
  int a = 2;
  printf("%d\n", a);
  func2();
  printf("%d\n", a);
}
int main(){
  func1();
}
