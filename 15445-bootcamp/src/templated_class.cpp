// 模板类教程代码
#include<iostream>

// 模板也可以用于实现类。例如，这是一个基本的模板类，
// 它存储一个模板类型的元素，并在调用 print 函数时打印该元素
template<typename T>
class Foo{
public: 
    Foo(T var): var_(var){}
    void print(){
        std::cout << var_ << std::endl;
    }

private:
    T var_;
};


// 你还可以通过模板将多个类型传递给类
// 例如，这是另一个基本的模板类，它存储两个模板类型的元素，
// 并在调用 print 函数时打印它们。
template<typename T, typename U>
class Foo2{
public:
    Foo2(T var1, U var2):
    var1_(var1), var2_(var2) {}

    void print(){
        std::cout << var1_ << " and " << var2_ << std::endl;
    }
private:
    T var1_;
    U var2_;    
};

// 还可以创建特化的模板类，针对不同的类型执行不同的操作。
// 当类实例化为非 float 类型时，
// 它会打印所存储变量的值；而当类实例化为 float 类型时，
// 它会打印 "hello float" 和存储在 var_ 字段中的变量值
template<typename T>
class FooSpecial{
public:
    FooSpecial(T var): var_(var) {}
    void print(){
        std::cout << var_ << std::endl;
    }
private:
    float var_;
};

// 特化的模板类，针对 float 类型进行了特化
template<>
class FooSpecial<float>{
public:
    FooSpecial(float var): var_(var) {}
    void print(){
        std::cout << "hello float! " << var_ << std::endl;
    }
private:
    float var_;
};

// 模板参数不一定是类型，它们也可以是值!
template<int T>
class Bar{
public:
    Bar() {}
    void print_int(){
        std::cout << "print integer: " << T << std::endl;
    }
};

int main(){
    // 首先，我们从一个模板类构造对象。Foo 类模板使用 int 类型作为模板参数进行实例化
    // 这使得 a 的类型为 class Foo<int> 而不是 Foo a 的 print 函数按预期工作。
    Foo<int> a(3);
    std::cout << "Calling Foo<int> a(3)'s print function:";
    a.print();

    // 模板类也可以推断其参数的类型，但是在不确定类型的情况下，务必谨慎使用此功能
    Foo b(3.4f);
    std::cout << "Calling Foo b(3.4f)'s print function:";
    b.print();

    // 其次，我们从具有多个类型参数的模板类构造对象
    Foo2<int, float> c(3, 3.2f);
    std::cout << "Calling Foo2<int, float> c(3, 3.2f)'s print function:";
    c.print();

    // 当我们分别使用和不使用 float 类型参数实例化 FooSpecial 时会发生什么。
    // 正如预期，当我们从 d 调用 print 时，它对因变量而不是 "hello float".
    // 当我们从 e 调用 print 时，e 是 FooSpecial<float> 类的实例，因此它打印 "hello float!".
    FooSpecial<int> d(5);
    std::cout << "Calling FooSpecial<int> d(5)'s print function:";
    d.print();

    FooSpecial<float> e(4.5);
    std::cout << "Calling FooSpecial<float> e(4.5)'s print function:";
    e.print();

    // 最后，从具有非类型参数的模板类构造对象会发生什么
    Bar<150> f;
    std::cout << "Calling Bar<150> f's print_int function: ";
    f.print_int();

    return 0;
}