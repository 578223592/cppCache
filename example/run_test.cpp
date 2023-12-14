//
// Created by swx on 23-12-5.
//
#include <string>
#include "LRUCache.h"
#include "LFUCache.h"
#include "ARCCache.h"
#include <iostream>
int main(){

//    todo：LFUCache完成，待测试和检查
    auto *pLruCache = new LRUCache<std::string , std::string>(10);
    pLruCache->set("1","1");

    std::cout<<pLruCache->get("1")<<std::endl;
    std::cout<<pLruCache->get("2")<<std::endl;
    delete pLruCache;

    auto *pLfuCache = new LFUCache<std::string , std::string>(10);
    pLfuCache->set("1","1");

    std::cout<<pLfuCache->get("1")<<std::endl;
    std::cout<<pLfuCache->get("2")<<std::endl;
    delete pLfuCache;


    auto *pARCCache = new ARCCache<std::string , std::string>(10);
    pARCCache->set("1","1");

    std::cout<<pARCCache->get("1")<<std::endl;
    std::cout<<pARCCache->get("2")<<std::endl;
    delete pARCCache;
    return 0;
}

