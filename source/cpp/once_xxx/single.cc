
#include <memory>
#include <mutex>
#include <iostream>

template <typename T>
class  Singletonon
{
public:  
     static T *getInstance();
private: 
     Singletonon() { std::cout<< "Singleton" << std::endl; }
     static void init();
     static  T *inst_;
     static std::once_flag init_flag_;
     

};
template <typename T>  void Singletonon<T>::init() {
     inst_ = new T;
    std::cout<<"init()"<<std::endl;
}
template <typename T>   T *Singletonon<T>::getInstance() {
    std::call_once(init_flag_,Singletonon::init);
    return inst_;
}

int  main()
{
  
    Singletonon<int>::getInstance();
Singletonon<int>::getInstance();
    return  0;
}