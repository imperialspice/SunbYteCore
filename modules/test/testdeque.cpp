#include <deque>
#include <memory>
#include <iostream>

class ff{
public:
ff(){}
int a = 5;
};


int main(){
    std::shared_ptr<std::deque<std::unique_ptr<ff>>> list = std::make_shared<std::deque<std::unique_ptr<ff>>>();

    std::unique_ptr<ff> a = std::make_unique<ff>();

    (*list).push_back(std::move(a));

    std::unique_ptr<ff> ac = std::move((*list).front());
    std::cout << ac->a << std::endl;
    std::cout << list->size() << std::endl;
    (*list).pop_front();
    std::cout << list->size() << std::endl;

}
