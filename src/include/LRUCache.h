//
// Created by swx on 23-12-4.
//

#ifndef CPPCACHE_LRUCACHE_H
#define CPPCACHE_LRUCACHE_H

#include <list>
#include <iostream>
#include <unordered_map>
#include <string>

template<typename TK, typename TV>
class LRUCache {

public:
    explicit LRUCache(int capacity);

    TV get(TK key) {
        if (iteratorMap_.find(key) == iteratorMap_.end()) {
            return TV(); //这个只适用于一些特殊的TV模板
        }
        auto value = iteratorMap_[key]->second;

        eraseByKey(key);

        insert(key,value);
    }

    /**
     * 如果有这个key，该值即可。没有这个key，需要插入
     * @param key
     * @param val
     */
    void set(TK key, TV newVal) {
        //有这个key
        if (iteratorMap_.find(key) != iteratorMap_.end()) {
            eraseByKey(key);
        }
//        没有这个key
        if (cacheList_.size() >= capacity_) {
//            检查size是否达到容量，如果到达容量需要删除
            auto toDeletedIt = cacheList_.back();
            iteratorMap_.erase(toDeletedIt.first);
            cacheList_.pop_back();
        }
        insert(key,newVal);
    }

private:
    void eraseByKey(TK &key){
        cacheList_.erase(iteratorMap_[key]);
        iteratorMap_.erase(key);
        capacity_--;
    }
    /**
     * 插入节点，自然是插到队头(最近刚使用的那个节点)
     * @param key
     * @param val
     */
    void insert(TK key,TV val){
        cacheList_.emplace_front(std::make_pair(key, val));
        iteratorMap_[key] = cacheList_.begin();
        capacity_++;
    }

private:
    int capacity_;
    //如果在模板里面使用了一个模板类，则需要使用typename关键词
    std::unordered_map<TK, typename std::list<std::pair<TK, TV>>::iterator> iteratorMap_;   //key - cacheList_中的iterator
    std::list<std::pair<TK, TV>> cacheList_;   //存放pair<key,val>的list
};

template<typename TK, typename TV>
LRUCache<TK, TV>::LRUCache(int capacity):capacity_(capacity) {

}


#endif //CPPCACHE_LRUCACHE_H
