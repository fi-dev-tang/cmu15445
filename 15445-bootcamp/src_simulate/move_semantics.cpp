/*
比较陌生的知识点，左值（lvalue），右值（rvalue）比如 42, a + 5 这种无法长期驻留在内存中，没有具体内存位置对应的值
同时也没有办法取地址
左值引用 T& 右值引用 T&&
强调 std::move 移动语义
C++11 的新特性，
这个的好处就是，比如本来我创建了一个动态数组 std::vector<int> vec1,
现在我想要把 vec1 中的内容全部转移到 vec2 中，甚至说废掉 vec1。
原来的操作是深拷贝，创建新的内存空间然后逐个复制，时间复杂度是 O(n),
但实际上使用 std::move 之后可以把时间复杂度降低到 O(1)。
同时原本新旧内存都要保持，现在不需要额外开辟内存空间。

std::move 最本质的作用是将一个左值转换成一个右值，具体资源是否释放了，原来的左值是否还能用？
需要看具体是否发送了移动资源的行为（比如函数里创建了新的变量，或者将所有权从一个左值转移到另一个左值）
*/
#include<iostream>
#include<utility> // std::move 所在
#include<vector>

void move_and_add_three(std::vector<int>&& vec1){
    std::vector<int> new_vec = std::move(vec1);
    new_vec.push_back(3);
    for(const int& item: new_vec){
        std::cout << item << " ";
    }
    std::cout << "\n" << std::endl;
}

void add_three(std::vector<int>&& vec1){
    vec1.push_back(3);
    for(const int&item: vec1){
        std::cout << item << " ";
    }
    std::cout << "\n" << std::endl;
}


int main(){
    /*
    ===========================================================================
    第一种 sample:
    1. 先使用 std::move 真正转移所有权，将元素从一个左值转移到另一个左值
    2. 然后使用右值引用，这里赋值给了右值引用，但是还是能访问到原来的左值资源（因为并没有发生实际的资源释放行为）
    */
    std::vector<int> vec1 = {1, 2, 3, 4};
    std::vector<int> stealing_ints = std::move(vec1);  
    std::vector<int>&& rvalue_stealing_ints = std::move(stealing_ints);

    // std::cout << "First element of vec1 " << vec1[0] << std::endl;    // 转移所有权之后已经访问不到了，直接运行会报 Segmentation fault
    std::cout << "First element of stealing_ints " << stealing_ints[0] << std::endl;

    /*
    ===========================================================================
    第二种 sample:
    1. 把右值引用传递给函数，函数将所有权转移给新的局部变量
    2. 函数结束，局部变量生命周期结束，资源立刻释放
    */
    std::vector<int> vec2 = {1, 2, 3, 4};
    std::cout << "Move and add_three" << std::endl;
    move_and_add_three(std::move(vec2));
    // std::cout << "First element of vec2 " << vec2[0] << std::endl;  // 资源被局部变量释放，也是所有权转移，Segmentation fault

    /*
    =========================================================================
    第三种 sample:
    1. 函数本身没有转移所有权，还可以打印出原来左值的值
    */
    std::vector<int> vec3 = {1, 2, 3, 4};
    std::cout << "Not Move and add_three" << std::endl;
    add_three(std::move(vec3));
    std::cout << "First element of vec3 " << vec3[0] << std::endl;

    return 0;
}