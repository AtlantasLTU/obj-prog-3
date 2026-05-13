#include <memory>
#include <iterator>

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
        // destructors:

        // operator=:

        // assign:
        
        // assign_range, jei bus noro

        // get_allocator:

        // element access:
            // at:

            // operator[]:

            // front:

            // back:
            
            // data:

        // iterators:
            // begin:

            // cbegin:

            // end:

            // cend:

            // rbegin:

            // crbegin:

            // rend:

            // crend:

        // capacity:
            // empty:

            // size:

            // max_size:

            // reserve:

            // capacity:

            // shrink_to_fit:

        // modifiers:
            // clear:

            // insert:

            // insert_range jei bus noro

            // emplace

            // erase

            // push_back

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