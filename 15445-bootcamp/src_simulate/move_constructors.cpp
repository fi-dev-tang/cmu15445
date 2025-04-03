/*
对移动赋值运算符和移动构造函数的模拟。
这里的 std::move 是 C++ 11 引入的新特性，目的是转移资源而不是深拷贝
对于需要拥有唯一所有权的对象非常重要。
相对应的是拷贝构造函数和拷贝赋值运算符，这中间会出现内存泄漏或者双重释放的问题。
在许多类的构造中，可以使用移动构造函数和移动赋值运算符 来取代 拷贝构造函数和拷贝赋值运算符。
(1） Person p1; Person p2 = std::move(p1)  取代 p2 = p1;
(2)  Person p1; Person p3(std::move(p1))   取代 Person p3(p1);

注意一个问题：赋值运算符 operator= 的返回值类型应该是 Type &
也就是返回原来类型的引用，返回原来类型的问题：
C++ 的值语义（value semantics）返回的是副本，导致原变量的修改不能反映在副本上。
（1）a = b = c 这种链式赋值，对于深拷贝的类型来说，存在 b,b_temp 的两次复制
（2）a_temp 返回对象并销毁内存的时候，浅拷贝会销毁 a 也指向的内存。
*/
#include<iostream>
#include<utility>
#include<cstdint>
#include<vector>

class Person{
public:
    // 1. 默认构造函数，采用初始化 列表表达式
    Person(): age_(0), nicknames_({}), valid_(true) {}
    // 2. 一种采用 std::move 来进行 nicknames_ 资源转移的构造函数
    Person(uint32_t age, std::vector<std::string>&& nicknames): age_(age), nicknames_(std::move(nicknames)),valid_(true) {}
    // 3. 移动构造函数
    Person(Person&& other): age_(other.age_), nicknames_(std::move(other.nicknames_)), valid_(true) {
        std::cout<< "Calling move constructors!" << std::endl;
        other.valid_ = false; // 已经发生了资源转移
    }
    // 4. 移动赋值运算符
    Person& operator=(Person&& other){
        age_ = other.age_;
        nicknames_ = std::move(other.nicknames_);
        std::cout << "Calling Move operator=!" << std::endl;
        other.valid_ = false;
        return *this;
    }

    // 5. 禁用拷贝构造函数
    Person(const Person&) = delete;
    // 6. 禁用拷贝赋值运算符
    Person& operator=(const Person&) = delete;

    // 7. 普通的 GetAge 获取 private 对象
    uint32_t GetAge(){ return age_;}
    // 8. 普通的 获取 nicknames_ 第 i 个成员的原始引用
    std::string& GetNicknamesAtI(int i){return nicknames_[i];}
    // 9. 打印当前成员是否拥有资源
    void PrintValid(){
        if(valid_){
            std::cout << "Member valid!" << std::endl;
        }else{
            std::cout << "Invalid" << std::endl;
        }
    }

private:
    uint32_t age_;
    std::vector<std::string> nicknames_;
    bool valid_;            // 判断移动前后，实例是否还持有资源。std::move 的语义是将资源从一个左值移动到另一个左值
};

int main(){
    Person andy(15455, {"Andy", "Brown"});
    Person andy1;
    andy1 = std::move(andy);

    std::cout << "Andy's validity: ";
    andy.PrintValid();
    std::cout << "Andy1's validity: ";
    andy1.PrintValid();

    Person andy2(15456, {"Anderson", "Parker"});
    Person andy3(std::move(andy2));
    
    std::cout << "Andy2's validity: ";
    andy2.PrintValid();
    std::cout << "Andy3's validity: ";
    andy3.PrintValid();

    // Person tom(11, {"tom", "jerry"});
    // Person tom1 = tom;
    // Person tom2(tom1);
    return 0;
}

/*
【psNote】我第一次的执行结果非常奇怪：
Calling move constructors!
Andy's validity!
Invalid
Andy1's validity!
Invalid
Calling move constructors!
Calling move constructors!
Andy2's validity!
Invalid
Andy3's validity!
Invalid

现象是完全没有看到对于 move 赋值语句的调用，并且所有的 valid_ 都显示 Invalid。
期望的现象是既可以看到移动构造函数的调用，也可以看到移动赋值运算符的调用，
valid 和invalid 应该是交错

[fix bug 1]:Invalid 没有交错的原因是，最初只在默认构造函数里加了 valid_(true)
之后几个构造方式都使用的是移动构造函数，valid_ 还没有赋初始值
[fix bug 2]: 不是说直接 Person p1 = std::move(p2); 就是移动赋值语句
这里如果 p1 对象在之前从未被创建过，那么编译器会把这里的 std::move(p2) 理解成移动构造函数 而是移动赋值运算符。
*/