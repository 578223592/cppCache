# cppCache
cpp实现的缓存算法仓库，包括不同算法的具体实现和命中率性能对比

## Algorithm

- lFU
- LRU
- ARC

## Useage
```
cd build
cmake ..
make
```
之后可执行文件应该会生成在`${PROJECT_SOURCE_DIR}/bin`目录下



## How to test your cache algorithm
- 继承Cache虚基类
- 在CMakeLists文件中添加对应的编译代码
- 在per_test.cpp文件中添加对应的算法

> 细节可参考已有的算法实现


## Reference
- https://github.com/Xiaoccer/ARC_Cache
- https://github.com/djblue/caching
- https://zhuanlan.zhihu.com/p/522306900
