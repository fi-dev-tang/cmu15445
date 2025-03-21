/*
Q1. 引用和指针的区别？
引用是别名，声明时必须初始化，不可以改变指向。
一个安全不可变的指针。
引用本身是变量的别名，不占用独立内存。但指针作为独立变量存储地址值，占用4-8字节内存。

Q2. 在传递值给带有引用参数的函数时，为什么不需要显式地传递 & 符号？
比如这样的函数 void add_three(int& a){} int x = 3;  add_three(x);
这里编译器会自动处理绑定操作，把 x 的地址绑定到函数参数（别名）a 上，本身的意图是想让用户像操作变量一样操作。
*/
#include<iostream>

void add_three(int &a){
    a = a + 3;
}

int main(){
    int a = 10;
    int& b = a;

    std::cout << "b is " << b << std::endl;
    
    add_three(a);
    std::cout << "a is " << a << std::endl;
    return 0;
}