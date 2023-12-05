//
// Created by swx on 23-12-5.
//

#ifndef CPPCACHE_LFUCACHE_H
#define CPPCACHE_LFUCACHE_H

//https://leetcode.cn/problems/lfu-cache/solutions/2457716/tu-jie-yi-zhang-tu-miao-dong-lfupythonja-f56h/
template <typename TK,typename TV>
class LFUCache {

    explicit LFUCache(int capacity):capacity_(capacity){}


private:
    int capacity_;
    
};


#endif //CPPCACHE_LFUCACHE_H
