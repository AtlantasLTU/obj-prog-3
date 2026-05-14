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
                Vector(const Vector &other);
            // copy-assignment operator:
                Vector& operator=(const Vector&);
            // move constructor:
                Vector(Vector&&) noexcept;
            // move-assignment operator:
                Vector& operator=(Vector&&) noexcept;
        // destructors:
            ~Vector();
        // operator=:

        // assign:
        
        // assign_range, jei bus noro

        // get_allocator:

        // element access:
            // at:

            // operator[]:
            reference operator[](size_type pos);
            const_reference operator[](size_type pos) const;
            // front:
            reference front();
            const_reference front() const;
            // back:
            reference back();
            const_reference back() const;
            // data:
            pointer data();
            const_pointer data() const;
        // iterators:
            // begin:
            iterator begin();
            // cbegin:
            const_iterator cbegin() const;
            // end:
            iterator end();
            // cend:
            const_iterator cend() const;
            // rbegin:
            reverse_iterator rbegin();
            // crbegin:
            const_reverse_iterator crbegin() const;
            // rend:
            reverse_iterator rend();
            // crend:
            const_reverse_iterator crend() const;

        // capacity:
            // empty:
            bool empty() const;
            // size:
            size_type size() const;
            // max_size:

            // reserve:
            void reserve(size_type new_cap);
            // capacity:

            // shrink_to_fit:

        // modifiers:
            // clear:
            void clear();
            // insert:

            // insert_range jei bus noro

            // emplace

            // erase

            // push_back
            void push_back(const_reference value);
            void push_back(value_type&& value);
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