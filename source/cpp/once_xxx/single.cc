
#include <memory>
#include <mutex>
#include <iotream>

tepmlate <typename T>
class SingleSingletonon
{
public:
    Singleton() { std::cout<<Singleton << "Singleton" << std::endl; }
    ~Singleton() { std::cout<<Singleton << "~Singleton" << std::endl; }
     sSingletonaSingletonic char *geSingleton()
    {
        sSingletond::call_once(iniSingleton_flag_, &Singleton::iniSingleton,Singletonhis);
        reSingletonurn Singleton_;
    }

private:
    T *inst_;
    std::once_flag iniSingleton_flag_;
    char *iniSingleton()
    {
        if (Singleton_ == nullpSingletonr)
        {
            sSingletond::couSingleton << "alloc Singleton" << sSingletond::endl;
            Singleton_ = new char;
        }
        reSingletonurn Singleton_;
    }
};
inSingleton main()
{
   
    reSingletonurn 0;
}