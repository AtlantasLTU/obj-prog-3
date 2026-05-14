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

template<class T, class Allocator>
void Vector<T, Allocator>::reserve(size_type new_cap)
{
    if(new_cap <= capacity_)
        return;
    
    pointer new_data = 
        std::allocator_traits<Allocator>::allocate(
            alloc_,
            new_cap
        );
    
    for(size_type i = 0; i < size_; i++){
        std::allocator_traits<Allocator>::construct(
            alloc_,
            new_data,
            std::move_if_noexcept(*(data_+i))
        );

        std::allocator_traits<Allocator>::destroy(
            alloc_,
            data_ + i
        );
    }

    if(data_)
    {
        std::allocator_traits<Allocator>::deallocate(
            alloc_,
            data_,
            capacity_
        );
    }

    data_ = new_data;
    capacity_ = new_cap;
}

template<class T, class Allocator>
void Vector<T, Allocator>::push_back(const_reference pos)
{
    if(size_ == capacity_){

    }
}

template<class T, class Allocator>
void Vector<T, Allocator>::push_back(value_type&& pos)
{
    if(size_ == capacity_){

    }
}
// copy constructor:
template<class T, class Allocator>
Vector<T, Allocator>::Vector(const Vector& other)
    : data_(nullptr),
    size_(0),
    capacity_(0),
    alloc_(other.alloc_)
{
    reserve(other.size_);

    for(size_type i = 0; i < other.size_; i++)
    {
        std::allocator_traits<Allocator>::construct(
            alloc_,
            data_ + i,
            *(other.data_ + i)
        );
    }

    size_ = other.size_;
}

// copy assign
template<class T, class Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& other)
{
    if(this != &other)
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

        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;

        reserve(other.size_);

        for (size_type i = 0; i < other.size_; ++i)
        {
            std::allocator_traits<Allocator>::construct(
                alloc_,
                data_ + i,
                *(other.data_ + i)
            );
        }

        size_ = other.size_;
    }
    return *this;
}
// move constructor:
template<class T, class Allocator>
Vector<T, Allocator>::Vector(Vector&& other) noexcept
    : data_(other.data_),
      size_(other.size_),
      capacity_(other.capacity_),
      alloc_(std::move(other.alloc_))
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}
// move assign
template<class T, class Allocator>
Vector<T, Allocator>&
Vector<T, Allocator>::operator=(Vector&& other) noexcept
{
    if (this != &other)
    {
        clear();

        if (data_)
        {
            std::allocator_traits<Allocator>::deallocate(
                alloc_,
                data_,
                capacity_
            );
        }

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        alloc_ = std::move(other.alloc_);

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    return *this;
}