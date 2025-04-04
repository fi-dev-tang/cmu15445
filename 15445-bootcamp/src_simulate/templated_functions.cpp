// 展示模板函数的使用，这里不区分 template<typename T> 和 template<classs T> 的区别
#include<iostream>

// 1. 定义拥有一个模板类型参数的函数
template<typename T>
T add_two(T a, T b){
    return a + b;
}

// 2. 定义拥有两个类型的模板参数的函数
template<typename U, typename V>
void print_two(U a, V b){
    std::cout << "print_two: " << a << " and " << b << std::endl;
}

// 3. 对于模板类型而言，可以限制在某一个float 类型上为专门化的函数，其他部分照旧
template<typename T>
void print_msg(T a){
    std::cout << "Hello World!" << std::endl;
}

// 对 float 进行专门化
template<>
void print_msg<float>(float a){
    std::cout << "calling print_msg on float: " << a << std::endl;
}

// 4.限制住模板类型的参数，好处是在编译时进行确定，省去多余的分支
template<bool T>
int add_3(int a){
    if(T){
        return a + 3;
    }
    return a;
}


int main(){
    // 1. 调用上面的 add_two
    std::cout << "Calling add_two<int>(3, 5): " << add_two<int>(3, 5) << std::endl;
    std::cout << "Calling add_two<float>(2.8, 3.7): " << add_two<float>(2.8, 3.7) << std::endl;

    // 2. 调用上面的 print_two
    std::cout << "Calling print_two<int, float>(3, 2.8): "; 
    print_two<int, float>(3, 2.8);
    
    // 3. 调用上面的 print_msg
    std::cout << "Calling print_msg<double>: ";
    print_msg<double>(2.5);

    // 3. 调用上面的 print_msg(float)
    std::cout << "Calling print_msg<float>: ";
    print_msg<float>(2.5);

    // 4. 调用上面的 bool T
    std::cout << "Calling add_3<true>(3): " << add_3<true>(3) << std::endl;
    std::cout << "Calling add_3<false>(3): " << add_3<false>(3) << std::endl;
    return 0;
}