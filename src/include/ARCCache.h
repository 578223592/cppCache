//
// Created by swx on 23-12-12.
//

#ifndef CPPCACHE_ARCCACHE_H
#define CPPCACHE_ARCCACHE_H
#include <list>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "Cache.h"
//
enum LruType
{
    T1,
    B1,
    T2,
    B2,
    None,
};

template <typename TK, typename TV>
class ARCCache:public Cache<TK,TV>
{
private:
    
private:
    struct ArcEntry;
    using ArcEntryPtr = std::shared_ptr<ArcEntry>;
    struct ArcEntry
    {
        TK key_;
        TV val_;
        LruType lruType_;                              // 因为arc算法涉及在不同的list中间移动节点，因此为了简化流程，进行节点和链表之间的双向绑定，方便从节点快速定位回链表
        typename std::list<ArcEntryPtr>::iterator it_; //
        ArcEntry(TV key, TV val, LruType type) : key_(key), val_(val), lruType_(type), it_(nullptr) {}
    };

public:
    explicit ARCCache(int capacity) ;
    // get和set的逻辑应该是类似的，文中的get相当于是getAndSet方法
    TV get(TK);
    /**
     *
     * @param key
     * @param val
     */
    void set(TK key, TV val);

private:
    /**
     * 真正实现arc的逻辑，即相当于原文中的get方法
     * @param arcEntry
     */
    void hit(ArcEntryPtr arcEntry);

    void move_to_lru(ARCCache::ArcEntryPtr arcEntry, LruType toList);
    /**
     * 这个函数的设计思想个人猜测如下：如果t1大小超了，必须要限制，但是t1命中多，说明后面可能需要增大b1，因此方式是将t1中的移动到b1，反之亦然反之
     * 而对于参数xtInB2的设计可能主要是考虑对于在b2中的情况下，而且数据已经平衡（到p_)，那么后续肯定是要b2->t2的，b2会减少，然后此时b1增大，容易导致b1>b2，然后就后续再命中就很容易让t2增大，跟上缓存需求
     * 感觉上面第二行的描述有点勉强，但是也没有找到/思考到更合理的答案
     * 
    */
    void replace(bool xtInB2);

private:
    const int capacity_;
    int p_;
    std::list<ArcEntryPtr> T1List_;
    std::list<ArcEntryPtr> B1List_;
    std::list<ArcEntryPtr> T2List_;
    std::list<ArcEntryPtr> B2List_;
    std::unordered_map<TK, ArcEntryPtr> arcEntryUMap;
    std::unordered_map<LruType, std::list<ArcEntryPtr> *> listUMap_; // 保存不同list的地址，目的是方便不同节点之间的移动
};

template <typename TK, typename TV>
void ARCCache<TK, TV>::hit(ARCCache::ArcEntryPtr arcEntry)
{
    if((int)B2List_.size()+(int)B1List_.size()+(int)T1List_.size()+(int)T2List_.size() - (int)arcEntryUMap.size()!=0){
        std::cout<<(int)B2List_.size()+(int)B1List_.size()+(int)T1List_.size()+(int)T2List_.size() - (int)arcEntryUMap.size()<<std::endl;

    }
    if (arcEntry->lruType_ != LruType::None)
    {
        //        case 1 2 3
        //        case 1 在缓存中，那么不需要调整p，只需要将缓存移动到t2开头
        if (arcEntry->lruType_ == LruType::T1 || arcEntry->lruType_ == LruType::T2)
        {
            move_to_lru(arcEntry, LruType::T2);
        }
        else if (arcEntry->lruType_ == LruType::B1) //         case 2
        {
            int delta = B1List_.size() >= B2List_.size() ? 1 : B2List_.size() / B1List_.size();
            p_ = std::min(p_ + delta, capacity_);
            replace(false);
            move_to_lru(arcEntry, LruType::T2);
        }
        else if (arcEntry->lruType_ == LruType::B2)
        { //          case 3
            int delta = B2List_.size() >= B1List_.size() ? 1 : B1List_.size() / B2List_.size();
            p_ = std::max(p_-delta,0);
            replace(true);
            move_to_lru(arcEntry,LruType::T2);
        }
        else
        {
            std::cout<<"void ARCCache<TK, TV>::hit  error!"<<"\n";
        }
    }else{

        // miss hit :case 4
        if (T1List_.size() + B1List_.size() == capacity_)
        {
            // case4.1
            if (T1List_.size() < capacity_)
            {
                arcEntryUMap.erase(B1List_.back()->key_);
                B1List_.pop_back();
                replace(false);
            }
            else
            {
                arcEntryUMap.erase(T1List_.back()->key_);
                T1List_.pop_back();
            }
        }
        else
        {
            // case 4.2
            auto size = T1List_.size() + T2List_.size() + B1List_.size() + B2List_.size();
            if (size >= capacity_)
            {
                if (size == capacity_ * 2)
                {
                    arcEntryUMap.erase(B2List_.back()->key_);
                    B2List_.pop_back();
                }
                replace(false);
            }
        }
        // 这里没有生成新的节点，因为如果是新的节点，那么在set方法中已经生成了，这里不需要再重复生成
        arcEntry->lruType_ = LruType::T1;
        T1List_.push_front(arcEntry);
        arcEntry->it_ = T1List_.begin();
        arcEntryUMap[arcEntry->key_] = arcEntry;
    }


}

template <typename TK, typename TV>
inline void ARCCache<TK, TV>::move_to_lru(ARCCache::ArcEntryPtr arcEntry, LruType toListType)
{
    std::list<ARCCache<TK, TV>::ArcEntryPtr> *toList = listUMap_[toListType];
    toList->splice(toList->begin(), *listUMap_[arcEntry->lruType_], arcEntry->it_);
    arcEntry->it_ = toList->begin();
    arcEntry->lruType_ = toListType;
}

template <typename TK, typename TV>
inline void ARCCache<TK, TV>::replace(bool xtInB2)
{
    if (!T1List_.empty() && (T1List_.size() > p_ || (xtInB2 && T1List_.size() == p_)))
    {
        move_to_lru(T1List_.back(), LruType::B1);
    }
    else
    {
        move_to_lru(T2List_.back(), LruType::B2);
    }
}

template <typename TK, typename TV>
void ARCCache<TK, TV>::set(TK key, TV val)
{
    this->totalCount_++;

    ArcEntryPtr arcEntryPtr = nullptr;
    if (arcEntryUMap.find(key) == arcEntryUMap.end())
    {

        arcEntryPtr = std::make_shared<ArcEntry>(key, val, LruType::None);
    }
    else
    {
        this->hitCount_++;
        arcEntryPtr = arcEntryUMap[key];
        arcEntryPtr->val_ = val;
    }
    hit(arcEntryPtr);
}

template <typename TK, typename TV>
inline ARCCache<TK, TV>::ARCCache(const int capacity): capacity_(capacity), p_(0),
T1List_(),T2List_(),B1List_(),B2List_(),arcEntryUMap(),listUMap_()
{
    listUMap_[LruType::T1] = &T1List_;
    listUMap_[LruType::T2] = &T2List_;
    listUMap_[LruType::B1] = &B1List_;
    listUMap_[LruType::B2] = &B2List_;
}

template <typename TK, typename TV>
TV ARCCache<TK, TV>::get(TK key)
{
    //    如果没有就返回，如果存在缓存，那么就进行arc算法对缓存进行相关的淘汰
    if (arcEntryUMap.find(key) == arcEntryUMap.end())
    {
        return TV();
    }
    ArcEntryPtr res = arcEntryUMap[key];

    hit(res);
    return res->val_;
}

#endif // CPPCACHE_ARCCACHE_H
