#pragma once

#include "IDictionary.h"
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ShrdPtr.h"
#include "UnqPtr.h"
#include <functional>
#include <stdexcept>

#include "KeyValue.h"

template<typename TKey, typename TElement>
class HashTable : public IDictionary<TKey, TElement> {
public:
    HashTable(size_t initialCapacity = 16);

    virtual ~HashTable();

    size_t GetCount() const override;

    size_t GetCapacity() const override;

    TElement Get(const TKey &key) const override;

    bool ContainsKey(const TKey &key) const override;

    void Add(const TKey &key, const TElement &element) override;

    void Remove(const TKey &key) override;

    void Update(const TKey &key, const TElement &element) override;

    UnqPtr<IDictionaryIterator<TKey, TElement>> GetIterator() override {
        return UnqPtr<IDictionaryIterator<TKey, TElement>>(new Iterator(this->table.get()));
    }

private:
    class Iterator : public IDictionaryIterator<TKey, TElement> {
    public:
        explicit Iterator(DynamicArraySmart<LinkedListSmart<KeyValue<TKey, TElement>>>* array)
        : ArrayIter(new typename DynamicArraySmart<LinkedListSmart<KeyValue<TKey,TElement>>>::Iterator(array)),
          bucketIter(nullptr)
        {
            goToValidBucket();
        }

        void Reset() override {
            ArrayIter->Reset();
            goToValidBucket();
        }

        TElement& operator*() const override {
            if (ArrayIter->atEnd() || (*bucketIter).atEnd())
                throw std::out_of_range("HashTable::Iterator is out of range");
            return (**bucketIter).value;
        }

        TKey& GetCurrentKey() const {
            if (ArrayIter->atEnd() || (*bucketIter).atEnd())
                throw std::out_of_range("HashTable::Iterator is out of range");
            return (**bucketIter).key;
        }

        TElement* operator->() const override {
            return &this->operator*();
        }

        Iterator& operator++() override {
            if (ArrayIter->atEnd()) {
                return *this;
            }
            ++(*bucketIter);
            if (!(*bucketIter).atEnd()) {
                return *this;
            }
            while (true) {
                ++(*ArrayIter);
                if (ArrayIter->atEnd()) {
                    break;
                }
                auto& lst = **ArrayIter;
                bucketIter = new typename LinkedListSmart<KeyValue<TKey,TElement>>::Iterator(&lst);
                if (!(*bucketIter).atEnd()) {
                    break;
                }
            }
            return *this;
        }
        Iterator& operator+=(size_t steps) override {
            while(steps-- > 0) {
                ++(*this);
            }
            return *this;
        }

        bool operator==(const IDictionaryIterator<TKey, TElement> & other) const override {
            auto* o = dynamic_cast<const Iterator*>(&other);
            if (!o) return false;
            bool sameArray   = (*this->ArrayIter) == (*o->ArrayIter);
            bool sameBucket  = (*this->bucketIter) == (*o->bucketIter);
            return sameArray && sameBucket;
        }
        bool operator!=(const IDictionaryIterator<TKey, TElement>& other) const override {
            return !(*this == other);
        }

        bool atEnd() const override {
            if (ArrayIter->atEnd()) return true;
            return (*bucketIter).atEnd();
        }

    private:
        typename DynamicArraySmart<LinkedListSmart<KeyValue<TKey, TElement>>>::Iterator* ArrayIter;
        typename LinkedListSmart<KeyValue<TKey, TElement>>::Iterator* bucketIter;
        void goToValidBucket() {
            if (ArrayIter->atEnd()) {
                return;
            }
            auto & lst = **ArrayIter;
            bucketIter = new typename LinkedListSmart<KeyValue<TKey,TElement>>::Iterator(&lst);
            while (!ArrayIter->atEnd() && (*bucketIter).atEnd()) {
                ++(*ArrayIter);
                if (!ArrayIter->atEnd()) {
                    auto & nextLst = **ArrayIter;
                    bucketIter = new typename LinkedListSmart<KeyValue<TKey,TElement>>::Iterator(&nextLst);
                }
            }
        }
    };


    typedef LinkedListSmart<KeyValue<TKey, TElement>> KeyValueList;
    typedef DynamicArraySmart<KeyValueList> ArrayList;
    typedef UnqPtr<ArrayList> TableUPtr;

    TableUPtr table;
    size_t count;

    size_t HashFunction(const TKey &key) const;

    void Rehash();
};

template<typename TKey, typename TElement>
HashTable<TKey, TElement>::HashTable(size_t initialCapacity)
        : table(new DynamicArraySmart<LinkedListSmart<KeyValue<TKey, TElement>>>(static_cast<int>(initialCapacity))), count(0) {
    for (size_t i = 0; i < initialCapacity; ++i) {
        table->Append(LinkedListSmart<KeyValue<TKey, TElement>>());
    }
}

template<typename TKey, typename TElement>
HashTable<TKey, TElement>::~HashTable() {}

template<typename TKey, typename TElement>
size_t HashTable<TKey, TElement>::GetCount() const {
    return count;
}

template<typename TKey, typename TElement>
size_t HashTable<TKey, TElement>::GetCapacity() const {
    return table->GetLength();
}

template<typename TKey, typename TElement>
size_t HashTable<TKey, TElement>::HashFunction(const TKey &key) const {
    return std::hash<TKey>()(key);
}

template<typename TKey, typename TElement>
void HashTable<TKey, TElement>::Add(const TKey &key, const TElement &element) {
    size_t capacity = table->GetLength();
    size_t index = HashFunction(key) % capacity;
    auto arrIter = table->GetIterator();
    (*arrIter) += index;
    LinkedListSmart<KeyValue<TKey, TElement>> &chain = (**arrIter);
    for (auto it = chain.GetIterator();!it->atEnd(); ++(*it)) {
        if ((*it)->key == key) {
            (*it)->value = element;
            return;
        }
    }

    chain.Append(KeyValue<TKey, TElement>(key, element));
    ++count;

    if (static_cast<double>(count) / capacity > 0.75) {
        Rehash();
    }
}

template<typename TKey, typename TElement>
void HashTable<TKey, TElement>::Remove(const TKey &key) {
    size_t capacity = table->GetLength();
    size_t index = HashFunction(key) % capacity;
    auto arrIter = table->GetIterator();
    (*arrIter) += index;
    LinkedListSmart<KeyValue<TKey, TElement>> &chain = (**arrIter);
    int i = 0;
    for (auto it = chain.GetIterator();!it->atEnd(); ++(*it)) {
        if ((*it)->key == key) {
            chain.RemoveAt(i);
            --count;
            return;
        }
        ++i;
    }

    throw std::runtime_error("Key not found.");
}

template<typename TKey, typename TElement>
void HashTable<TKey, TElement>::Update(const TKey &key, const TElement &element) {
    size_t capacity = table->GetLength();
    size_t index = HashFunction(key) % capacity;
    auto arrIter = table->GetIterator();
    (*arrIter) += index;
    LinkedListSmart<KeyValue<TKey, TElement>> &chain = (**arrIter);

    for (auto it = chain.GetIterator();!it->atEnd(); ++(*it)) {
        if ((*it)->key == key) {
            (*it)->value = element;
            return;
        }
    }

    throw std::runtime_error("Key not found.");
}

template<typename TKey, typename TElement>
bool HashTable<TKey, TElement>::ContainsKey(const TKey &key) const {
    size_t capacity = table->GetLength();
    size_t index = HashFunction(key) % capacity;
    auto arrIter = table->GetIterator();
    (*arrIter) += index;
    LinkedListSmart<KeyValue<TKey, TElement>> &chain = (**arrIter);

    for (auto it = chain.GetIterator();!it->atEnd(); ++(*it)) {
        if ((*it)->key == key) {
            return true;
        }
    }

    return false;
}

template<typename TKey, typename TElement>
TElement HashTable<TKey, TElement>::Get(const TKey &key) const {
    size_t capacity = table->GetLength();
    size_t index = HashFunction(key) % capacity;
    auto arrIter = table->GetIterator();
    (*arrIter) += index;
    LinkedListSmart<KeyValue<TKey, TElement>> &chain = (**arrIter);
    for (auto it = chain.GetIterator(); !it->atEnd(); ++(*it)) {
        if ((*it)->key == key) {
            return (*it)->value;
        }
    }

    throw std::runtime_error("Key not found.");
}

template<typename TKey, typename TElement>
void HashTable<TKey, TElement>::Rehash() {
    size_t oldCapacity = table->GetLength();
    size_t newCapacity = oldCapacity * 2;
    UnqPtr<DynamicArraySmart<LinkedListSmart<KeyValue<TKey, TElement>>>> newTable(
            new DynamicArraySmart<LinkedListSmart<KeyValue<TKey, TElement>>>(static_cast<int>(newCapacity)));

    for (size_t i = 0; i < newCapacity; ++i) {
        newTable->Append(LinkedListSmart<KeyValue<TKey, TElement>>());
    }

    for (size_t i = 0; i < oldCapacity; ++i) {
        auto arrIter = table->GetIterator();
        (*arrIter) += i;
        LinkedListSmart<KeyValue<TKey, TElement>> &chain = (**arrIter);
        for (auto it = chain.GetIterator();!it->atEnd(); ++(*it)) {
            const KeyValue<TKey, TElement> &kvp = *(*it);
            size_t index = HashFunction(kvp.key) % newCapacity;
            auto newArrIt = newTable->GetIterator();
            (*newArrIt) += index;
            (**newArrIt).Append(kvp);
        }
    }

    table = std::move(newTable);
}