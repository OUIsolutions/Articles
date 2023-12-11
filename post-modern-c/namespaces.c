
#include <stdio.h>

int add (int a, int b){
    return a + b;
}
int sub (int a, int b){
    return a - b;
}
int mul (int a, int b){
    return a * b;
}
float div (int a, int b){
    return (float)a / (float)b;
}

typedef struct {
int (*add)(int a, int b);
int (*sub)(int a, int b);
int (*mul)(int a, int b);
float(*div)(int a, int b);

}CalcNamespace;

CalcNamespace CalcNamespace_constructor(){
    CalcNamespace self;
    self.add = add;
    self.sub = sub;
    self.mul = mul;
    self.div = div;
    return self;
}


int main(){
    CalcNamespace calc = CalcNamespace_constructor();
    printf("%d\n", calc.add(1,2));
    printf("%d\n", calc.sub(1,2));
    printf("%d\n", calc.mul(1,2));
    printf("%f\n", calc.div(1,2));
    return 0;
}