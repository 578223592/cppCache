//
// Created by swx on 23-12-5.
//
#include <string>
#include "LRUCache.h"
#include <iostream>
int main(){

    auto *pLruCache = new LRUCache<std::string , std::string>(10);
    pLruCache->set("1","1");

    std::cout<<pLruCache->get("1")<<std::endl;
    std::cout<<pLruCache->get("2")<<std::endl;
    delete pLruCache;
    return 0;
}

