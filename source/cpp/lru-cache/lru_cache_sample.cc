#include <list>
#include <atomic>
#include <unordered_map>
#include <string>
#include <mutex>
#include <cstdint>
#include <list>
#include <iostream>
struct LRUNode
{
    std::string key;
    int val;
    LRUNode(std::string key, int val) : key(key), val(val) {}
    ~LRUNode() {}
};
class LRUCache
{
public:
    LRUCache(uint32_t max) : max_(max), cur_(0) {}
    ~LRUCache();
    bool Put(std::string key, int val);
    int Get(std::string);
    void Print(int i);
private:
    uint32_t max_;
    std::atomic<uint32_t> cur_;
    std::list<struct LRUNode *> lru_;
    std::unordered_map<std::string, struct LRUNode *> cache_;
    std::mutex lock_;
};

LRUCache::~LRUCache()
{
    lru_.clear();
    cache_.clear();
    cur_ = 0;
}
bool LRUCache::Put(std::string key, int val)
{
    if (key.size() == 0)
    {
        return false;
    }
    auto item = cache_.find(key);
    std::lock_guard<std::mutex> lock(lock_);
    struct LRUNode *node = new LRUNode(key, val);
    struct LRUNode *tmp_ = nullptr;
    // data already exists,just update
    if (item == cache_.end())
    {
        if (cur_ >= max_)
        {
            tmp_ = lru_.back();
            cache_.erase(tmp_->key);
            lru_.pop_back();
        }
        cache_[key] = node;
        lru_.push_front(node);
        cur_++;
        return true;
    }
    tmp_ = cache_[key];
    tmp_->val = val;
    lru_.remove(tmp_);
    lru_.push_front(tmp_);
    return true;
}
void LRUCache::Print(int i)
{
        std::cout << "*******************"<<i<<"**********************" << std::endl;

    for(auto const& node:lru_) {

        std::cout << "{key="<<node->key << ",val=" << node->val<<"}"<<std::endl;
    }

}
int LRUCache::Get(std::string key)
{

    struct LRUNode *node = cache_[key];
    if (node == nullptr)
    {
        return -1;
    }
    return node->val;
}
int main()
{
    LRUCache cache(10);
    for (int i = 0; i < 20; i++)
    {
        char buf[64] = {'\0'};
        snprintf((char *)&buf, 64, "%d", i);
        std::cout << " result:" << cache.Put(std::string(((char *)&buf)), i);
        cache.Print(i);
    }
         char buf[64] = {'\0'};
        snprintf((char *)&buf, 64, "%d", 14);
        std::cout << " result:" << cache.Put(std::string(((char *)&buf)), 1024);
        cache.Print(21);
    return 0;
}