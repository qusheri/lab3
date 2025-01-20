#pragma once

#include "Sequence.h"
#include "UnqPtr.h"
#include "ShrdPtr.h"
#include <stdexcept>
#include <utility>

template <typename T>
class DynamicArraySmart : public Sequence<T> {
private:
    UnqPtr<T[]> data;
    int capacity;
    int length;

    void resize(int newCapacity) {
        UnqPtr<T[]> newData(new T[newCapacity]);
        for (int i = 0; i < length; ++i) {
            newData[i] = std::move(data[i]);
        }
        data = std::move(newData);
        capacity = newCapacity;
    }
    template <typename TKey, typename TElement>
    friend class HashTable;

public:
    DynamicArraySmart() : data(nullptr), capacity(0), length(0) {}

    explicit DynamicArraySmart(int initialCapacity)
            : data(new T[initialCapacity]), capacity(initialCapacity), length(0) {}

    ~DynamicArraySmart() override = default;

    DynamicArraySmart(const DynamicArraySmart&) = delete;
    DynamicArraySmart& operator=(const DynamicArraySmart&) = delete;

    DynamicArraySmart(DynamicArraySmart&& other) noexcept
            : data(std::move(other.data)),
              capacity(other.capacity),
              length(other.length) {
        other.capacity = 0;
        other.length = 0;
    }

    DynamicArraySmart& operator=(DynamicArraySmart&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            capacity = other.capacity;
            length = other.length;

            other.capacity = 0;
            other.length = 0;
        }
        return *this;
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex)
            throw std::out_of_range("Invalid subsequence indices");

        auto* subseq = new DynamicArraySmart<T>(endIndex - startIndex + 1);
        for (int i = startIndex; i <= endIndex; ++i) {
            subseq->Append(data[i]);
        }
        return subseq;
    }

    int GetLength() const override {
        return length;
    }

    void Append(const T& item) override {
        if (length == capacity) {
            int newCapacity = capacity == 0 ? 1 : capacity * 2;
            resize(newCapacity);
        }
        data[length++] = item;
    }

    void Prepend(const T& item) override {
        if (length == capacity) {
            int newCapacity = capacity == 0 ? 1 : capacity * 2;
            resize(newCapacity);
        }
        for (int i = length; i > 0; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[0] = item;
        ++length;
    }

    void InsertAt(const T& item, int index) override {
        if (index < 0 || index > length)
            throw std::out_of_range("Index out of range");
        if (length == capacity) {
            int newCapacity = capacity == 0 ? 1 : capacity * 2;
            resize(newCapacity);
        }
        for (int i = length; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[index] = item;
        ++length;
    }

    void RemoveAt(int index) override {
        if (index < 0 || index >= length)
            throw std::out_of_range("Index out of range");
        for (int i = index; i < length - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --length;
    }

    Sequence<T>* Concat(Sequence<T>* list) const override {
        auto* newArray = new DynamicArraySmart<T>(length + list->GetLength());
        for (int i = 0; i < length; ++i) {
            newArray->Append(data[i]);
        }
        for (int i = 0; i < list->GetLength(); ++i) {
            newArray->Append(data[i]);
        }
        return newArray;
    }

    T& operator[](int index) {
        if (index < 0 || index >= length)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= length)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    class Iterator : public SequenceIterator<T> {
    private:
        ShrdPtr<DynamicArraySmart> array;
        size_t currentIndex;

    public:
        explicit Iterator(size_t index, DynamicArraySmart* array) : array(array), currentIndex(index) {}
        explicit Iterator(DynamicArraySmart* array)
            : array(array), currentIndex(0) {}

        void Reset() override {
            currentIndex = array->GetLength() - 1;
        }

        const size_t& GetKey() const {
            return currentIndex;
        }

        const size_t& GetSize() const {
            return array->GetLength();
        }

        T& operator*() const override {
            if (currentIndex < 0 || currentIndex >= array->GetLength()) throw std::out_of_range("Iterator out of range");
            return (*array)[static_cast<int>(currentIndex)];
        }

        T* operator->() const override {
            if (currentIndex < 0 || currentIndex >= array->GetLength()) throw std::out_of_range("Iterator out of range");
            return &(*array)[static_cast<int>(currentIndex)];
        }

        Iterator& operator++() override {
            return *this += 1;
        }

        Iterator& operator+=(size_t steps) override {
            currentIndex += steps;
            return *this;
        }

        Iterator& operator--() {
            return *this += -1;
        }

        Iterator& operator-=(int steps) {
            return *this += -steps;
        }

        bool operator==(const SequenceIterator<T>& other) const override {
            auto* otherIterator = dynamic_cast<const Iterator*>(&other);
            return otherIterator && currentIndex == otherIterator->currentIndex;
        }

        bool operator!=(const SequenceIterator<T>& other) const override {
            return !(*this == other);
        }

        bool atEnd() const override {
            return currentIndex == array->GetLength();
        }
    };

    SequenceIterator<T>* GetIterator() override {
        return new Iterator(0, this);
    }
};