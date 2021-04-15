#include <iostream>
class A
{
public:
    A()
    {

        std::cout << "A()" << std::endl;
    }
    ~A()
    {
        std::cout << "~A()" << std::endl;
    }
};
class B : public A
{
public:
    B()
    {
        std::cout << "B()" << std::endl;
    }
    virtual ~B()
    {
        std::cout << "~B()" << std::endl;
    }
};
int main(void)
{

    A *a = new B;
    std::string s;
    s.clear();
    s.resize(8);
    char *p = &s[2];
    uint8_t *const buffer = reinterpret_cast<uint8_t *>(p);
    buffer[0] = '0';
    buffer[1] = '2';
    s.push_back('a');
    std::cout << "s=" << s << std::endl;
    delete a;
    return 0;
}