// 模板函数的教程代码

#include<iostream>

// 模板是 C++ 中的一种语言特性，允许你编写可以处理多种数据类型的代码，
// 而无需明确指定这些类型。在 C++ 中，你可以创建模板函数和模板类。
// 在本文件中，我们将讨论模板函数。

// 这是一个基本的模板函数，用于将两个数字相加。
// 语法说明，你会看到使用 template<class T> 和 template<typename T> 的代码。
// 尽管这两种声明是等价的，但 class 和 typename 关键字之间存在一些差异。
// 不需要为这门课程了解这些细节；
template <typename T>
T add(T a, T b){
    return a + b;
}

// 可以通过模板向函数传递多个类型名称
// 此函数将打印这两个值
template<typename T, typename U>
void print_two_values(T a, U b){
    std::cout << a << " and " << b << std::endl;
}

// 还可以创建专门化的模板函数，它们对不同的类型执行不同的操作。
// 以下是一个人为的例子，当传入 float 类型时会打印该类型
// 而对于所有其他类型则打印 "Hello World"
template <typename T>
void print_msg(){
    std::cout << "Hello world!\n";
}

// 针对 float 类型的专门化模板函数。
template<>
void print_msg<float>(){
    std::cout << "print_msg is called, the type is float!\n";
}

// 最后，模板参数不一定是类。以下是一个非常简单（但人为设计）的函数，
// 它接受一个布尔值作为模板参数，并根据布尔值对参数执行不同的操作。

/*
【psNote：这里解释一下对于特定的模板参数 template<bool T> 和普通的带有两个参数的函数的区别
比如下面的函数实际上也可以写成 
template<bool T>
int add3(bool t, int a){ if(t){return a + 3;} else {return a;}}

这里在于模板参数 T 是编译时常量，所以 if(T) 的分支会在编译时被优化掉。
生成时的代码：编译器会为每个不同的 T 值生成独立的函数实例。
int add3_true(int a){return a + 3;}

对于模板版本来说，没有运行时开销，因为 if(T) 在编译时就被优化掉了，运行时不会有任何条件判断。
对于频繁调用的场景，模板版本可以避免运行时的分支预测失败或额外的指针执行。
非模板版本情况下，参数 t 是一个运行时变量，无论 t 的值是什么，都只会生成一个通用的函数 ，包含完整的 if 分支逻辑。
*/
template<bool T>
int add3(int a){
    if(T){
        return a + 3;
    }
    return a;
}

int main(){
    // add 函数在 int 和 float 类型上的调用
    std::cout << "print add<int>(3, 5): " << add<int>(3, 5) << std::endl;
    std::cout << "print add<float>(2.8, 3.7): " << add<float>(2.8, 3.7) << std::endl;

    // 模板函数也可以推断其参数的类型，不建议这样做，因为可能无法确定传递给函数的类型
    std::cout << "print add(3, 5): " << add(3, 5) << std::endl;

    // 其次，来看 print_two_values 函数被调用时传入两种不同类型的参数。
    std::cout << "print print_two_values<int, float>(3, 3.2): ";
    print_two_values<int, float>(3, 3.2);

    // 让我们看看在传入和不传入 float 类型时调用 print_msg 的结果
    // 第一次调用 print_msg 打印通用输出
    // 第二次调用（传入 float 参数）识别了类型参数并调用了专门化的函数。
    std::cout << "Calling print_msg<<int>(): ";
    print_msg<int>();
    std::cout << "Calling print_msg<float>(): ";
    print_msg<float>();

    // add3 对于 true 和 false 模板参数都有指定的行为，比如
    std::cout << "print add3<true>(3): " << add3<true>(3) << std::endl;
    std::cout << "print add3<false>(3): " << add3<false>(3) << std::endl;

    return 0;
}