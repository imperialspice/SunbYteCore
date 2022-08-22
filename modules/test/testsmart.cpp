#include <memory>
#include <iostream>
#include <string>

class base{
public:
    std::string t;
	virtual void create() = 0;
    virtual ~base() = default;

};

class test : public base{
public:


    void create() override{
        //t = "hello world";
    }
    test(){
        create();
    }


};

int main(){

    base * h = new test();
    std::cout << h->t << std::endl;
    std::cout << typeid(h).name() << std::endl;

	std::unique_ptr<base> smt = std::make_unique<test>();
	std::cout << smt.get()->t << std::endl;


}
