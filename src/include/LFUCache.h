//
// Created by swx on 23-12-5.
//

#ifndef CPPCACHE_LFUCACHE_H
#define CPPCACHE_LFUCACHE_H
#include <list>
#include <unordered_map>
#include <map>
#include <iostream>
#include "Cache.h"
// https://leetcode.cn/problems/lfu-cache/solutions/2457716/tu-jie-yi-zhang-tu-miao-dong-lfupythonja-f56h/

template <typename TK, typename TV>
class LFUCache :public Cache<TK,TV>
{
private:
private:
    //    使用Node代替pair，是为了方便的保存freq，虽然有很多冗余，但是写起来方便 ，todo：后续考虑换成tuple
    class Node
    {
    public:
        Node(TK key, TV val, int freq) : key_(key), val_(val), freq_(freq) {}

    public:
        TK key_;
        TV val_;
        int freq_;
    };

private:
    int capacity_;
    typedef std::list<Node *> oneCacheList;
    std::map<int, std::list<Node *>> freqToListUMap;                       // 频率-频率对应的链表
    std::unordered_map<TK, typename oneCacheList::iterator> keyToNodeUMap; // key-对应的Node在List中的iterator

public:
    explicit LFUCache(int capacity) : capacity_(capacity) {}
    void set(TK key, TV val)
    {
        this->totalCount_++;
        if (keyToNodeUMap.find(key) != keyToNodeUMap.end())
        {
            this->hitCount_++;
            //            如果有该节点
            typename oneCacheList::iterator it = keyToNodeUMap[key];
            //            std::cout<< typeid(it).name()<<std::endl;
            (*it)->val_ = val;
            moveNodeToNextFreq(it);
        }
        else
        {
            //            如果没有该节点
            if (keyToNodeUMap.size() >= capacity_)
            {
                eraseOneNode();
            }
            Node *node = new Node(key, val, 1);
            freqToListUMap[1].push_front(node);
            keyToNodeUMap[key] = freqToListUMap[1].begin();
        }
    }
    //    虽然操作的变量多了，但是基本的逻辑和LRU是一样的，是找到这个节点后，先删除再移动到指定的位置
    TV get(TK key) override
    {
        if (keyToNodeUMap.find(key) == keyToNodeUMap.end())
        {
            return TV();
        }
        typename std::list<Node *>::iterator it = keyToNodeUMap[key];
        auto res = (*it)->val_;
        moveNodeToNextFreq(it);
        return res;
    }

private:
    /**
     * 清除list中指定的it，不设计map和Node*
     * @param it
     */
    void eraseListIt(typename oneCacheList::iterator it)
    {
        int freq = (*it)->freq_;
        freqToListUMap[freq].erase(it);
        if (freqToListUMap[freq].size() == 0)
        { // 如果为空，就删除该列表
            freqToListUMap.erase(freq);
        }
    }
    void moveNodeToNextFreq(typename oneCacheList::iterator it)
    {
        Node *node = (*it);
        eraseListIt(it);
        node->freq_++;
        freqToListUMap[node->freq_].emplace_front(node);
    }
    /**
     * 在满时删除一个应该删除的节点
     * 函数在满容量的时候调用，这时候freqToListUMap一定不是空的，保证正确性
     */
    void eraseOneNode()
    {
        int minFreq = freqToListUMap.begin()->first;
        auto it = freqToListUMap[minFreq].begin();
        keyToNodeUMap.erase((*it)->key_);
        eraseListIt(it);

        delete (*it);
    }
};

#endif // CPPCACHE_LFUCACHE_H
