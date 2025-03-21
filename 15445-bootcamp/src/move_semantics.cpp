/*
C++ 中的移动语义是一个非常有用的概念，它允许在对象之间高效地转移数据的所有权。
移动语义的主要目标之一是提高性能，因为移动一个对象比深拷贝该对象更快、更高效。

要理解移动语义，必须先了解左值（lvalue）和右值（rvalue）的概念。
左值的简化定义是：它们是指向内存中某个位置的对象。
右值则是任何不是左值的东西。

std::move 是将一个对象从一个左值移动到另一个左值的最常见方法。
std::move 将一个表达式转换为右值。这使得我们可以将左值当作右值来操作。
并允许所有权从一个左值转移到另一个左值。

下面的代码中，包含了一些示例，用于识别C++表达式是左值还是右值，
如何使用 std::move, 以及如何将右值引用传递给函数。
*/

#include<iostream>
// 包含 utility 头文件以使用 std::move
#include<utility>
/*
vectors 本质上是动态数组。
std::vetor<int> 类型是一个整数数组。
主要的是，vectors 占用相当可观的内存，它们在这里展示了使用 std::move 的性能优势。
*/
#include<vector>

// 接受右值引用作为参数的函数
// 它接管传入的 vector 所有权，在其末尾添加 3，
// 并打印 vector 中的值
/*
[psNote]:
这里补充对下面遍历语法的说明，
for(const int& item: val){}
1. 使用引用的方式对数据进行遍历，第一个是引用本身不占用额外空间，不是复制元素 int item：val 的方式每一轮都要创建一个副本
2. const 强调指针指向的内容不可变
*/
void move_add_three_and_print(std::vector<int> && vec){
    std::vector<int> vec1 = std::move(vec);
    vec1.push_back(3);
    for(const int &item: vec1){
        std::cout << item << " ";
    }
    std::cout << "\n";
}

// 接受右值引用作为参数的函数。
// 它在传入的 vector 末尾添加 3, 并打印 vector 中的值。
// 需要注意的是，它不会接管 vector 的所有权。
// 因此，传入的参数在调用者上下文中仍然可以使用
void add_three_and_print(std::vector<int>&& vec){
    vec.push_back(3);
    for(const int &item: vec){
        std::cout << item << " ";
    }
    std::cout << "\n";
}

int main(){
    // 'a' 是一个左值，因为它是一个变量，指向内存中的特定位置（即存储'a'的地方）。而 10 是一个右值
    int a = 10;
    
    // 让我们看一个从一个左值移动数据到另一个左值的基本示例。
    // 我们在这里定义了一个整数 vector
    std::vector<int> int_array = {1, 2, 3, 4};

    // 现在，我们将这个数组的值移动到另一个左值中。
    std::vector<int> stealing_ints = std::move(int_array);

    // 右值引用是指向数据本身的引用，而不是左值。
    // 对左值（例如 stealing_ints) 调用 std::move 会将表达式转换为右值引用。
    std::vector<int> &&rvalue_stealing_ints = std::move(stealing_ints);

    // 之后仍然可以访问 stealing_ints 中的数据。
    // 因为它是拥有数据的左值，而不是 rvalue_stealing_ints.
    std::cout << "print from stealing_ints: " << stealing_ints[1] << std::endl;

    // 可以将右值引用传递给函数
    // 但是，一旦右值从调用者上下文中的左值移动到被调用者上下文中的左值。
    // 它对调用者来说实际上就不可用了
    // 本质上，在调用 move_add_three_and_print 后，我们无法再使用 int_array2 中的数据。
    // 它不再属于 int_array2 左值。
    std::vector<int> int_array2 = {1, 2, 3, 4};
    std::cout << "Calling move_and_three_and_print...\n";
    move_add_three_and_print(std::move(int_array2));

    // 如果我们在调用者上下文中没有将左值移动到被调用者上下文中的任何左值。
    // 那么函数实际上会将传入的右值引用当作普通引用处理
    // 此时调用者上下文中的左值仍然拥有 vector 数据。
    std::vector<int> int_array3 = {1, 2, 3, 4};
    std::cout << "Calling add_three_and_print...\n";
    add_three_and_print(std::move(int_array3));

    std::cout << "print from int_array3: " << int_array3[1] << std::endl;
    return 0;
}