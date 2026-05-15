/**
 * @file Vector.h
 * @brief Savarankiškai įgyvendintas C++20 reikalavimus atitinkantis dinaminis masyvas (vektorius).
 */

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

/**
 * @brief Dinaminio masyvo konteineris, atitinkantis std::vector sąsają (C++20).
 *
 * @tparam T       Elementų tipas.
 * @tparam Allocator Atminties paskirstymo tipo (numatytasis – std::allocator<T>).
 */
template <class T, class Allocator = std::allocator<T>>
class Vector {
public:
    // -------------------- tipų apibrėžimai --------------------
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

    // -------------------- konstruktoriai --------------------

    /**
     * @brief Sukuria tuščią vektorių.
     */
    Vector() : data_(nullptr), size_(0), capacity_(0), alloc_(Allocator{}) {}

    /**
     * @brief Sukuria vektorių su `count` numatytais elementais.
     *
     * @param count Elementų skaičius.
     * @param alloc Naudojamas atminties paskirstytojas.
     */
    explicit Vector(size_type count, const Allocator& alloc = Allocator{})
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        resize(count);
    }

    /**
     * @brief Sukuria vektorių su `count` kopijomis reikšmės `value`.
     *
     * @param count Elementų skaičius.
     * @param value Reikšmė, kuria užpildomi elementai.
     * @param alloc Naudojamas atminties paskirstytojas.
     */
    Vector(size_type count, const_reference value, const Allocator& alloc = Allocator{})
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        reserve(count);
        for (size_type i = 0; i < count; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
        }
        size_ = count;
    }

    /**
     * @brief Sukuria vektorių iš iteratorių intervalo [first, last).
     *
     * Šis konstruktorius neįgalinamas, jei InputIt yra integralus tipas (kad nebūtų painiojamas su count konstruktoriumi).
     *
     * @tparam InputIt Įvesties iteratoriaus tipas.
     * @param first Intervalo pradžia.
     * @param last  Intervalo pabaiga.
     * @param alloc Naudojamas atminties paskirstytojas.
     */
    template <class InputIt, class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
    Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator{})
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        size_type count = static_cast<size_type>(std::distance(first, last));
        reserve(count);
        for (size_type i = 0; first != last; ++first, ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, *first);
        }
        size_ = count;
    }

    /**
     * @brief Sukuria vektorių iš std::initializer_list.
     *
     * @param ilist Inicijavimo sąrašas.
     * @param alloc Naudojamas atminties paskirstytojas.
     */
    Vector(std::initializer_list<T> ilist, const Allocator& alloc = Allocator{})
        : Vector(ilist.begin(), ilist.end(), alloc) {}

    /**
     * @brief Kopijavimo konstruktorius.
     *
     * Sukuria vektorių, identišką `other`, naudodamas `other` paskirstytojo kopiją.
     *
     * @param other Kopijuojamas vektorius.
     */
    Vector(const Vector& other)
        : data_(nullptr), size_(0), capacity_(0), alloc_(other.alloc_) {
        reserve(other.size_);
        for (size_type i = 0; i < other.size_; i++) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, *(other.data_ + i));
        }
        size_ = other.size_;
    }

    /**
     * @brief Kopijavimo konstruktorius su nurodytu paskirstytoju.
     *
     * Sukuria vektorių, identišką `other`, tačiau naudoja duotą paskirstytoją `alloc`.
     *
     * @param other Kopijuojamas vektorius.
     * @param alloc Šiam vektoriui naudojamas paskirstytojas.
     */
    Vector(const Vector& other, const Allocator& alloc)
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        reserve(other.size_);
        for (size_type i = 0; i < other.size_; i++) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, *(other.data_ + i));
        }
        size_ = other.size_;
    }

    /**
     * @brief Kopijavimo priskyrimo operatorius.
     *
     * Pakeičia esamą turinį `other` kopija.
     *
     * @param other Kopijuojamas vektorius.
     * @return *this
     */
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            clear();
            if (data_) {
                std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
            }
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            reserve(other.size_);
            for (size_type i = 0; i < other.size_; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + i, *(other.data_ + i));
            }
            size_ = other.size_;
        }
        return *this;
    }

    /**
     * @brief Perkėlimo konstruktorius.
     *
     * Perima `other` vidinius išteklius, palikdamas `other` tuščią.
     *
     * @param other Perkeliamas vektorius.
     */
    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_), alloc_(std::move(other.alloc_)) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    /**
     * @brief Perkėlimo konstruktorius su nurodytu paskirstytoju.
     *
     * Jei pateiktas paskirstytojas `alloc` sutampa su `other` paskirstytoju, perima išteklius;
     * priešingu atveju atlieka perkėlimo konstrukciją elementas po elemento.
     *
     * @param other Perkeliamas vektorius.
     * @param alloc Naudojamas atminties paskirstytojas.
     */
    Vector(Vector&& other, const Allocator& alloc)
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        if (alloc == other.alloc_) {
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        } else {
            reserve(other.size_);
            for (size_type i = 0; i < other.size_; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + i, std::move(*(other.data_ + i)));
            }
            size_ = other.size_;
            other.clear();
        }
    }

    /**
     * @brief Priskiria std::initializer_list reikšmes.
     *
     * Pakeičia esamą turinį inicijavimo sąrašo elementais.
     *
     * @param ilist Inicijavimo sąrašas.
     * @return *this
     */
    Vector& operator=(std::initializer_list<T> ilist) {
        assign(ilist.begin(), ilist.end());
        return *this;
    }

    /**
     * @brief Perkėlimo priskyrimo operatorius.
     *
     * Atlaisvina esamus išteklius ir perima `other` vidinius išteklius.
     *
     * @param other Perkeliamas vektorius.
     * @return *this
     */
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            clear();
            if (data_) {
                std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
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

    /**
     * @brief Destruktorius.
     *
     * Sunaikina visus elementus ir atlaisvina atmintį.
     */
    ~Vector() {
        clear();
        if (data_) {
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
        }
    }

    // -------------------- priskyrimo metodai --------------------

    /**
     * @brief Pakeičia turinį `count` kopijomis reikšmės `value`.
     *
     * Jei `count` mažesnis už esamą dydį, pertekliniai elementai sunaikinami.
     * Jei reikia, perskirsto atmintį.
     *
     * @param count Naujas elementų skaičius.
     * @param value Reikšmė, kuria užpildomi elementai.
     */
    void assign(size_type count, const_reference value) {
        if (count > capacity_) reserve(count);
        if (count <= size_) {
            std::fill_n(data_, count, value);
            for (size_type i = count; i < size_; ++i) {
                std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
            }
        } else {
            std::fill_n(data_, size_, value);
            size_type i = size_;
            try {
                for (; i < count; ++i) {
                    std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
                }
            } catch (...) {
                for (size_type j = size_; j < i; ++j) {
                    std::allocator_traits<Allocator>::destroy(alloc_, data_ + j);
                }
                throw;
            }
        }
        size_ = count;
    }

    /**
     * @brief Pakeičia turinį elementais iš iteratorių intervalo [first, last).
     *
     * @tparam InputIt Įvesties iteratoriaus tipas.
     * @param first Intervalo pradžia.
     * @param last  Intervalo pabaiga.
     */
    template <class InputIt, class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
    void assign(InputIt first, InputIt last) {
        size_type new_size = static_cast<size_type>(std::distance(first, last));
        if (new_size > capacity_) reserve(new_size);
        size_type i = 0;
        for (; i < size_ && first != last; ++i, ++first) {
            *(data_ + i) = *first;
        }
        size_type constructed = i;
        try {
            for (; first != last; ++first, ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + i, *first);
            }
        } catch (...) {
            for (size_type j = constructed; j < i; ++j) {
                std::allocator_traits<Allocator>::destroy(alloc_, data_ + j);
            }
            size_ = constructed;
            throw;
        }
        for (size_type j = new_size; j < size_; ++j) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + j);
        }
        size_ = new_size;
    }

    /**
     * @brief Pakeičia turinį std::initializer_list elementais.
     *
     * @param ilist Inicijavimo sąrašas.
     */
    void assign(std::initializer_list<T> ilist) {
        assign(ilist.begin(), ilist.end());
    }

    /**
     * @brief Grąžina naudojamo atminties paskirstytojo kopiją.
     *
     * @return allocator_type
     */
    allocator_type get_allocator() const {
        return alloc_;
    }

    // -------------------- elementų prieiga --------------------

    /**
     * @brief Grąžina nuorodą į elementą pozicijoje `pos` su ribų tikrinimu.
     *
     * @param pos Elemento indeksas.
     * @return reference
     * @throw std::out_of_range Jei `pos >= size()`.
     */
    reference at(size_type pos) {
        if (pos >= size_) throw std::out_of_range("Vector::at");
        return *(data_ + pos);
    }

    /**
     * @brief Grąžina konstantinę nuorodą į elementą pozicijoje `pos` su ribų tikrinimu.
     *
     * @param pos Elemento indeksas.
     * @return const_reference
     * @throw std::out_of_range Jei `pos >= size()`.
     */
    const_reference at(size_type pos) const {
        if (pos >= size_) throw std::out_of_range("Vector::at");
        return data_[pos];
    }

    /**
     * @brief Grąžina nuorodą į elementą pozicijoje `pos` be ribų tikrinimo.
     *
     * @param pos Elemento indeksas.
     * @return reference
     */
    reference operator[](size_type pos) {
        return *(data_ + pos);
    }

    /**
     * @brief Grąžina konstantinę nuorodą į elementą pozicijoje `pos` be ribų tikrinimo.
     *
     * @param pos Elemento indeksas.
     * @return const_reference
     */
    const_reference operator[](size_type pos) const {
        return *(data_ + pos);
    }

    /**
     * @brief Grąžina nuorodą į pirmą elementą.
     *
     * @return reference
     */
    reference front() {
        return *data_;
    }

    /**
     * @brief Grąžina konstantinę nuorodą į pirmą elementą.
     *
     * @return const_reference
     */
    const_reference front() const {
        return *data_;
    }

    /**
     * @brief Grąžina nuorodą į paskutinį elementą.
     *
     * @return reference
     */
    reference back() {
        return *(data_ + (size_ - 1));
    }

    /**
     * @brief Grąžina konstantinę nuorodą į paskutinį elementą.
     *
     * @return const_reference
     */
    const_reference back() const {
        return *(data_ + (size_ - 1));
    }

    /**
     * @brief Grąžina rodyklę į vidinį duomenų masyvą.
     *
     * @return pointer
     */
    pointer data() {
        return data_;
    }

    /**
     * @brief Grąžina konstantinę rodyklę į vidinį duomenų masyvą.
     *
     * @return const_pointer
     */
    const_pointer data() const {
        return data_;
    }

    // -------------------- iteratorių palaikymas --------------------

    /**
     * @brief Grąžina iteratorių į pirmą elementą.
     * @return iterator
     */
    iterator begin() {
        return data_;
    }

    /**
     * @brief Grąžina konstantinį iteratorių į pirmą elementą.
     * @return const_iterator
     */
    const_iterator begin() const {
        return data_;
    }

    /**
     * @brief Grąžina konstantinį iteratorių į pirmą elementą (const vektoriams).
     * @return const_iterator
     */
    const_iterator cbegin() const noexcept {
        return data_;
    }

    /**
     * @brief Grąžina iteratorių į poziciją po paskutinio elemento.
     * @return iterator
     */
    iterator end() noexcept {
        return data_ + size_;
    }

    /**
     * @brief Grąžina konstantinį iteratorių į poziciją po paskutinio elemento.
     * @return const_iterator
     */
    const_iterator end() const noexcept {
        return data_ + size_;
    }

    /**
     * @brief Grąžina konstantinį iteratorių į poziciją po paskutinio elemento (const vektoriams).
     * @return const_iterator
     */
    const_iterator cend() const noexcept {
        return data_ + size_;
    }

    /**
     * @brief Grąžina atbulinį iteratorių į pirmą elementą atbuline tvarka (t.y. paskutinį).
     * @return reverse_iterator
     */
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    /**
     * @brief Grąžina konstantinį atbulinį iteratorių į pirmą elementą atbuline tvarka.
     * @return const_reverse_iterator
     */
    const_reverse_iterator rbegin() const {
        return reverse_iterator(end());
    }

    /**
     * @brief Grąžina konstantinį atbulinį iteratorių į pirmą elementą atbuline tvarka (const vektoriams).
     * @return const_reverse_iterator
     */
    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    /**
     * @brief Grąžina atbulinį iteratorių į poziciją prieš pirmą elementą atbuline tvarka.
     * @return reverse_iterator
     */
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    /**
     * @brief Grąžina konstantinį atbulinį iteratorių į poziciją prieš pirmą elementą atbuline tvarka.
     * @return const_reverse_iterator
     */
    const_reverse_iterator rend() const {
        return reverse_iterator(begin());
    }

    /**
     * @brief Grąžina konstantinį atbulinį iteratorių į poziciją prieš pirmą elementą atbuline tvarka (const vektoriams).
     * @return const_reverse_iterator
     */
    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    // -------------------- talpos metodai --------------------

    /**
     * @brief Patikrina, ar vektorius tuščias.
     * @return true jei size() == 0, kitaip false.
     */
    bool empty() const {
        return size_ == 0;
    }

    /**
     * @brief Grąžina elementų skaičių.
     * @return size_type
     */
    size_type size() const {
        return size_;
    }

    /**
     * @brief Grąžina maksimalų galimą elementų skaičių pagal paskirstytoją.
     * @return size_type
     */
    size_type max_size() const {
        return std::allocator_traits<Allocator>::max_size(alloc_);
    }

    /**
     * @brief Rezervuoja atmintį bent `new_cap` elementams.
     *
     * Jei `new_cap > capacity()`, perskirsto vidinį masyvą.
     *
     * @param new_cap Nauja talpa.
     * @throw std::length_error Jei `new_cap > max_size()`.
     */
    void reserve(size_type new_cap) {
        if (new_cap > max_size()) throw std::length_error("new capacity larger than maximum allowed size");
        if (new_cap <= capacity_) return;
        reallocate(new_cap);
    }

    /**
     * @brief Grąžina dabartinę talpą (kiek elementų galima sutalpinti be perskirstymo).
     * @return size_type
     */
    size_type capacity() const {
        return capacity_;
    }

    /**
     * @brief Sumažina talpą iki esamo dydžio, atlaisvindama nenaudojamą atmintį.
     */
    void shrink_to_fit() {
        if (size_ < capacity_) {
            if (size_ == 0) {
                std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
                data_ = nullptr;
                capacity_ = 0;
            } else {
                reallocate(size_);
            }
        }
    }

    // -------------------- modifikatoriai --------------------

    /**
     * @brief Sunaikina visus elementus; dydis tampa 0, talpa nesikeičia.
     */
    void clear() {
        for (size_type i = 0; i < size_; i++) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
        size_ = 0;
    }

    /**
     * @brief Įterpia `value` kopiją prieš `pos`.
     *
     * @param pos Konstantinis iteratoriuss, prieš kurį įterpti.
     * @param value Reikšmė.
     * @return Iteratorius į įterptą elementą.
     */
    iterator insert(const_iterator pos, const_reference value) {
        size_type index = static_cast<size_type>(pos - cbegin());
        if (size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        shift_right(index, 1);
        std::allocator_traits<Allocator>::construct(alloc_, data_ + index, value);
        ++size_;
        return begin() + index;
    }

    /**
     * @brief Įterpia `value` perkeltinai prieš `pos`.
     *
     * @param pos Konstantinis iteratoriuss, prieš kurį įterpti.
     * @param value Perkeliama reikšmė.
     * @return Iteratorius į įterptą elementą.
     */
    iterator insert(const_iterator pos, T&& value) {
        size_type index = static_cast<size_type>(pos - cbegin());
        if (size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        shift_right(index, 1);
        std::allocator_traits<Allocator>::construct(alloc_, data_ + index, std::move(value));
        ++size_;
        return begin() + index;
    }

    /**
     * @brief Įterpia `count` kopijų reikšmės `value` prieš `pos`.
     *
     * @param pos Konstantinis iteratoriuss, prieš kurį įterpti.
     * @param count Kopijų skaičius.
     * @param value Reikšmė.
     * @return Iteratorius į pirmą įterptą elementą (arba `pos`, jei `count == 0`).
     */
    iterator insert(const_iterator pos, size_type count, const_reference value) {
        if (count == 0) return begin() + (pos - cbegin());
        size_type index = static_cast<size_type>(pos - cbegin());
        if (size_ + count > capacity_) reserve(size_ + count);
        shift_right(index, count);
        for (size_type i = 0; i < count; ++i)
            std::allocator_traits<Allocator>::construct(alloc_, data_ + index + i, value);
        size_ += count;
        return begin() + index;
    }

    /**
     * @brief Įterpia elementus iš iteratorių intervalo [first, last) prieš `pos`.
     *
     * @tparam InputIt Įvesties iteratoriaus tipas.
     * @param pos Konstantinis iteratoriuss, prieš kurį įterpti.
     * @param first Intervalo pradžia.
     * @param last  Intervalo pabaiga.
     * @return Iteratorius į pirmą įterptą elementą (arba `pos`, jei `first == last`).
     */
    template <class InputIt, class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        size_type index = static_cast<size_type>(pos - cbegin());
        size_type count = static_cast<size_type>(std::distance(first, last));
        if (count == 0) return begin() + index;
        if (size_ + count > capacity_) reserve(size_ + count);
        shift_right(index, count);
        size_type i = 0;
        try {
            for (; first != last; ++first, ++i)
                std::allocator_traits<Allocator>::construct(alloc_, data_ + index + i, *first);
        } catch (...) {
            for (size_type j = 0; j < i; ++j)
                std::allocator_traits<Allocator>::destroy(alloc_, data_ + index + j);
            shift_left(index, count);
            throw;
        }
        size_ += count;
        return begin() + index;
    }

    /**
     * @brief Įterpia std::initializer_list elementus prieš `pos`.
     *
     * @param pos Konstantinis iteratoriuss, prieš kurį įterpti.
     * @param ilist Inicijavimo sąrašas.
     * @return Iteratorius į pirmą įterptą elementą.
     */
    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    }

    /**
     * @brief Sukuria elementą vietoje prieš `pos`, perduodant argumentus jo konstruktoriui.
     *
     * @tparam Args Argumentų tipai.
     * @param pos Konstantinis iteratoriuss, prieš kurį kurti.
     * @param args Argumentai elemento konstruktoriui.
     * @return Iteratorius į naują elementą.
     */
    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        size_type index = static_cast<size_type>(pos - cbegin());
        if (size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        shift_right(index, 1);
        std::allocator_traits<Allocator>::construct(alloc_, data_ + index, std::forward<Args>(args)...);
        ++size_;
        return begin() + index;
    }

    /**
     * @brief Pašalina elementą, į kurį rodo `pos`.
     *
     * @param pos Konstantinis iteratoriuss į šalinamą elementą.
     * @return Iteratorius į elementą, buvusį po pašalinto.
     */
    iterator erase(const_iterator pos) {
        size_type index = static_cast<size_type>(pos - cbegin());
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + index);
        shift_left(index + 1, 1);
        --size_;
        return begin() + index;
    }

    /**
     * @brief Pašalina elementus intervale [first, last).
     *
     * @param first Konstantinis iteratoriuss į pirmą šalinamą elementą.
     * @param last  Konstantinis iteratoriuss po paskutinio šalinamo elemento.
     * @return Iteratorius į elementą, buvusį po pašalintų.
     */
    iterator erase(const_iterator first, const_iterator last) {
        size_type index_first = static_cast<size_type>(first - cbegin());
        size_type index_last = static_cast<size_type>(last - cbegin());
        size_type count = index_last - index_first;
        for (size_type i = index_first; i < index_last; ++i)
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        shift_left(index_last, count);
        size_ -= count;
        return begin() + index_first;
    }

    /**
     * @brief Prideda `value` kopiją į vektoriaus pabaigą.
     *
     * Jei nėra pakankamai talpos, perskirsto atmintį.
     *
     * @param value Pridedama reikšmė.
     */
    void push_back(const_reference value) {
        if (size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, value);
        ++size_;
    }

    /**
     * @brief Prideda `value` perkeltinai į vektoriaus pabaigą.
     *
     * @param value Perkeliama reikšmė.
     */
    void push_back(value_type&& value) {
        if (size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, std::move(value));
        ++size_;
    }

    /**
     * @brief Sukuria elementą vektoriaus pabaigoje, perduodant argumentus jo konstruktoriui.
     *
     * @tparam Args Argumentų tipai.
     * @param args Argumentai elemento konstruktoriui.
     * @return Nuoroda į naujai sukurtą elementą.
     */
    template <class... Args>
    reference emplace_back(Args&&... args) {
        if (size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
        ++size_;
        return back();
    }

    /**
     * @brief Pašalina paskutinį vektoriaus elementą.
     *
     * @note Vektorius turi būti netuščias.
     */
    void pop_back() {
        assert(size_ > 0 && "pop_back called on empty Vector");
        --size_;
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + size_);
    }

    /**
     * @brief Pakeičia vektoriaus dydį į `count`.
     *
     * Jei `count > size()`, pridedami numatyti elementai.
     * Jei `count < size()`, elementai pašalinami nuo galo.
     *
     * @param count Naujas dydis.
     */
    void resize(size_type count) {
        if (count < size_) {
            for (size_type i = count; i < size_; ++i) {
                std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
            }
            size_ = count;
        } else if (count > size_) {
            if (count > capacity_) {
                size_type new_cap = capacity_;
                if (new_cap == 0) new_cap = 1;
                while (new_cap < count) new_cap *= 2;
                reserve(new_cap);
            }
            for (size_type i = size_; i < count; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + i);
            }
            size_ = count;
        }
    }

    /**
     * @brief Pakeičia vektoriaus dydį į `count`, užpildant naujus elementus reikšme `value`.
     *
     * @param count Naujas dydis.
     * @param value Reikšmė, kuria užpildomi nauji elementai.
     */
    void resize(size_type count, const_reference value) {
        if (count < size_) {
            for (size_type i = count; i < size_; ++i) {
                std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
            }
            size_ = count;
        } else if (count > size_) {
            if (count > capacity_) {
                size_type new_cap = capacity_;
                if (new_cap == 0) new_cap = 1;
                while (new_cap < count) new_cap *= 2;
                reserve(new_cap);
            }
            for (size_type i = size_; i < count; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
            }
            size_ = count;
        }
    }

    /**
     * @brief Sukeičia turinį su `other` vektoriumi.
     *
     * @param other Kitas vektorius.
     */
    void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(alloc_, other.alloc_);
    }

private:
    pointer data_ = nullptr;       ///< Rodyklė į elementų masyvą.
    size_type size_ = 0;           ///< Esamas elementų skaičius.
    size_type capacity_ = 0;       ///< Rezervuota atmintis (elementais).
    allocator_type alloc_;         ///< Atminties paskirstytojas.

    /**
     * @brief Perskirsto vidinį masyvą į `new_cap` dydžio buferį.
     *
     * Perkelia visus esamus elementus naudodamas `move_if_noexcept`, sunaikina senus ir atlaisvina buvusį buferį.
     *
     * @param new_cap Nauja talpa.
     */
    void reallocate(size_type new_cap) {
        pointer new_data = std::allocator_traits<Allocator>::allocate(alloc_, new_cap);
        size_type i = 0;
        try {
            for (; i < size_; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, new_data + i,
                                                            std::move_if_noexcept(*(data_ + i)));
            }
        } catch (...) {
            for (size_type j = 0; j < i; ++j) {
                std::allocator_traits<Allocator>::destroy(alloc_, new_data + j);
            }
            std::allocator_traits<Allocator>::deallocate(alloc_, new_data, new_cap);
            throw;
        }
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
        if (data_) {
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
        }
        data_ = new_data;
        capacity_ = new_cap;
    }

    /**
     * @brief Pastumia elementus į dešinę nuo `index`, atlaisvindama `count` pozicijų.
     *
     * Elementai perkeliami naudojant `move_if_noexcept`.
     *
     * @param index Pozicija, nuo kurios elementai stumiami.
     * @param count Laisvų pozicijų skaičius.
     */
    void shift_right(size_type index, size_type count) {
        for (size_type i = size_; i > index; --i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i + count - 1,
                                                        std::move_if_noexcept(*(data_ + i - 1)));
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i - 1);
        }
    }

    /**
     * @brief Pastumia elementus į kairę nuo `index`, uždengdama `count` pozicijų.
     *
     * Elementai perkeliami naudojant `move_if_noexcept`.
     *
     * @param index Pozicija, nuo kurios pradedama stumti.
     * @param count Pašalintų pozicijų skaičius.
     */
    void shift_left(size_type index, size_type count) {
        for (size_type i = index; i < size_; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i - count,
                                                        std::move_if_noexcept(data_[i]));
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
    }
};

// -------------------- ne narių funkcijos --------------------

/**
 * @brief Palygina du vektorius lygybei.
 *
 * @tparam T Elemento tipas.
 * @tparam Alloc Paskirstytojas.
 * @param lhs Pirmasis vektorius.
 * @param rhs Antrasis vektorius.
 * @return true jei abiejų dydžiai lygūs ir visi elementai lygūs, kitaip false.
 */
template <class T, class Alloc>
bool operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    for (typename Vector<T, Alloc>::size_type i = 0; i < lhs.size(); ++i)
        if (lhs[i] != rhs[i]) return false;
    return true;
}

/**
 * @brief Atlieka trijų krypčių palyginimą (C++20).
 *
 * @tparam T Elemento tipas.
 * @tparam Alloc Paskirstytojas.
 * @param lhs Pirmasis vektorius.
 * @param rhs Antrasis vektorius.
 * @return Rezultatas pagal <=> operatorių.
 */
template <class T, class Alloc>
auto operator<=>(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
    -> std::compare_three_way_result_t<T> {
    for (typename Vector<T, Alloc>::size_type i = 0; i < lhs.size() && i < rhs.size(); ++i)
        if (auto cmp = lhs[i] <=> rhs[i]; cmp != 0) return cmp;
    return static_cast<std::compare_three_way_result_t<T>>(lhs.size() <=> rhs.size());
}

/**
 * @brief Sukeičia dviejų vektorių turinius (specializacija std::swap).
 *
 * @tparam T Elemento tipas.
 * @tparam Alloc Paskirstytojas.
 * @param lhs Pirmasis vektorius.
 * @param rhs Antrasis vektorius.
 */
template <class T, class Alloc>
void swap(Vector<T, Alloc>& lhs, Vector<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

/**
 * @brief Pašalina visus elementus, lygius `value`.
 *
 * @tparam T Elemento tipas.
 * @tparam Alloc Paskirstytojas.
 * @tparam U Palyginamos reikšmės tipas.
 * @param c Vektorius, iš kurio šalinama.
 * @param value Reikšmė, su kuria lyginama.
 * @return Pašalintų elementų skaičius.
 */
template <class T, class Alloc, class U>
typename Vector<T, Alloc>::size_type erase(Vector<T, Alloc>& c, const U& value) {
    auto it = std::remove(c.begin(), c.end(), value);
    auto count = static_cast<typename Vector<T, Alloc>::size_type>(c.end() - it);
    c.erase(it, c.end());
    return count;
}

/**
 * @brief Pašalina visus elementus, tenkinančius predikatą `pred`.
 *
 * @tparam T Elemento tipas.
 * @tparam Alloc Paskirstytojas.
 * @tparam Pred Predikato tipas.
 * @param c Vektorius, iš kurio šalinama.
 * @param pred Predikatas, grąžinantis true šalinamiems elementams.
 * @return Pašalintų elementų skaičius.
 */
template <class T, class Alloc, class Pred>
typename Vector<T, Alloc>::size_type erase_if(Vector<T, Alloc>& c, Pred pred) {
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto count = static_cast<typename Vector<T, Alloc>::size_type>(c.end() - it);
    c.erase(it, c.end());
    return count;
}

#endif // VECTOR_H