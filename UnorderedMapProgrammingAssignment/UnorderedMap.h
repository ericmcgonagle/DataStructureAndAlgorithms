#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility> // std::pair
#include <iostream>

#include "primes.h"

template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap
{
public:
    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

private:
    struct HashNode
    {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type &val, HashNode *next = nullptr) : next{next}, val{val} {}
        HashNode(value_type &&val, HashNode *next = nullptr) : next{next}, val{std::move(val)} {}
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode *_head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count)
    {
        return hash_code % bucket_count;
    }

public:
    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap *_map;
        HashNode *_ptr;

        explicit basic_iterator(UnorderedMap const *map, HashNode *ptr) noexcept : _map(map), _ptr(ptr)
        { /* TODO */
        }

    public:
        basic_iterator() : _map(nullptr), _ptr(nullptr){};

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const
        {
            return _ptr->val;
        }
        pointer operator->() const
        {
            return &(_ptr->val);
        }
        basic_iterator &operator++()
        {
            // if end of bucket reached, find next bucket that isn't nullptr and return
            if (_ptr->next == nullptr)
            {
                size_type tempBucket = _map->_bucket(_ptr->val);
                tempBucket += 1;
                while (tempBucket < _map->_bucket_count)
                {
                    _ptr = _map->_buckets[tempBucket];
                    if (_ptr)
                    {
                        //_ptr = _map->_buckets[tempBucket];
                        return *this;
                    }
                    else
                        tempBucket += 1;
                    // if (_ptr == nullptr)
                    //     tempBucket += 1;
                    // else
                    //     break;
                }
                _ptr = nullptr;
            }
            // if not end of bucket, sipmly go to next key-value
            else
                _ptr = _ptr->next;

            return *this;
        }
        basic_iterator operator++(int)
        {
            basic_iterator origItr = *this;
            // (*this)++;
            // return origItr;

            if (_ptr->next == nullptr)
            {
                size_type tempBucket = _map->_bucket(_ptr->val);
                tempBucket += 1;
                while (tempBucket < _map->_bucket_count)
                {
                    _ptr = _map->_buckets[tempBucket];
                    if (_ptr)
                    {
                        //_ptr = _map->_buckets[tempBucket];
                        return origItr;
                    }
                    else
                        tempBucket += 1;
                    // if (_ptr == nullptr)
                    //     tempBucket += 1;
                    // else
                    //     break;
                }
                _ptr = nullptr;
            }
            // if not end of bucket, sipmly go to next key-value
            else
                _ptr = _ptr->next;

            return origItr;
        }
        bool operator==(const basic_iterator &other) const noexcept
        {
            return _ptr == other._ptr;
        }
        bool operator!=(const basic_iterator &other) const noexcept
        {
            return _ptr != other._ptr;
        }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const key_type, mapped_type>;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        HashNode *_node;

        explicit local_iterator(HashNode *node) noexcept : _node(node)
        {
        }

    public:
        local_iterator() : _node(nullptr)
        {
        }

        local_iterator(const local_iterator &) = default;
        local_iterator(local_iterator &&) = default;
        ~local_iterator() = default;
        local_iterator &operator=(const local_iterator &) = default;
        local_iterator &operator=(local_iterator &&) = default;
        reference operator*() const
        {
            return _node->val;
        }
        pointer operator->() const
        {
            return &(_node->val);
        }
        local_iterator &operator++()
        {
            _node = _node->next;
        }
        local_iterator operator++(int)
        {
            local_iterator tempItr = local_iterator(_node);
            _node = _node->next;
            return tempItr;
        }

        bool operator==(const local_iterator &other) const noexcept
        {
            return _node == other._node;
        }
        bool operator!=(const local_iterator &other) const noexcept
        {
            return _node != other._node;
        }
    };

private:
    size_type _bucket(size_t code) const
    {
        return _range_hash(code, _bucket_count);
    }
    size_type _bucket(const Key &key) const
    {
        return _bucket(_hash(key));
    }
    size_type _bucket(const value_type &val) const
    {
        return _bucket(_hash(val.first));
    }

    HashNode *&_find(size_type code, size_type bucket, const Key &key)
    {
        HashNode **currNode = &_buckets[bucket];

        // iterate until key is found
        while (*currNode)
        {
            if (_hash((*currNode)->val.first) == code && _equal((*currNode)->val.first, key))
                return *currNode;
            currNode = &(*currNode)->next;
        }

        return *currNode;
    }

    HashNode *&_find(const Key &key)
    {
        size_type currCode = _hash(key);
        size_type currBucket = _bucket(key);
        return _find(currCode, currBucket, key);
    }

    HashNode *_insert_into_bucket(size_type bucket, value_type &&value)
    {
        HashNode *&currHead = _buckets[bucket];
        HashNode *newNode = new HashNode(std::move(value), currHead);

        // head is empty or _head bucket is greater than value's bucket
        if (_head == nullptr || bucket <= _bucket(_head->val))
        {
            // newNode->next = _head->next;
            _head = newNode;
        }

        // update remaining attributes
        currHead = newNode;
        _size += 1;
        return newNode;
    }

    void _move_content(UnorderedMap &src, UnorderedMap &dst)
    {
    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash &hash = Hash{},
                          const key_equal &equal = key_equal{})
    {
        _bucket_count = next_greater_prime(bucket_count);
        _hash = hash;
        _equal = equal;
        _head = nullptr;
        _size = 0;
        _buckets = new HashNode *[_bucket_count]();
    }

    ~UnorderedMap()
    {
        clear();
        delete[] _buckets;
    }

    UnorderedMap(const UnorderedMap &other)
    {
        _bucket_count = other._bucket_count;
        _head = nullptr;
        _size = 0;
        _equal = other._equal;
        _hash = other._hash;
        _buckets = new HashNode *[_bucket_count]();
        for (const_iterator i = other.cbegin(); i != other.cend(); ++i)
        {
            insert(i._ptr->val);
        }
    }

    UnorderedMap(UnorderedMap &&other)
    {
        _bucket_count = other._bucket_count;
        _head = other._head;
        _size = other._size;
        _equal = other._equal;
        _hash = other._hash;
        _buckets = other._buckets;

        // update other
        other._head = nullptr;
        other._size = 0;
        other._buckets = new HashNode *[other._bucket_count]();
    }

    UnorderedMap &operator=(const UnorderedMap &other)
    {
        // check for self-assignment
        if (this == &other)
            return *this;

        clear();
        delete _buckets;
        _bucket_count = other._bucket_count;
        _head = nullptr;
        _size = 0;
        _equal = other._equal;
        _hash = other._hash;
        _buckets = new HashNode *[_bucket_count]();
        for (const_iterator i = other.cbegin(); i != other.cend(); ++i)
        {
            insert(i._ptr->val);
        }
    }

    UnorderedMap &operator=(UnorderedMap &&other)
    {
        // check for self assignment
        if (this == &other)
            return *this;

        clear();
        delete _buckets;
        _bucket_count = other._bucket_count;
        _head = other._head;
        _size = other._size;
        _equal = other._equal;
        _hash = other._hash;
        _buckets = other._buckets;

        // update other
        other._head = nullptr;
        other._size = 0;
        other._buckets = new HashNode *[other._bucket_count]();
    }

    void clear() noexcept
    {
        while (_size > 0)
            erase(begin());
    }

    size_type size() const noexcept
    {
        return _size;
    }

    bool empty() const noexcept
    {
        return _size == 0;
    }

    size_type bucket_count() const noexcept
    {
        return _bucket_count;
    }

    iterator begin()
    {
        return iterator(this, _head);
    }
    iterator end()
    {
        return iterator(this, nullptr);
    }

    const_iterator cbegin() const { return const_iterator(this, _head); };
    const_iterator cend() const { return const_iterator(this, nullptr); };

    local_iterator begin(size_type n)
    {
        return local_iterator(_buckets[n]);
    }
    local_iterator end(size_type n)
    {
        return local_iterator(nullptr);
    }

    size_type bucket_size(size_type n)
    {
        local_iterator itr = local_iterator(_buckets[n]);
        size_type numElem = 0;

        // iterate until end of bucket reached
        while (itr != local_iterator(nullptr))
        {
            numElem += 1;
            itr++;
        }

        return numElem;
    }

    float load_factor() const
    {
        float totalNum = (float)size();
        return totalNum / bucket_count();
    }

    size_type bucket(const Key &key) const
    {
        return _bucket(key);
    }

    std::pair<iterator, bool> insert(value_type &&value)
    {
        // resulting pair that is returned
        // std::pair<iterator, bool> result;
        // size_type currCode = _hash(value.first);
        size_type currBucket = _bucket(value);

        HashNode *findNode = _find(value.first);

        if (findNode)
        {
            return std::pair<iterator, bool>(iterator(this, findNode), false);
        }
        else
        {
            HashNode *newNode = _insert_into_bucket(_bucket(value.first), std::move(value));
            //_size++;
            return std::pair<iterator, bool>(iterator(this, newNode), true);
        }
    }

    std::pair<iterator, bool> insert(const value_type &value)
    {
        size_type currBucket = _bucket(value);

        HashNode *findNode = _find(value.first);

        if (findNode)
        {
            return std::pair<iterator, bool>(iterator(this, findNode), false);
        }
        else
        {
            value_type tempVal = value;
            HashNode *newNode = _insert_into_bucket(_bucket(tempVal.first), std::move(tempVal));
            //_size++;
            return std::pair<iterator, bool>(iterator(this, newNode), true);
        }
    }

    iterator find(const Key &key)
    {
        iterator result = iterator(this, _find(key));
        return result;
    }

    T &operator[](const Key &key)
    {
        HashNode *currNode = _find(key);
        if (currNode)
            return currNode->val.second;
        else
        {
            auto currPair = std::pair(key, mapped_type());
            return insert(currPair).first->second;
        }
    }

    iterator erase(iterator pos)
    {
        HashNode *&currNode = _find(pos._ptr->val.first);
        if (currNode == _head)
        {
            iterator itr = iterator(this, currNode);
            itr++;
            _head = itr._ptr;
        }

        HashNode *tmpNode = currNode;
        iterator newItr = iterator(this, currNode);
        newItr++;
        currNode = currNode->next;
        delete tmpNode;
        _size -= 1;
        return newItr;
    }

    size_type erase(const Key &key)
    {
        HashNode *&currNode = _find(key);
        // if key isn't found
        if (!currNode)
            return 0;

        if (currNode == _head)
        {
            // iterator itr = iterator(this, currNode);
            // itr++;
            // _head = itr._ptr;
            _head = currNode->next;
        }

        HashNode *tmpNode = currNode;
        currNode = currNode->next;
        delete tmpNode;
        _size -= 1;
        return 1;
    }

    template <typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> &map, std::ostream &os);
};

template <typename K, typename V>
void print_map(const UnorderedMap<K, V> &map, std::ostream &os = std::cout)
{
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for (size_type bucket = 0; bucket < map.bucket_count(); bucket++)
    {
        os << bucket << ": ";

        HashNode const *node = map._buckets[bucket];

        while (node)
        {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
