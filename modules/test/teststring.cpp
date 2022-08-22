#include <string>
#include <iostream>

std::string a;
std::string b;

int main(){
std::cout << a.c_str() << std::endl;
std::cout << (NULL == a.c_str())<< std::endl;
std::cout << b.c_str() << std::endl;
return 0;
}
