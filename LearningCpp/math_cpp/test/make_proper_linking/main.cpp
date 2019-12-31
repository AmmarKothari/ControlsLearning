#include "classA.h"
#include "classB.cpp"
#include "class_folder/classC.h"
#include "class_folder/classD.cpp"

int main(){
    A a;
    B b;
    C c;
    D d;
    a.print_A();
    b.print_B();
    c.print_C();
    d.print_D();
    return 0;
}
