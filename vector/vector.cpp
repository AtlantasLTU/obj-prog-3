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

template<class T, class Allocator>
typename Vector<T, Allocator>::size_type
Vector<T, Allocator>::size() const
{
    return size_;
}

template<class T, class Allocator>
bool Vector<T, Allocator>::empty() const
{
    return size_ == 0;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::reference
Vector<T, Allocator>::operator[](size_type pos)
{
    return *(data_+pos);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reference
Vector<T, Allocator>::operator[](size_type pos) const
{
    return *(data_+pos);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::pointer
Vector<T, Allocator>::data()
{
    return data_;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_pointer
Vector<T, Allocator>::data() const
{
    return data_;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::reference
Vector<T, Allocator>::front()
{
    return *data_;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reference
Vector<T, Allocator>::front() const
{
    return *data_;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::reference
Vector<T, Allocator>::back()
{
    return *(data_+(size_ - 1));
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reference
Vector<T, Allocator>::back() const
{
    return *(data_+(size_ - 1));
}

template<class T, class Allocator>
typename Vector<T, Allocator>::iterator
Vector<T, Allocator>::begin()
{
    return data_;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_iterator
Vector<T, Allocator>::cbegin() const
{
    return data_;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::iterator
Vector<T, Allocator>::end()
{
    return data_+size_;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_iterator
Vector<T, Allocator>::cend() const
{
    return data_+size_;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::reverse_iterator
Vector<T, Allocator>::rbegin()
{
    return reverse_iterator(end());
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reverse_iterator
Vector<T, Allocator>::crbegin() const
{
    return const_reverse_iterator(cend());
}

template<class T, class Allocator>
typename Vector<T, Allocator>::reverse_iterator
Vector<T, Allocator>::rend()
{
    return reverse_iterator(begin());
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reverse_iterator
Vector<T, Allocator>::crend() const
{
    return const_reverse_iterator(cbegin());
}