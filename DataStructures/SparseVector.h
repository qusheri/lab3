#pragma once

#include "IDictionary.h"
#include "ShrdPtr.h"
#include "DynamicArray.h"
#include "KeyValue.h"
#include "memory"
#include "stdexcept"
#include <vector>

template <typename TElement>
class SparseVector
{
public:
    SparseVector(int length, UnqPtr<IDictionary<int, TElement>> dictionary)
            : length(length), elements(std::move(dictionary)) {}

    SparseVector() : length(0), elements(UnqPtr<IDictionary<int, TElement>>()) {}

    ~SparseVector(){}

    int GetLength() const
    {
        return length;
    }

    TElement GetElement(int index) const
    {
        if (index < 0 || index >= length)
        {
            throw std::out_of_range("Index is out of bounds.");
        }

        if (elements->ContainsKey(index))
        {
            return elements->Get(index);
        }
        return TElement();
    }

    void SetElement(int index, const TElement& value)
    {
        if (index < 0 || index >= length)
        {
            throw std::out_of_range("Index is out of bounds.");
        }

        if (value != TElement())
        {
            if (elements->ContainsKey(index))
            {
                elements->Update(index, value);
            }
            else
            {
                elements->Add(index, value);
            }
        }
        else
        {
            RemoveElement(index);
        }
    }

    void RemoveElement(int index) {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Index is out of bounds.");
        }

        if (elements->ContainsKey(index)) {
            elements->Remove(index);
        }
    }

    void Map(TElement (*func)(const TElement&))
    {
        DynamicArraySmart<KeyValue<int, TElement>> updates;
        auto iterator = elements->GetIterator();
        while (iterator->MoveNext()) {
            int key = iterator->GetCurrentKey();
            TElement value = iterator->GetCurrentValue();
            TElement newValue = func(value);
            elements->Update(key, newValue);
        }
    }

    template <typename FuncType>
    FuncType Reduce(FuncType (*func)(const FuncType&, const TElement&), FuncType initial) const
    {
        FuncType result = initial;
        auto iterator = elements->GetIterator();
        while (iterator->MoveNext())
        {
            TElement value = iterator->GetCurrentValue();
            result = func(result, value);
        }
        return result;
    }

    UnqPtr<IDictionaryIterator<int, TElement>> GetIterator() const
    {
        return elements->GetIterator();
    }
    const IDictionary<int, TElement>& GetElements() const {
        return *elements;
    }

private:
    int length;
    UnqPtr<IDictionary<int, TElement>> elements;
};

