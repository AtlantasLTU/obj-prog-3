#include "vector.h"

template <class T, class Allocator>
Vector<T, Allocator>::~Vector()
{
    clear();
    if(data_)
    {
        std::allocator_traits<Allocator>::deallocate(
            alloc_,
            data_,
            capacity_
        );
    }
}

template<class T, class Allocator>
void Vector<T, Allocator>::clear(){
    for(size_type i = 0; i < size_; i++)
    {
        std::allocator_traits<Allocator>::destroy(
            alloc_,
            data_ + i
        );
    }

    size_ = 0;
}