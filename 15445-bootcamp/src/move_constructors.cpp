// 【移动构造函数和移动赋值运算符的教程代码】

// 移动构造函数和移动赋值运算符是类中实现的方法，用于有效地将资源从一个对象转移到另一个对象
// 通常使用 std::move。这些方法接收另一个相同类型的对象，并将其资源移动到调用该方法的实例中。
// 在这个文件中，我们将探讨如何实现和使用移动构造函数和移动赋值运算符。

#include<iostream>
#include<utility>  // utility 头文件以使用 std::move
#include<string>
#include<cstdint> // 包含 uint32_t 的头文件
#include<vector> // 这里使用向量的原因是：向量占用相当可观的内存，从而可以显示 std::move 的性能优势

// 基本的Person 类，实现了移动构造函数和移动赋值运算符
// 并删除了拷贝构造函数和拷贝赋值运算符。这意味着一旦一个 Person 对象被实例化，
// 它就不能被复制，必须从一个左值转移到另一个左值。
// 没有拷贝操作符的类在以下情况下非常有用：当必须只有一个定义的类实例时
// 例如，如果一个类管理动态分配的内存块，则在没有适当处理的情况下创建该类的多个实例
// 可能会导致双重释放或内存泄漏。

// psNote:【这里我认为拷贝构造函数和拷贝赋值运算符相当于浅拷贝】
// 问题在于当多个浅拷贝的对象指向同一块内存区域的时候，可能出现某一个对象已经释放过该内存，另一个对象又重复释放
// 或者当某一个对象已经释放了资源，但新对象不知道这个释放过程时，原先的内存指针没有用
// std::move 的移动构造函数和移动赋值运算符相当于保证了资源的所有权唯一性
// 方便所有权的转移，资源也不会重复创建和释放的内存泄漏问题。
class Person{
public:
    Person() : age_(0), nicknames_({}), valid_(true) {}

    // 注意，此构造函数接收一个 std::vector<std::string> 右值。
    // 这使得构造函数更加高效，因为它不会在构造 Person 对象时深度复制向量实例。
    Person(uint32_t age, std::vector<std::string> &&nicknames)
    :age_(age), nicknames_(std::move(nicknames)), valid_(true) {}

    // Person 类的移动构造函数。它接收一个类型为 Person 的右值，
    // 并将传入的右值的内容移动到此 Person 对象实例中。注意 std::move 的使用。
    // 为了确保 person 对象中的 nicknames 被移动而不是深度复制，我们使用 std::move.
    // std::move 将 person.nicknames_ 左值转换为右值，表示其值本身。
    // 同时注意，我们没有在 age_字段上调用 std::move。
    // 因为它是整数类型，大小太小，不会产生显著的复制成本。
    // 一般来说，对于数值类型，复制是可以接受的，但对于其他类型（如字符串和对象类型），
    // 应该移动类实例，除非确实需要复制。
    Person(Person &&person)
    : age_(person.age_), nicknames_(std::move(person.nicknames_)),
    valid_(true) {
        std::cout << "Calling Person's move constructor. \n";
        // 被调用对象的有效性标签被设置为 false。
        person.valid_ = false;
    }

    // Person 类的移动赋值运算符
    // psNote【解决一个赋值运算符返回 Person& 引用的问题】
    /*
    之前不太了解为什么赋值需要返回的类型是 Person& 而不是 Person
    以及返回 Person& 为什么就支持链式操作。
    1. C++ 的值语义（value semantics）默认行为。当函数返回非引用类型时，会强制生成副本。
    所以当返回值为 Person 时，返回的并不是 a = b = c， 第二个赋值操作之前的 b 本身，而是 b 的一个副本。
    2. a = b = c 的过程中出错原因
    比如第一次产生 b 的副本，之后 a 的值生成情况完全根据 b 的独立副本来。
    这时如果再对原对象 b 进行修改，这个修改不会反应到 a 之上。
    【副本】: 
    副本意味着一个新的Person 对象，其内容与原对象相同，但内存地址不同。
    因此，任何后续的操作都是在这个副本上进行，而原对象的状态不会影响副本，副本也不会影响原对象的状态。
    
    如果只是简单对 Person.age 的部分进行修改，返回 Person& 或者返回 Person 的区别不大，问题出在深拷贝和资源释放的时候
    
    【举例说明赋值操作 operator= 返回值应该是 Person& 而不是 Person】
    class Person{
        int *data;
    public:
        Person operator=(const Person& other){
            delete[] data;
            data = new int[other.data];
            return *this;
        }
    }
    a = b = c
    b 释放原有内存，深拷贝 c 的资源。返回b 的副本时再次触发拷贝构造函数，
    拷贝构造函数再次深拷贝 b 的资源（生成独立副本）
    a 释放原有内存，深拷贝 temp_b 的资源。
    （这个过程中经历了 b, temp_b, a) 资源被复制了两次，性能低
    如果 temp_b 在链式复制后释放资源，浅拷贝的情况下会导致 b 的资源也被释放。

    此外可能出现 a = (b = c).some_method()  对于 some_method() 而言，调用的对象是 b 的副本而不是原来的 b
    */
    Person &operator=(Person &&other){
        std::cout << "Calling Person's move assignment operator.\n";
        age_ = other.age_;
        nicknames_ = std::move(other.nicknames_);
        valid_ = true;

        // 被移动对象的有效性标签被设置为 false.
        other.valid_ = false;
        return *this;
    }

    // 我们删除了拷贝构造函数和拷贝赋值运算符，
    // 因此此类不能被拷贝构造
    Person(const Person&) = delete;
    Person &operator=(const Person&) = delete;

    uint32_t GetAge() {return age_;}

    // 返回类型中的这个 & 表示我们返回对 nicknames_[i] 字符串的引用。
    // 这也意味着我们不会复制结果字符串，这个返回值底层实际上是
    // 指向 nicknames_ 向量内存的地址。
    // [psNote]: 如果返回类型是 std::string, 会调用拷贝构造函数创建一个新的字符串对象，
    // 如果返回类型是 std::string&, 直接返回原对象的引用，避免拷贝操作
    std::string &GetNicknameAtI(size_t i) {return nicknames_[i];}

    void PrintValid(){
        if(valid_){
            std::cout << "Person object valid. " << std::endl;
        }else{
            std::cout << "Person object invalid. " << std::endl;
        }
    }

private:
    uint32_t age_;
    std::vector<std::string> nicknames_;
    bool valid_;        // 跟踪对象的数据是否有效，即是否所有数据都已转移到另一个实例
};

int main(){
    // 看看移动构造函数和移动赋值运算符在类中如何实现和使用
    // 首先，我们创建一个 Person 类的实例。andy 对象是一个有效的对象。
    Person andy(15445, {"andy", "pavlo"});
    std::cout << "Print validity of andy: ";
    andy.PrintValid();

    // 要将 andy 对象的内容移动到另一个对象，我们可以使用 std::move 的几种方式
    // 此方法调用移动赋值运算符。
    Person andy1;
    andy1 = std::move(andy);

    // 注意，andy1 是有效的，而 andy 不再是有效的对象。
    std::cout << "Print validity for andy1: ";
    andy1.PrintValid();
    std::cout << "Print validity for andy: ";
    andy.PrintValid();

    // 此方法调用移动构造函数。在此操作之后，原始 andy 对象的内容已经移动到 andy1，
    // 然后又移动到 andy2. andy 和 andy1 左值实际上已经失效（不应再使用，除非重新初始化）。
    Person andy2(std::move(andy1));

    // 注意，andy2 是有效的，而 andy1 不再是有效的对象。
    std::cout << "Print validity for andy2: ";
    andy2.PrintValid();
    std::cout << "Print validity for andy1: ";
    andy1.PrintValid();

    // 下面的代码编译无法通过，因为已经删除了使用拷贝赋值运算符，以及使用 andy2 的深拷贝内容重新初始化 andy3。
    // 拷贝构造函数也被删除
    // Person andy3;
    // andy3 = andy2;
    // Person andy4(andy3);
    return 0;
}