#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>   // std::random_access_iterator_tag
#include <cstddef>     // size_t
#include <stdexcept>   // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector
{
public:
    class iterator;

private:
    T *array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow()
    { /* TODO */
        // gotta grow the capacity by one if the original vector is zero
        if (this->_capacity == 0)
        {
            // size_t newCapacity = 1;
            // size_t newSize = 1;
            T *nArr = new T[1];
            delete[] this->array;
            this->array = nArr;
            this->_capacity = 1;
        }
        else
        {
            this->_capacity = this->_capacity * 2;
            T *nArr = new T[this->_capacity];
            for (size_t i = 0; i < this->_size; ++i)
            {
                nArr[i] = std::move(array[i]);
            }
            delete[] this->array;
            this->array = nArr;
        }
    }

public:
    Vector() noexcept : _capacity(0), _size(0), array(nullptr)
    {
    }
    Vector(size_t count, const T &value) : _capacity(count), _size(count), array(new T[count])
    {
        for (size_t index = 0; index < count; ++index)
        {
            array[index] = value;
        }
    }
    explicit Vector(size_t count) : _capacity(count), _size(count), array(new T[count]{})
    {
        // for (size_t i = 0; i < count; ++i)
        // {
        //     array[i] = NULL;
        // }
    }
    Vector(const Vector &other)
    {
        this->_capacity = other._capacity;
        this->_size = other._size;
        this->array = new T[this->_capacity];

        for (size_t i = 0; i < this->_size; ++i)
        {
            // array[i] = other.array[i];
            array[i] = other[i];
        }
    }
    Vector(Vector &&other) noexcept
    {
        // this->_capacity = other._capacity;
        // this->_size = other._size;
        // this->array = new T[this->_capacity];
        // // this->array = std::move(other.array);
        // for (size_t i = 0; i < this->_size; ++i)
        // {
        //     array[i] = std::move(other.array[i]);
        // }
        // delete[] other.array;
        // other.array = this->array;
        // // make other empty
        // other._capacity = 0;
        // other._size = 0;
        // other.array = nullptr;
        this->_capacity = other._capacity;
        this->_size = other._size;
        this->array = std::move(other.array);
        // make other empty
        other._capacity = 0;
        other._size = 0;
        other.array = nullptr;
    }

    ~Vector()
    {
        if (this->array != nullptr)
            delete[] this->array;
        this->array = nullptr;
    }

    Vector &operator=(const Vector &other)
    {

        // NEED TO IMPLEMENT CLEAR TO CLEAR VECTOR BEFORE REASSIGNING ALL THE VALUES
        if (this != &other)
        {

            clear();
            this->_capacity = other._capacity;
            this->_size = other._size;
            T *nArr = new T[this->_capacity];
            for (size_t idx = 0; idx < other._size; ++idx)
            {
                nArr[idx] = other[idx];
            }
            // delete and reappoint accordingly
            delete[] this->array;
            this->array = nArr;
        }
        // other.array = nullptr;
        return *this;
    }
    Vector &operator=(Vector &&other) noexcept
    { /* TODO */
        if (this != &other)
        {
            clear();
            this->array = other.array;
            this->_capacity = other._capacity;
            this->_size = other._size;
            // T *nArr = new T[this->_capacity];
            // for (size_t idx = 0; idx < other._size; ++idx)
            // {
            //     nArr[idx] = std::move(other[idx]);
            // }
            // // delete and re point accordingly
            // delete[] this->array;
            // this->array = nArr;

            // delete[] other.array;
            other.array = nullptr;
            other._capacity = 0;
            other._size = 0;
        }
        // NEED TO TO EMPTY OUT THE CONTENTS OF THE OTHER VECTOR
        return *this;
    }

    iterator begin() noexcept
    {
        iterator itr(array);

        return itr;
    }
    iterator end() noexcept
    {
        iterator itr(array + this->_size);
        return itr;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        if (this->_size == 0)
            return true;
        return false;
    }
    size_t size() const noexcept
    {
        return this->_size;
    }
    size_t capacity() const noexcept
    {
        return this->_capacity;
    }

    T &at(size_t pos)
    {
        if (pos >= this->_size)
            throw std::out_of_range("");
        else
            return *(array + pos);
    }
    const T &at(size_t pos) const
    {
        if (pos >= this->_size)
            throw std::out_of_range("");
        else
            return *(array + pos);
    }
    T &operator[](size_t pos)
    {

        return *(array + pos);
    }
    const T &operator[](size_t pos) const
    {
        // const T result;
        // result = *(array + pos);

        return *(array + pos);
    }
    T &front()
    {
        return *(array);
    }
    const T &front() const
    {
        return *(array);
    }
    T &back()
    {
        return *(array + _size - 1);
    }
    const T &back() const
    {
        return *(array + _size - 1);
    }

    void push_back(const T &value)
    {
        if (this->_size == this->_capacity)
        {
            this->grow();
            array[this->_size] = value;
            this->_size += 1;
        }
        else
        {
            array[this->_size] = value;
            this->_size += 1;
        }
    }
    void push_back(T &&value)
    {
        if (this->_size == this->_capacity)
        {
            this->grow();
            array[this->_size] = std::move(value);
            this->_size += 1;
        }
        else
        {
            array[this->_size] = std::move(value);
            this->_size += 1;
        }
    }
    void pop_back()
    {
        if (_size != 0)
        {
            array[_size - 1] = {};
            _size -= 1;
        }
    }

    iterator insert(iterator pos, const T &value)
    {                                         /* TODO */
        ptrdiff_t insert_idx = pos - begin(); // know exactly where to insert the desired value
        if (_size == _capacity)
            grow(); // need to check if it's within bounds

        // essentially starting at the back and shifting every value over one until we get to the insert index
        // to open up space for the value at the desired index
        for (ptrdiff_t index = _size; index > insert_idx; --index)
        {
            array[index] = std::move(array[index - 1]);
        }
        array[insert_idx] = value;
        ++_size;
        return begin() + insert_idx;
    }
    iterator insert(iterator pos, T &&value)
    {                                         /* TODO */
        ptrdiff_t insert_idx = pos - begin(); // know exactly where to insert the desired value
        if (_size == _capacity)
            grow(); // need to check if it's within bounds

        // essentially starting at the back and shifting every value over one until we get to the insert index
        // to open up space for the value at the desired index
        for (ptrdiff_t index = _size; index > insert_idx; --index)
        {
            array[index] = std::move(array[index - 1]);
        }
        array[insert_idx] = std::move(value);
        ++_size;
        return begin() + insert_idx;
    }
    iterator insert(iterator pos, size_t count, const T &value)
    { /* TODO */
        if (count == 0)
            return pos;
        ptrdiff_t insert_idx = pos - begin();
        while (_size + count > _capacity)
            grow(); // to do multiple times if necessary rather than just once
        for (ptrdiff_t index = _size + count - 1; index > insert_idx + count - 1; --index)
        {
            array[index] = std::move(array[index - count]);
        }

        for (size_t index = insert_idx; index < insert_idx + count; ++index)
        {
            array[index] = value;
        }

        _size += count;
        return begin() + insert_idx;
    }

    iterator erase(iterator pos)
    { /* TODO */
        ptrdiff_t dltIdx = pos - begin();
        // array[dltIdx] = nullptr;
        for (ptrdiff_t index = dltIdx; index < _size - 1; ++index)
        {
            array[index] = std::move(array[index + 1]);
        }
        _size -= 1;
        return begin() + dltIdx;
    }
    iterator erase(iterator first, iterator last)
    {
        ptrdiff_t startIdx = first - begin();
        ptrdiff_t endIdx = last - begin();
        ptrdiff_t shiftIdx = endIdx - startIdx;
        for (ptrdiff_t index = startIdx; index < _size; ++index)
        {
            if ((index + shiftIdx) < _size)
            {
                array[index] = std::move(array[index + shiftIdx]);
            }
            else
                array[index] = {};
        }

        _size -= shiftIdx;
        return begin() + startIdx;
    }

    class iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T *;
        using reference = T &;

    private:
        // Add your own data members here
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
        pointer ptr;

    public:
        iterator() : ptr(nullptr)
        { /* TODO */
        }
        // Add any constructors that you may need
        iterator(T *array) : ptr(array) {}
        // This assignment operator is done for you, please do not add more
        iterator &operator=(const iterator &) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept
        { /* TODO */
            return *ptr;
        }
        [[nodiscard]] pointer operator->() const noexcept
        { /* TODO */
            return ptr;
        }

        // Prefix Increment: ++a
        iterator &operator++() noexcept
        { /* TODO */
            ptr += 1;
            return *this;
        }
        // Postfix Increment: a++
        iterator operator++(int) noexcept
        { /* TODO */
            iterator temp(ptr);
            ptr += 1;
            return temp;
        }
        // Prefix Decrement: --a
        iterator &operator--() noexcept
        { /* TODO */
            ptr -= 1;
            return *this;
        }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept
        { /* TODO */
            iterator temp(ptr);
            ptr -= 1;
            return temp;
        }

        iterator &operator+=(difference_type offset) noexcept
        { /* TODO */
            ptr += offset;
            return *this;
        }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept
        { /* TODO */
            iterator copyIt(ptr + offset);
            return copyIt;
        }

        iterator &operator-=(difference_type offset) noexcept
        { /* TODO */
            ptr -= offset;
            return *this;
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept
        { /* TODO */
            iterator copyIt(ptr - offset);
            return copyIt;
        }
        [[nodiscard]] difference_type operator-(const iterator &rhs) const noexcept
        { /* TODO */
            return ptr - rhs.ptr;
        }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept
        { /* TODO */
            iterator it(ptr + offset);
            reference rfr = *(it.ptr);
            return rfr;
        }

        [[nodiscard]] bool operator==(const iterator &rhs) const noexcept
        { /* TODO */
            if (this->ptr == rhs.ptr)
                return true;
            return false;
        }
        [[nodiscard]] bool operator!=(const iterator &rhs) const noexcept
        { /* TODO */
            if (this->ptr != rhs.ptr)
                return true;
            return false;
        }
        [[nodiscard]] bool operator<(const iterator &rhs) const noexcept
        { /* TODO */
            if (this->ptr < rhs.ptr)
                return true;
            return false;
        }
        [[nodiscard]] bool operator>(const iterator &rhs) const noexcept
        { /* TODO */
            if (this->ptr > rhs.ptr)
                return true;
            return false;
        }
        [[nodiscard]] bool operator<=(const iterator &rhs) const noexcept
        { /* TODO */
            if (this->ptr <= rhs.ptr)
                return true;
            return false;
        }
        [[nodiscard]] bool operator>=(const iterator &rhs) const noexcept
        { /* TODO */
            if (this->ptr >= rhs.ptr)
                return true;
            return false;
        }
    };

    void clear() noexcept
    { /* TODO */
        delete[] array;
        array = nullptr;
        _size = 0;
    }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace
{
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const &iterator) noexcept
{ /* TODO */
    _Iterator copyItr(iterator + offset);
    return copyItr;
}

#endif
