#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <iterator>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <compare>
#include <cassert>

// taikomasi į C++20 vektoriaus funkcionalumą;
template <class T, class Allocator = std::allocator<T>>
class Vector{
    public:
    // member types:
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // member functions:
        // constructors:
            // default:
                Vector() : data_(nullptr), size_(0), capacity_(0), alloc_(Allocator{}) {}
                explicit Vector (size_type count, const Allocator& alloc = Allocator{})
                    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc)
                {
                    resize(count);
                }
                Vector(size_type count, const_reference value, const Allocator& alloc = Allocator{})
                    : data_(nullptr),
                    size_(0),
                    capacity_(0),
                    alloc_(alloc)
                {
                    reserve(count);
                    for(size_type i = 0; i < count; ++i)
                    {
                        std::allocator_traits<Allocator>::construct(
                            alloc_,
                            data_+i,
                            value
                        );
                    }
                    size_ = count;
                }
                template<class InputIt, class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
                Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator{})
                    : data_(nullptr),
                    size_(0),
                    capacity_(0),
                    alloc_(alloc)
                {
                    size_type count = static_cast<size_type>(std::distance(first, last));
                    reserve(count);
                    for(size_type i = 0; first != last; ++first, ++i)
                    {
                        std::allocator_traits<Allocator>::construct(
                            alloc_,
                            data_+i,
                            *first
                        );
                    }
                    size_ = count;
                }

                Vector(std::initializer_list<T> ilist, const Allocator& alloc = Allocator{})
                    : Vector(ilist.begin(), ilist.end(), alloc) {}
            // copy constructor:
                Vector(const Vector& other)
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
                };

                Vector(const Vector& other, const Allocator& alloc)
                    : data_(nullptr),
                    size_(0),
                    capacity_(0),
                    alloc_(alloc)
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
                };
            // copy-assignment operator:
                Vector& operator=(const Vector& other)
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
                };
            // move constructor:
                Vector(Vector&& other) noexcept
                    : data_(other.data_),
                    size_(other.size_),
                    capacity_(other.capacity_),
                    alloc_(std::move(other.alloc_))
                {
                    other.data_ = nullptr;
                    other.size_ = 0;
                    other.capacity_ = 0;
                };

                Vector(Vector&& other, const Allocator& alloc)
                    : data_(nullptr),
                    size_(0),
                    capacity_(0),
                    alloc_(alloc)
                {
                    if(alloc == other.alloc_)
                    {
                        data_ = other.data_;
                        size_ = other.size_;
                        capacity_ = other.capacity_;
                        other.data_ = nullptr;
                        other.size_ = 0;
                        other.capacity_ = 0;
                    }
                    else
                    {
                        reserve(other.size_);
                        for(size_type i = 0; i < other.size_; ++i)
                        {
                            std::allocator_traits<Allocator>::construct(
                                alloc_,
                                data_ + i,
                                std::move(*(other.data_+i))
                            );
                            
                        }
                        size_ = other.size_;
                        other.clear();
                    }
                };
            // operator=(initializer_list):
                Vector& operator=(std::initializer_list<T> ilist)
                {
                    assign(ilist.begin(), ilist.end());
                    return *this;
                };
            // move-assignment operator:
                Vector& operator=(Vector&& other) noexcept
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
                };
        // destructors:
            ~Vector()
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
            };
        // assign:
            void assign(size_type count, const_reference value)
            {
                if(count > capacity_)
                {
                    reserve(count);
                }

                if(count <= size_)
                {
                    std::fill_n(data_, count, value);
                    for(size_type i = count; i < size_; ++i) {
                        std::allocator_traits<Allocator>::destroy(
                            alloc_,
                            data_ + i
                        );
                    }
                }
                else
                {
                    std::fill_n(data_, size_, value);
                    size_type i = size_;
                    try {
                        for(; i < count; ++i)
                        {
                            std::allocator_traits<Allocator>::construct(
                                alloc_,
                                data_ + i,
                                value
                            );
                        }
                    } catch (...) {
                        for(size_type j = size_; j < i; ++j)
                        {
                            std::allocator_traits<Allocator>::destroy(
                                alloc_,
                                data_ + j
                            );
                        }
                        throw;
                    }
                }
                size_ = count;
            }

            template <class InputIt, class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
            void assign(InputIt first, InputIt last)
            {
                size_type new_size = static_cast<size_type>(std::distance(first, last));

                if (new_size > capacity_)
                    reserve(new_size);

                size_type i = 0;

                for(; i < size_ && first != last; ++i, ++first){
                    *(data_+i) = *first;
                }
                size_type constructed = i;
                try {
                    for(; first != last; ++first, ++i){
                        std::allocator_traits<Allocator>::construct(
                            alloc_,
                            data_ + i,
                            *first
                        );
                    }
                } catch (...) {
                    for(size_type j = constructed; j < i; ++j){
                        std::allocator_traits<Allocator>::destroy(
                            alloc_,
                            data_ + j
                        );
                    }    
                    size_ = constructed;
                    throw;
                }

                for(size_type j = new_size; j < size_; ++j){
                    std::allocator_traits<Allocator>::destroy(
                        alloc_,
                        data_ + j
                    );
                }
                size_ = new_size;
            }

            void assign(std::initializer_list<T> ilist) {
                assign(ilist.begin(), ilist.end());
            }
        // get_allocator:
            allocator_type get_allocator() const {
                return alloc_;
            };
        // element access:
            // at:
            reference at(size_type pos)
            {
                if (pos >= size_)
                    throw std::out_of_range("Vector::at");

                return *(data_+pos);
            };
            const_reference at(size_type pos) const
            {
                if (pos >= size_)
                    throw std::out_of_range("Vector::at");

                return data_[pos];
            }
            // operator[]:
            reference operator[](size_type pos)
            {
                return *(data_+pos);
            };
            const_reference operator[](size_type pos) const
            {
                return *(data_+pos);
            };
            // front:
            reference front()
            {
                return *data_;
            }
            const_reference front() const
            {
                return *data_;
            };
            // back:
            reference back()
            {
                return *(data_+(size_ - 1));
            };
            const_reference back() const
            {
                return *(data_+(size_ - 1));
            };
            // data:
            pointer data()
            {
                return data_;
            };
            const_pointer data() const
            {
                return data_;
            };
        // iterators:
            // begin:
            iterator begin()
            {
                return data_;
            };
            const_iterator begin() const
            {
                return data_;
            }
            // cbegin:
            const_iterator cbegin() const noexcept
            {
                return data_;
            };
            // end:
            iterator end() noexcept
            {
                return data_+size_;
            };
            const_iterator end() const noexcept
            {
                return data_ + size_;
            }
            // cend:
            const_iterator cend() const noexcept
            {
                return data_+size_;
            };
            // rbegin:
            reverse_iterator rbegin()
            {
                return reverse_iterator(end());
            };
            const_reverse_iterator rbegin() const
            {
                return reverse_iterator(end());
            };
            // crbegin:
            const_reverse_iterator crbegin() const noexcept
            {
                return const_reverse_iterator(cend());
            };
            // rend:
            reverse_iterator rend()
            {
                return reverse_iterator(begin());
            };
            const_reverse_iterator rend() const
            {
                return reverse_iterator(begin());
            };
            // crend:
            const_reverse_iterator crend() const noexcept
            {
                return const_reverse_iterator(cbegin());
            };

        // capacity:
            // empty:
            bool empty() const
            {
                return size_ == 0;
            };
            // size:
            size_type size() const
            {
                return size_;
            };
            // max_size:
            size_type max_size() const
            {
                return std::allocator_traits<Allocator>::max_size(alloc_);
            };
            // reserve:
            void reserve(size_type new_cap)
            {
                if(new_cap > max_size()) throw std::length_error("new capacity larger than maximum allowed size");
                if(new_cap <= capacity_)
                    return;
                
                reallocate(new_cap);
            };
            // capacity:
            size_type capacity() const
            {
                return capacity_;
            };
            // shrink_to_fit:
            void shrink_to_fit()
            {
                if(size_ < capacity_)
                {
                    if (size_ == 0)
                    {
                        std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
                        data_ = nullptr;
                        capacity_ = 0;
                    } 
                    else 
                    {
                        reallocate(size_);
                    }
                }
            }
        // modifiers:
            // clear:
            void clear()
            {
                for(size_type i = 0; i < size_; i++)
                {
                    std::allocator_traits<Allocator>::destroy(
                        alloc_,
                        data_ + i
                    );
                }

                size_ = 0;
            };
            // insert:
            iterator insert(const_iterator pos, const_reference value)
            {
                size_type index = static_cast<size_type>(pos - cbegin());
                if(size_ >= capacity_)
                {
                    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
                }
                shift_right(index, 1);
                std::allocator_traits<Allocator>::construct(
                    alloc_,
                    data_ + index,
                    value
                );
                ++size_;
                return begin() + index;
            }

            iterator insert(const_iterator pos, T&& value)
            {
                size_type index = static_cast<size_type>(pos - cbegin());
                if(size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
                shift_right(index, 1);
                std::allocator_traits<Allocator>::construct(
                    alloc_,
                    data_ + index,
                    std::move(value)
                );
                ++size_;
                return begin() + index;
            }

            iterator insert(const_iterator pos, size_type count, const_reference value)
            {
                if(count == 0) return begin() + (pos - cbegin());
                size_type index = static_cast<size_type>(pos - cbegin());
                if(size_ + count > capacity_) reserve(size_ + count);
                shift_right(index, count);
                for(size_type i = 0; i < count; ++i)
                    std::allocator_traits<Allocator>::construct(
                        alloc_,
                        data_+index+i,
                        value
                    );
                size_ += count;
                return begin() + index;
            }

            template <class InputIt, class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
            iterator insert(const_iterator pos, InputIt first, InputIt last)
            {
                size_type index = static_cast<size_type>(pos - cbegin());
                size_type count = static_cast<size_type>(std::distance(first, last));
                if(count == 0) return begin() + index;
                if(size_ + count > capacity_) reserve(size_ + count);
                shift_right(index, count);
                size_type i = 0;
                try{
                    for(; first != last; ++first, ++i)
                        std::allocator_traits<Allocator>::construct(
                            alloc_,
                            data_+index+i,
                            *first
                        );
                } catch(...) {
                    for(size_type j = 0; j < i; ++j)
                        std::allocator_traits<Allocator>::destroy(
                            alloc_, data_ + index + j
                        );
                    shift_left(index, count);
                    throw;
                }
                size_ += count;
                return begin() + index;
            }

            iterator insert(const_iterator pos, std::initializer_list<T> ilist)
            {
                return insert(pos, ilist.begin(), ilist.end());
            }
            // emplace
            template<class... Args>
            iterator emplace(const_iterator pos, Args&&... args)
            {
                size_type index = static_cast<size_type>(pos - cbegin());
                if(size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
                shift_right(index, 1);
                std::allocator_traits<Allocator>::construct(
                    alloc_,
                    data_+index,
                    std::forward<Args>(args)...
                );
                ++size_;
                return begin() + index;
            }
            // erase
            iterator erase(const_iterator pos)
            {
                size_type index = static_cast<size_type>(pos - cbegin());
                std::allocator_traits<Allocator>::destroy(alloc_, data_ + index);
                shift_left(index + 1, 1);
                --size_;
                return begin() + index;
            };

            iterator erase(const_iterator first, const_iterator last)
            {
                size_type index_first = static_cast<size_type>(first - cbegin());
                size_type index_last = static_cast<size_type>(last - cbegin());
                size_type count = index_last - index_first;
                for(size_type i = index_first; i < index_last; ++i)
                    std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
                shift_left(index_last, count);
                size_ -= count;
                return begin() + index_first;
            };
            // push_back
            void push_back(const_reference value)
            {
                if (size_ >= capacity_)
                {
                    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
                }

                std::allocator_traits<Allocator>::construct(
                    alloc_,
                    data_ + size_,
                    value
                );

                ++size_;
            };

            void push_back(value_type&& value)
            {
                if (size_ >= capacity_)
                {
                    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
                }

                std::allocator_traits<Allocator>::construct(
                    alloc_,
                    data_ + size_,
                    std::move(value)
                );

                ++size_;
            };
            // emplace_back
            template<class... Args >
            reference emplace_back(Args&&... args)
            {
                if(size_ >= capacity_)
                {
                    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
                }
                
                std::allocator_traits<Allocator>::construct(
                    alloc_, 
                    data_ + size_, 
                    std::forward<Args>(args)...
                );
                
                ++size_;
                return back();
            };
            // pop_back
            void pop_back()
            {
                assert(size_ > 0 && "pop_back called on empty Vector");
                --size_;
                std::allocator_traits<Allocator>::destroy(
                    alloc_,
                    data_ + size_
                );
            };
            // resize
            void resize(size_type count){
                if(count < size_)
                {
                    for(size_type i = count; i < size_; ++i){
                        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
                    }
                    size_ = count;
                } 
                else if(count > size_)
                {
                    if(count > capacity_)
                    {
                        size_type new_cap = capacity_;
                        if(new_cap == 0){
                            new_cap = 1;
                        }
                        while(new_cap < count){
                            new_cap *= 2;
                        }
                        reserve(new_cap);
                    }
                    for(size_type i = size_; i < count; ++i){
                        std::allocator_traits<Allocator>::construct(alloc_, data_ + i);
                    }
                    size_ = count;
                }
            }
            void resize(size_type count, const_reference value){
                if(count < size_)
                {
                    for(size_type i = count; i < size_; ++i){
                        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
                    }
                    size_ = count;
                } 
                else if(count > size_)
                {
                    if(count > capacity_)
                    {
                        size_type new_cap = capacity_;
                        if(new_cap == 0){
                            new_cap = 1;
                        }
                        while(new_cap < count){
                            new_cap *= 2;
                        }
                        reserve(new_cap);
                    }
                    for(size_type i = size_; i < count; ++i){
                        std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
                    }
                    size_ = count;
                }
            }
            // swap
            void swap(Vector& other) noexcept
            {
                std::swap(data_, other.data_);
                std::swap(size_, other.size_);
                std::swap(capacity_, other.capacity_);
                std::swap(alloc_, other.alloc_);
            }
    private:
        pointer data_ = nullptr;
        size_type size_ = 0;
        size_type capacity_ = 0;
        allocator_type alloc_;

        void reallocate(size_type new_cap)
        {
            pointer new_data =
                std::allocator_traits<Allocator>::allocate(
                    alloc_,
                    new_cap
                );
            
            size_type i = 0;
            try{
                for (; i < size_; ++i)
                {
                    std::allocator_traits<Allocator>::construct(
                        alloc_,
                        new_data + i,
                        std::move_if_noexcept(*(data_+i))
                    );
                }
            } catch(...) {
                for(size_type j = 0; j < i; ++j)
                {
                    std::allocator_traits<Allocator>::destroy(
                        alloc_,
                        new_data + j
                    );
                }
                std::allocator_traits<Allocator>::deallocate(
                    alloc_,
                    new_data,
                    new_cap
                );
                throw;
            }

            for(size_type i = 0; i < size_; ++i)
            {
                std::allocator_traits<Allocator>::destroy(
                    alloc_,
                    data_+i
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

        void shift_right(size_type index, size_type count)
        {
            for(size_type i = size_; i > index; --i)
            {
                std::allocator_traits<Allocator>::construct(
                    alloc_,
                    data_+i+count-1,
                    std::move_if_noexcept(*(data_+i-1))
                );
                std::allocator_traits<Allocator>::destroy(
                    alloc_,
                    data_+i-1);
            }
        }

        void shift_left(size_type index, size_type count)
        {
            for (size_type i = index; i < size_; ++i) {
                std::allocator_traits<Allocator>::construct(
                    alloc_,
                    data_ + i - count,
                    std::move_if_noexcept(data_[i])
                );
                std::allocator_traits<Allocator>::destroy(
                    alloc_,
                    data_ + i
                );
            }
        }
};

// non-member functions:
// operator==:
template<class T, class Alloc>
bool operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;
    for (typename Vector<T, Alloc>::size_type i = 0; i < lhs.size(); ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}
// operator<=>:
template<class T, class Alloc>
auto operator<=>(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
-> std::compare_three_way_result_t<T>
{
    for (typename Vector<T, Alloc>::size_type i = 0; i < lhs.size() && i < rhs.size(); ++i)
        if (auto cmp = lhs[i] <=> rhs[i]; cmp != 0)
            return cmp;
    return static_cast<std::compare_three_way_result_t<T>>(lhs.size() <=> rhs.size());
}
//std::swap(vector);
template<class T, class Alloc >
void swap(Vector<T, Alloc>& lhs, Vector<T, Alloc>& rhs)noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}
// erase
template<class T, class Alloc, class U>
typename Vector<T, Alloc>::size_type erase(Vector<T, Alloc>& c, const U& value)
{
    auto it = std::remove(c.begin(), c.end(), value);
    auto count = static_cast<typename Vector<T, Alloc>::size_type>(c.end() - it);
    c.erase(it, c.end());
    return count;
}
// erase_if
template<class T, class Alloc, class Pred>
typename Vector<T, Alloc>::size_type erase_if(Vector<T, Alloc>& c, Pred pred)
{
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto count = static_cast<typename Vector<T, Alloc>::size_type>(c.end() - it);
    c.erase(it, c.end());
    return count;
}

#endif