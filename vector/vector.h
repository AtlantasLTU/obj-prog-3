#include <memory>
#include <iterator>

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
        
        // assign_range, jei bus noro

        // get_allocator:

        // element access:
            // at:

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
            // cbegin:
            const_iterator cbegin() const
            {
                return data_;
            };
            // end:
            iterator end()
            {
                return data_+size_;
            };
            // cend:
            const_iterator cend() const
            {
                return data_+size_;
            };
            // rbegin:
            reverse_iterator rbegin()
            {
                return reverse_iterator(end());
            };
            // crbegin:
            const_reverse_iterator crbegin() const
            {
                return const_reverse_iterator(cend());
            };
            // rend:
            reverse_iterator rend()
            {
                return reverse_iterator(begin());
            };
            // crend:
            const_reverse_iterator crend() const
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
            }
            // max_size:

            // reserve:
            void reserve(size_type new_cap)
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
            };
            // capacity:

            // shrink_to_fit:

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

            // insert_range jei bus noro

            // emplace

            // erase

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

            // append_range

            // pop_back

            // resize

            // swap

    // non-member functions:
        // operator==:
        // operator<=>:
        //std::swap(vector);
        // erase
        // erase_if
    private:
        pointer data_ = nullptr;
        size_type size_ = 0;
        size_type capacity_ = 0;
        allocator_type alloc_;
};