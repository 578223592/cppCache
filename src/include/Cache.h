





#ifndef CACHE_H
#define CACHE_H

/// @brief 
/// @tparam TK 
/// @tparam TV 
template <typename TK, typename TV>
class Cache
{
private:
    /* data */
public:
    Cache():totalCount_(0),hitCount_(0){}
    virtual ~Cache() = default;
    virtual  TV get(TK) = 0;
    /**
     *
     * @param key
     * @param val
     */
    virtual void set(TK , TV ) = 0;

public:
    int64_t totalCount_;
    int64_t hitCount_;
};


#endif // CACHE_H