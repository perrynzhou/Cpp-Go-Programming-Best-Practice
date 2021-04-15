#include <list>
#include <atomic>
#include <unordered_map>
#include <string>
#include <mutex>
#include <cstdint>
#include <list>
#include <iostream>
template <typename KeyType, typename ValueType>
struct LRUNode
{
    KeyType key;
    ValueType value;
    LRUNode(KeyType key, ValueType value) : key(key), value(value) {}
    ~LRUNode() {}
};
template <typename KeyType, typename ValueType>
class LRUCache
{
public:
    LRUCache(uint32_t max) : max_(max), cur_(0) {}
    ~LRUCache();
    bool Put(KeyType k, ValueType v);
    ValueType Get(KeyType k);
    void Print(int i);

private:
    uint32_t max_;
    std::atomic<uint32_t> cur_;
    std::list<struct LRUNode<KeyType, ValueType> *> lru_;
    std::unordered_map<KeyType, struct LRUNode<KeyType, ValueType> *> cache_;
    std::mutex lock_;
};

template <typename KeyType, typename ValueType>
LRUCache<KeyType, ValueType>::~LRUCache()
{
    lru_.clear();
    cache_.clear();
    cur_ = 0;
}
template <typename KeyType, typename ValueType>
bool LRUCache<KeyType, ValueType>::Put(KeyType k, ValueType v)
{
    auto item = cache_.find(k);
    std::lock_guard<std::mutex> lock(lock_);
    struct LRUNode<KeyType, ValueType> *node = new LRUNode<KeyType, ValueType>(k, v);
    struct LRUNode<KeyType, ValueType> *tmp_ = nullptr;
    // data already exists,just update
    if (item == cache_.end())
    {
        if (cur_ >= max_)
        {
            tmp_ = lru_.back();
            cache_.erase(tmp_->key);
            lru_.pop_back();
        }
        cache_[k] = node;
        lru_.push_front(node);
        cur_++;
        return true;
    }
    tmp_ = cache_[k];
    tmp_->value = v;
    lru_.remove(tmp_);
    lru_.push_front(tmp_);
    return true;
}
template <typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::Print(int i)
{
    std::cout << "*******************" << i << "**********************" << std::endl;

    for (auto const &node : lru_)
    {

        std::cout << "{key=" << node->key << ",value=" << node->value << "}" << std::endl;
    }
}
template <typename KeyType, typename ValueType>
ValueType LRUCache<KeyType, ValueType>::Get(KeyType k)
{

    struct LRUNode<KeyType, ValueType> *node = cache_[k];
    if (node == nullptr)
    {
        return -1;
    }
    return node->ValueType;
}
void test1()
{
    LRUCache<std::string, int> cache(4);
    int count=0;
    for (int i = 0; i < 20; i++)
    {
        count++;
        char buf[64] = {'\0'};
        snprintf((char *)&buf, 64, "%d", i);
        std::cout << "put:" << cache.Put(std::string(((char *)&buf)), i);
        cache.Print(count);
    }
    count++;
    char buf[64] = {'\0'};
    snprintf((char *)&buf, 64, "%d", 14);
    std::cout << "put:" << cache.Put(std::string(((char *)&buf)), 1024);
    cache.Print(count);
}
void test2()
{
    LRUCache<int, int> cache(4);
      int count=0;
    for (int i = 0; i < 20; i++)
    {
        count++;
        std::cout << "put:" << cache.Put(i,i);
        cache.Print(i);
    }
     count++;
    std::cout << "put:" << cache.Put(18, 1024);
    cache.Print(count);
}
int main() 
{
    test2();
    return 0;
}