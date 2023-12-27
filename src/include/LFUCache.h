//
// Created by swx on 23-12-5.
//

#ifndef CPPCACHE_LFUCACHE_H
#define CPPCACHE_LFUCACHE_H
#include <list>
#include <unordered_map>
#include <map>
#include <unordered_map>
#include <memory>

#include "Cache.h"
// https://leetcode.cn/problems/lfu-cache/solutions/2457716/tu-jie-yi-zhang-tu-miao-dong-lfupythonja-f56h/

template<typename TK, typename TV>
class LFUCache : public Cache<TK, TV> {
private:
private:
    //    使用Node代替pair，是为了方便的保存freq，虽然有很多冗余，但是写起来方便 ，todo：后续考虑换成tuple
    class Node {
    public:
        Node(TK key, TV val, int freq) : key_(key), val_(val), freq_(freq) {
        }


        TK key_;
        TV val_;
        int freq_;
    };

private:
    int capacity_;

    typedef std::list<std::shared_ptr<Node>> oneCacheList;
    std::map<int, std::shared_ptr<oneCacheList> > freqToListUMap; // 频率-频率对应的链表
    std::unordered_map<TK,typename oneCacheList::iterator > keyToNodeUMap; // key-对应的Node在List中的iterator

public:
    explicit LFUCache(int capacity) : capacity_(capacity) {
    }

    void set(TK key, TV val) override {
        ++this->totalCount_;
        if (keyToNodeUMap.find(key) != keyToNodeUMap.end()) {
            ++this->hitCount_;
            //            如果有该节点
            auto it = keyToNodeUMap[key];
            //            std::cout<< typeid(it).name()<<std::endl;
            (*it)->val_ = val;
            moveNodeToNextFreq(it);
        } else {
            //            如果没有该节点
            if (keyToNodeUMap.size() >= capacity_) {
                eraseOneNode();
            }

            if (freqToListUMap.find(1) == freqToListUMap.end()) {
                auto ptr = new oneCacheList();
                freqToListUMap[1] = std::shared_ptr<oneCacheList>(ptr);  //what fuck？use make_shared will make error
                // freqToListUMap[1] = std::make_shared<oneCacheList>();
            }
            auto node = std::make_shared<Node>(key, val, 1);
            freqToListUMap.at(1)->push_front(node);
            keyToNodeUMap[key] = freqToListUMap.at(1)->begin();
        }
    }

    //    虽然操作的变量多了，但是基本的逻辑和LRU是一样的，是找到这个节点后，先删除再移动到指定的位置
    TV get(TK key) override {
        if (keyToNodeUMap.find(key) == keyToNodeUMap.end()) {
            return TV();
        }
        typename  std::list<std::shared_ptr<Node> >::iterator it = keyToNodeUMap[key];
        auto res = (*it)->val_;
        moveNodeToNextFreq(it);
        return res;
    }

private:
    /**
     * 清除list中指定的it，不设计map和Node*
     * @param it
     */
    void eraseListIt(const typename oneCacheList::iterator &it) {
        int freq = (*it)->freq_;
        freqToListUMap.at(freq)->erase(it);
        if (freqToListUMap.at(freq)->size() == 0) {
            // 如果为空，就删除该列表
            freqToListUMap.erase(freq);
        }

    }

    void moveNodeToNextFreq(const typename oneCacheList::iterator &it) {
        auto nodeSPtr = (*it);
        eraseListIt(it);
        ++nodeSPtr->freq_;
        if(freqToListUMap.find(nodeSPtr->freq_) == freqToListUMap.end()){
            auto ptr = new oneCacheList();
            freqToListUMap[nodeSPtr->freq_] = std::shared_ptr<oneCacheList>(ptr);

            // freqToListUMap[nodeSPtr->freq_] = std::make_shared<oneCacheList>();
        }
        freqToListUMap[nodeSPtr->freq_]->emplace_front(nodeSPtr);
    }

    /**
     * 在满时删除一个应该删除的节点
     * 函数在满容量的时候调用，这时候freqToListUMap一定不是空的，保证正确性
     */
    void eraseOneNode() {
        int minFreq = freqToListUMap.begin()->first;
        auto nodeSharedPtr = freqToListUMap.begin()->second->back(); // 最后一个元素是最老的，最新使用的插在对头

        eraseListIt(--(freqToListUMap.at(minFreq)->end()));
        keyToNodeUMap.erase(nodeSharedPtr->key_);

    }
};

#endif // CPPCACHE_LFUCACHE_H
