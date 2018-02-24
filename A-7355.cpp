#include <vector>
#include <list>
#include <stdexcept>
#include <iostream>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>> 
class HashMap {
public:
    const int bSize = 8;
    size_t nowSize;
    size_t sizeOfTable;
    std::vector<std::pair<typename std::list<std::pair<const KeyType, ValueType>>::iterator, size_t>> hashPointers;
    std::list<std::pair<const KeyType, ValueType>> list;
    Hash hasher;

    HashMap(Hash hasher = Hash()): nowSize(bSize), sizeOfTable(0), hasher(hasher) {
        hashPointers.resize(nowSize, {list.end(), 0});
    }
    
    template<class BeginIterator, class EndIterator>
    HashMap(BeginIterator begin, EndIterator end, Hash hasher = Hash()) {
        *this = HashMap(hasher);
        while (begin != end) {
            this->insert(*begin);
            ++begin;
        }
    }

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list, Hash hasher = Hash()) {
        *this = HashMap(list.begin(), list.end(), hasher);
    }

    size_t size() const {
        return sizeOfTable;
    }

    bool empty() const {
        return sizeOfTable == 0;
    }

    Hash hash_function() const {
        return hasher;
    }

    void insert(std::pair<KeyType, ValueType> value) {
        if (find(value.first) != end()) return;
        if (sizeOfTable * 4 > nowSize * 3) rebuild();
        sizeOfTable += 1;
        size_t hash = hasher(value.first) % nowSize;
        if (hashPointers[hash].first == list.end()) {
            hashPointers[hash].first = list.begin();
            hashPointers[hash].second = 0;
        }
        hashPointers[hash].first = list.insert(hashPointers[hash].first, value);
        hashPointers[hash].second += 1;
    }

    void erase(KeyType key) {
        auto it = find(key);
        if (it == end()) return;
        sizeOfTable--;
        size_t hash = hasher(it->first) % nowSize;
        hashPointers[hash].second--;
        if (hashPointers[hash].second == 0) {
            hashPointers[hash].first = list.end();
        }
        if (hashPointers[hash].first == it) {
            hashPointers[hash].first = list.erase(it);
        } else {
            list.erase(it);
        }
    }

    using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;

    iterator begin() {
        return list.begin();
    }

    const_iterator begin() const {
        return list.cbegin();
    }

    iterator end() {
        return list.end();
    }

    const_iterator end() const {
        return list.cend();
    }

    iterator find(KeyType key) {
        size_t hash = hasher(key) % nowSize;
        auto it = hashPointers[hash].first;
        auto size = hashPointers[hash].second;
        for (size_t i = 0; i < size; ++i) {
            if (it->first == key) return it;
            ++it;
        }
        return end();
    }

    const_iterator find(KeyType key) const {
        size_t hash = hasher(key) % nowSize;
        auto it = hashPointers[hash].first;
        auto size = hashPointers[hash].second;
        for (size_t i = 0; i < size; ++i) {
            if (it->first == key) return static_cast<const_iterator>(it);
            ++it;
        }
        return end();
    }

    ValueType& operator[] (KeyType key) {
        auto it = find(key);
        if (it == end()) {
            insert({key, ValueType()});
            return find(key)->second;
        } else {
            return it->second;
        }
    }

    const ValueType& at(KeyType key) const {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("");
        } else {
            return it->second;
        }
    }

    void clear() {
        list.clear();
        hashPointers.assign(bSize, {list.end(), 0});
        sizeOfTable = 0;
        nowSize = bSize;
    }

    HashMap<KeyType, ValueType, Hash>& operator= (const HashMap<KeyType, ValueType, Hash>& hashmap) {
        hasher = hashmap.hasher;
        hashPointers.resize(hashmap.nowSize, {list.end(), 0});
        nowSize = hashmap.nowSize;
        sizeOfTable = 0;
        for (auto& x : hashmap) {
            insert(x);
        }
        return *this;
    }

    HashMap(HashMap<KeyType, ValueType, Hash>& hashmap) {
        *this = hashmap;
    }

    void rebuild() {
        size_t copyNowSize = nowSize * 2;
        std::list<std::pair<const KeyType, ValueType>> copyList = list;
        clear();
        nowSize = copyNowSize;
        hashPointers.resize(nowSize, {list.end(), 0});
        for (auto& x : copyList) {
            insert(x);
        }
    }
};
