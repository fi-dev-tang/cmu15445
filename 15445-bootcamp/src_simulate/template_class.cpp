// 模板类的教程代码
// 这个部分比较简单，相对模板函数来说，模板类也就是成员变量的类型不固定，没有其他特别的地方
#include<iostream>

// 1. 定义模板类，拥有一个模板参数
template<typename T>
// 这个模板类就体现在它的成员类型可以是 T, 任何类型
class Foo{
public:
    Foo(T var): var_(var) {}
    void print(){
        std::cout << "Foo's print: " << var_ << std::endl;
    }

private:
    T var_;
};

// 2. 定义模板类，拥有两个模板参数
template<typename U, typename V>
// 这个模板类就体现在它的成员类型可以是 typename U 和 typename V， 两种类型
class Foo2{
public:
    Foo2(U var1, V var2): var1_(var1), var2_(var2) {}
    void print(){
        std::cout << "Foo2's print: " << var1_ << " and " << var2_ << std::endl;
    }
private:
    U var1_;
    V var2_;
};


// 3. 定义模板类，其中分别专门化 typename 为 float, 和不进行专门化
// 注意构造器和 print 之前必须要加 public, 因为 C++ 类的默认访问级别是 private
template<typename T>
class FooSpecial{
public:
    FooSpecial(T var): var_(var) {}
    void print(){
        std::cout << "FooSpecial's print: " << var_ << std::endl;
    }
private:
    T var_;
};

template<>
class FooSpecial<float>{
public:
    FooSpecial(float var): var_(var) {}
    void print(){
        std::cout << "FooSpecial's print:(float) Hello Float!: " << var_ << std::endl;
    }
private:
    float var_;
};

// 4. 定义一个固定的类型，也用于创建模板类
/*
【fix bug】: 这里试图 fix 一个 bug
问题在于我对于模板类固定一个 typename 的写法和理解是: 把 typename 转换成 int 类型
错误写法：
template<int T>
class Bar{
public:
    Bar(T var): var_(var){}
    void print(){
        std::cout << "Bar's print: Integer: " << var_ << std::endl;
    }
private:
    T var_;
};

现在的问题在于：如果想要让 T 表示类型(int, double), 使用 template<typename T>
如果想要让 T 表示具体的值, template<int T>, 要注意 T 是一个值，不能直接用来定义变量类型
*/
// 模板参数不一定是类型，它们也可以是值！
template<int T>
class Bar{
public:
    Bar() {}
    void print(){
        std::cout << "Bar's value is integer:" << T << std::endl;
    }
};

int main(){
    // 1. 调用第一个模板类
    Foo<int> a(3);
    a.print();
    Foo b(3.2f);
    b.print();

    // 2. 调用第二个模板类
    Foo2<double, int> c(2.5, 4);
    c.print();

    // 3. 调用第三个模板类，分别对照有 float 的版本和没有 float 的版本
    FooSpecial<int> d(5);
    d.print();
    FooSpecial<float> e(3.4f);
    e.print();
    

    // 4. 调用第四个模板类
    Bar<2> f;
    f.print();
    return 0;
}