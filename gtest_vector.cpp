#include <gtest/gtest.h>
#include "./vector/vector.h"               // your header
#include <string>
#include <type_traits>
#include <stdexcept>
#include <utility>

// -------------------- Constructors & Destructor --------------------
TEST(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.data(), nullptr);
}

TEST(VectorTest, SizeConstructor) {
    Vector<int> v(5);
    EXPECT_EQ(v.size(), 5u);
    EXPECT_GE(v.capacity(), 5u);
    for (std::size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], 0);          // value-initialised
}

TEST(VectorTest, SizeValueConstructor) {
    Vector<int> v(4, 42);
    EXPECT_EQ(v.size(), 4u);
    for (std::size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], 42);
}

TEST(VectorTest, RangeConstructor) {
    std::string s = "hello";
    Vector<char> v(s.begin(), s.end());
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[0], 'h');
    EXPECT_EQ(v[4], 'o');
}

TEST(VectorTest, InitializerListConstructor) {
    Vector<int> v{10, 20, 30};
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorTest, CopyConstructor) {
    Vector<int> original{1, 2, 3};
    Vector<int> copy(original);
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
    // Ensure deep copy
    copy[0] = 99;
    EXPECT_EQ(original[0], 1);
}

TEST(VectorTest, CopyConstructorWithAllocator) {
    Vector<int> original{5, 6, 7};
    Vector<int> copy(original, std::allocator<int>{});
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(copy[0], 5);
}

TEST(VectorTest, MoveConstructor) {
    Vector<int> original{1, 2, 3};
    Vector<int> moved(std::move(original));
    EXPECT_EQ(moved.size(), 3u);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(original.size(), 0u);
    EXPECT_EQ(original.data(), nullptr);
}

TEST(VectorTest, MoveConstructorWithSameAllocator) {
    std::allocator<int> alloc;
    Vector<int> original({7,8,9}, alloc);
    Vector<int> moved(std::move(original), alloc);
    EXPECT_EQ(moved.size(), 3u);
    EXPECT_EQ(original.data(), nullptr);  // pointers swapped
}

TEST(VectorTest, MoveConstructorWithDifferentAllocator) {
    // std::allocator<int> always compares equal, but we can still test the path
    // The code uses `if(alloc == other.alloc_)` – with equal allocators it swaps;
    // with unequal it moves elements. Both paths are functionally correct.
    Vector<int> original{10, 11, 12};
    Vector<int> moved(std::move(original), std::allocator<int>{});
    EXPECT_EQ(moved.size(), 3u);
    EXPECT_EQ(original.size(), 0u);   // original cleared
    EXPECT_EQ(moved[0], 10);
}

TEST(VectorTest, DestructorDeallocates) {
    // Just ensure no memory leak / double free by letting v go out of scope.
    Vector<std::string> v;
    v.push_back("test");
    // No assertion – ASan would catch leaks.
}

// -------------------- Assignment Operators --------------------
TEST(VectorTest, CopyAssignment) {
    Vector<int> src{1, 2, 3};
    Vector<int> dst{4, 5};
    dst = src;
    EXPECT_EQ(dst.size(), 3u);
    EXPECT_EQ(dst[0], 1);
    EXPECT_EQ(dst[2], 3);
    // self-assignment
    dst = dst;
    EXPECT_EQ(dst.size(), 3u);
}

TEST(VectorTest, MoveAssignment) {
    Vector<int> src{10, 20, 30};
    Vector<int> dst{40};
    dst = std::move(src);
    EXPECT_EQ(dst.size(), 3u);
    EXPECT_EQ(dst[0], 10);
    EXPECT_EQ(src.size(), 0u);
    // self-move-assignment (should be safe)
    dst = std::move(dst);
    EXPECT_EQ(dst.size(), 3u);
}

TEST(VectorTest, InitializerListAssignment) {
    Vector<int> v{1};
    v = {7, 8, 9, 10};
    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[0], 7);
    EXPECT_EQ(v[3], 10);
}

// -------------------- assign --------------------
TEST(VectorTest, AssignCountValue) {
    Vector<int> v;
    v.assign(3, 42);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 42);
    EXPECT_EQ(v[2], 42);

    v.assign(2, 99);       // smaller count
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 99);
}

TEST(VectorTest, AssignRange) {
    Vector<int> v{1, 2, 3};
    int arr[] = {10, 20, 30, 40};
    v.assign(std::begin(arr), std::end(arr));
    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[3], 40);
}

TEST(VectorTest, AssignInitializerList) {
    Vector<int> v;
    v.assign({5, 6, 7});
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 5);
}

// -------------------- get_allocator --------------------
TEST(VectorTest, GetAllocator) {
    Vector<int> v;
    auto alloc = v.get_allocator();
    (void)alloc;   // just compiles
    SUCCEED();
}

// -------------------- Element Access --------------------
TEST(VectorTest, At) {
    Vector<int> v{10, 20, 30};
    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(2), 30);
    EXPECT_THROW(v.at(3), std::out_of_range);
    const auto& cv = v;
    EXPECT_EQ(cv.at(1), 20);
    EXPECT_THROW(cv.at(3), std::out_of_range);
}

TEST(VectorTest, SubscriptOperator) {
    Vector<int> v{1, 2, 3};
    v[1] = 99;
    EXPECT_EQ(v[1], 99);
    const auto& cv = v;
    EXPECT_EQ(cv[0], 1);
}

TEST(VectorTest, FrontBack) {
    Vector<int> v{7, 8, 9};
    EXPECT_EQ(v.front(), 7);
    EXPECT_EQ(v.back(), 9);
    v.front() = 100;
    EXPECT_EQ(v.front(), 100);
    const auto& cv = v;
    EXPECT_EQ(cv.front(), 100);
    EXPECT_EQ(cv.back(), 9);
}

TEST(VectorTest, Data) {
    Vector<int> v{1, 2, 3};
    int* p = v.data();
    EXPECT_EQ(p[0], 1);
    p[1] = 77;
    EXPECT_EQ(v[1], 77);
    const auto& cv = v;
    const int* cp = cv.data();
    EXPECT_EQ(cp[2], 3);
}

// -------------------- Iterators --------------------
TEST(VectorTest, BeginEnd) {
    Vector<int> v{5, 6, 7};
    EXPECT_EQ(*v.begin(), 5);
    EXPECT_EQ(*(v.end() - 1), 7);
    EXPECT_EQ(v.end() - v.begin(), 3);
    const auto& cv = v;
    EXPECT_EQ(*cv.begin(), 5);
    EXPECT_EQ(cv.end() - cv.begin(), 3);
}

TEST(VectorTest, CBeginCEnd) {
    Vector<int> v{1, 2, 3};
    EXPECT_EQ(*v.cbegin(), 1);
    EXPECT_EQ(*(v.cend() - 1), 3);
}

TEST(VectorTest, ReverseIterators) {
    Vector<int> v{10, 20, 30};
    EXPECT_EQ(*v.rbegin(), 30);
    EXPECT_EQ(*(v.rend() - 1), 10);
    const auto& cv = v;
    EXPECT_EQ(*cv.crbegin(), 30);
    EXPECT_EQ(*(cv.crend() - 1), 10);
}

// -------------------- Capacity --------------------
TEST(VectorTest, Empty) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    v.push_back(1);
    EXPECT_FALSE(v.empty());
}

TEST(VectorTest, Size) {
    Vector<int> v{1, 2, 3};
    EXPECT_EQ(v.size(), 3u);
}

TEST(VectorTest, MaxSize) {
    Vector<int> v;
    EXPECT_GT(v.max_size(), 0u);
}

TEST(VectorTest, Reserve) {
    Vector<int> v;
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100u);
    EXPECT_EQ(v.size(), 0u);   // reserve does not change size
    // check length_error on excessive reserve
    EXPECT_THROW(v.reserve(v.max_size() + 1), std::length_error);
}

TEST(VectorTest, Capacity) {
    Vector<int> v;
    EXPECT_EQ(v.capacity(), 0u);
    v.reserve(10);
    EXPECT_EQ(v.capacity(), 10u);
}

TEST(VectorTest, ShrinkToFit) {
    Vector<int> v{1, 2, 3};
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100u);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());   // capacity == size
    // test empty vector shrink
    Vector<int> empty;
    empty.reserve(50);
    empty.shrink_to_fit();
    EXPECT_EQ(empty.capacity(), 0u);
}

// -------------------- Modifiers --------------------
TEST(VectorTest, Clear) {
    Vector<int> v{1, 2, 3};
    v.clear();
    EXPECT_EQ(v.size(), 0u);
    EXPECT_GT(v.capacity(), 0u);  // capacity unchanged
}

TEST(VectorTest, InsertSingleLvalue) {
    Vector<int> v{10, 20, 30};
    auto it = v.insert(v.cbegin() + 1, 99);
    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[1], 99);
    EXPECT_EQ(*it, 99);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[3], 30);
}

TEST(VectorTest, InsertSingleRvalue) {
    Vector<std::string> v{"hello", "world"};
    std::string tmp = "!!!";
    v.insert(v.cbegin() + 1, std::move(tmp));
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1], "!!!");
    EXPECT_TRUE(tmp.empty()); // moved-from
}

TEST(VectorTest, InsertCountValue) {
    Vector<int> v{1, 2};
    auto it = v.insert(v.cbegin() + 1, 3, 0);
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[1], 0);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(*it, 0);
    EXPECT_EQ(v[4], 2);
}

TEST(VectorTest, InsertRange) {
    Vector<int> v{1, 5};
    int arr[] = {2, 3, 4};
    auto it = v.insert(v.cbegin() + 1, std::begin(arr), std::end(arr));
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[4], 5);
    EXPECT_EQ(*it, 2);
}

TEST(VectorTest, InsertInitializerList) {
    Vector<int> v{1, 5};
    auto it = v.insert(v.cbegin() + 1, {2, 3, 4});
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[3], 4);
}

TEST(VectorTest, Emplace) {
    Vector<std::string> v{"start", "end"};
    auto it = v.emplace(v.cbegin() + 1, "middle!");
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1], "middle!");
    EXPECT_EQ(*it, "middle!");
}

TEST(VectorTest, EraseSingle) {
    Vector<int> v{10, 20, 30};
    auto it = v.erase(v.cbegin() + 1);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 30);
    EXPECT_EQ(*it, 30);   // returns iterator to next element
}

TEST(VectorTest, EraseRange) {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.erase(v.cbegin() + 1, v.cbegin() + 3);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 4);
    EXPECT_EQ(v[2], 5);
    EXPECT_EQ(*it, 4);
}

TEST(VectorTest, PushBackLvalue) {
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
}

TEST(VectorTest, PushBackRvalue) {
    Vector<std::string> v;
    std::string s = "temporary";
    v.push_back(std::move(s));
    EXPECT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], "temporary");
    EXPECT_TRUE(s.empty());
}

TEST(VectorTest, EmplaceBack) {
    Vector<std::pair<int, std::string>> v;
    auto& ref = v.emplace_back(42, "hello");
    EXPECT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0].first, 42);
    EXPECT_EQ(v[0].second, "hello");
    EXPECT_EQ(&ref, &v.back());
}

TEST(VectorTest, PopBack) {
    Vector<int> v{1, 2, 3};
    v.pop_back();
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[1], 2);
    // do not test pop_back on empty – debug assert would fire.
}

TEST(VectorTest, ResizeCount) {
    Vector<int> v{1, 2, 3};
    v.resize(5);
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 0);
    v.resize(2);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST(VectorTest, ResizeCountValue) {
    Vector<int> v{10, 20};
    v.resize(4, 99);
    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[2], 99);
    EXPECT_EQ(v[3], 99);
    v.resize(1, 0);
    EXPECT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], 10);
}

TEST(VectorTest, SwapMember) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{4, 5};
    a.swap(b);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a[0], 4);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[2], 3);
}

// -------------------- Non‑member Functions --------------------
TEST(VectorTest, EqualityOperator) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 2, 3};
    Vector<int> v3{1, 2};
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}

TEST(VectorTest, SpaceshipOperator) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 4};
    Vector<int> c{1, 2};
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_GT(a, c);
    EXPECT_EQ(a <=> a, std::strong_ordering::equal);
}

TEST(VectorTest, NonMemberSwap) {
    Vector<int> a{10, 20};
    Vector<int> b{30, 40, 50};
    using std::swap;
    swap(a, b);
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a[0], 30);
    EXPECT_EQ(b.size(), 2u);
    EXPECT_EQ(b[1], 20);
}

TEST(VectorTest, EraseValue) {
    Vector<int> v{1, 2, 3, 2, 4};
    auto removed = erase(v, 2);
    EXPECT_EQ(removed, 2u);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 4);
}

TEST(VectorTest, EraseIf) {
    Vector<int> v{1, 2, 3, 4, 5};
    auto removed = erase_if(v, [](int x) { return x % 2 == 0; });
    EXPECT_EQ(removed, 2u);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 5);
}

// ------------ Additional edge case tests ------------
TEST(VectorTest, LargeInsert) {
    Vector<int> v;
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);
    EXPECT_EQ(v.size(), 1000u);
    EXPECT_EQ(v.front(), 0);
    EXPECT_EQ(v.back(), 999);
}

TEST(VectorTest, InsertAtEnd) {
    Vector<int> v{1, 2};
    auto it = v.insert(v.cend(), 3);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(it, v.begin() + 2);
}

TEST(VectorTest, EraseAllElements) {
    Vector<int> v{1, 2, 3};
    v.erase(v.cbegin(), v.cend());
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0u);
}

// Custom type to test correct construction/destruction
struct Counted {
    inline static int alive = 0;
    int value;
    Counted(int v) : value(v) { ++alive; }
    Counted(const Counted& o) : value(o.value) { ++alive; }
    Counted(Counted&& o) noexcept : value(o.value) { o.value = -1; ++alive; }
    ~Counted() { --alive; }
};

TEST(VectorTest, CountedElementLifecycle) {
    {
        Vector<Counted> v;
        v.emplace_back(1);
        v.emplace_back(2);
        EXPECT_EQ(Counted::alive, 2);
        v.pop_back();
        EXPECT_EQ(Counted::alive, 1);
    }
    EXPECT_EQ(Counted::alive, 0);
}