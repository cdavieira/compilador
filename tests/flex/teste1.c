#include <stdio.h>

float function(int x, float y){
    float result = x*y;
    return result;
}

void main(){
    int i = 10;
    int a = 20;
    int x;
    float y;
    char string[23] = "Este char e uma string";
    // Descomentar a linha abaixo para forçar um erro de sintaxe
    //float f = 2,2;
    float f = 1.1;
    char aaaaa = '\n';

    x = a + i;
    y = a*f+i;

    //comment
    /*  comment line 0
        comment line 1
        comment line 2
        comment line 3  */

    for(int j = 0; j <= 10; j++){
        i = i + 1;
    }

    printf("i=%d \n a=%d \n x=%d \n y=%.1f \n f=%f \n string=%s\n", i, a, x, y, f, string);

    printf("Resultado da multiplicacao eh = %f\n", function(x, y));
    if(function(x,y)>=10){
        printf("maior ou igual a 10");
    }else{
        printf("meenor que 10");
    }
    return;
}