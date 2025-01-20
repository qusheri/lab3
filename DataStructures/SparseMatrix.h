#pragma once


#include "IDictionary.h"
#include "DynamicArray.h"
#include "KeyValue.h"

template<typename TElement>
class SparseMatrix {
public:
    SparseMatrix(int rows, int columns, UnqPtr<IDictionary<KeyValue<int, int>, TElement>> dictionary)
            : rows(rows), columns(columns), elements(std::move(dictionary)) {}

    SparseMatrix() : rows(0), columns(0), elements(UnqPtr<IDictionary<KeyValue<int, int>, TElement>>()) {}

    ~SparseMatrix(){}

    int GetRows() const
    {
        return rows;
    }

    int GetColumns() const
    {
        return columns;
    }

    TElement GetElement(int row, int column) const
    {
        if (row < 0 || row >= rows || column < 0 || column >= columns)
        {
            throw std::out_of_range("Row or column index is out of bounds.");
        }

        KeyValue<int, int> key(row, column);
        if (elements->ContainsKey(key))
        {
            return elements->Get(key);
        }
        else
        {
            return TElement();
        }
    }

    void SetElement(int row, int column, const TElement& value)
    {
        if (row < 0 || row >= rows || column < 0 || column >= columns)
        {
            throw std::out_of_range("Row or column index is out of bounds.");
        }

        KeyValue<int, int> key(row, column);
        if (value != TElement())
        {
            if (elements->ContainsKey(key))
            {
                elements->Update(key, value);
            }
            else
            {
                elements->Add(key, value);
            }
        }
        else
        {
            RemoveElement(key.key, key.value);
        }
    }

    void RemoveElement(int row, int column) {
        if (row < 0 || row >= rows || column < 0 || column >= columns) {
            throw std::out_of_range("Row or column index is out of bounds.");
        }

        KeyValue<int, int> key(row, column);
        if (elements->ContainsKey(key)) {
            elements->Remove(key);
        }
    }

    void ForEach(void (*func)(const KeyValue<int, int> &, const TElement &)) const {
        auto iterator = elements->GetIterator();

        while (iterator->MoveNext()) {
            KeyValue<int, int> key = iterator->GetCurrentKey();
            TElement value = iterator->GetCurrentValue();
            func(key, value);
        }
    }



    void Map(TElement (*func)(const TElement&))
    {
        auto iterator = elements->GetIterator();
        while (!iterator->atEnd())
        {
            KeyValue<int, int> key = iterator->GetCurrentKey();
            TElement value = **iterator;
            elements->Update(key, func(value));
            ++(*iterator);
        }
    }

    template <typename FuncType>
    FuncType Reduce(FuncType (*func)(const FuncType&, const TElement&), FuncType initial) const
    {
        FuncType result = initial;
        auto iterator = elements->GetIterator();
        while (!(*iterator).atEnd())
        {
            TElement value = **iterator;
            result = func(result, value);
            ++*iterator;
        }
        return result;
    }

    UnqPtr<IDictionaryIterator<KeyValue<int, int>, TElement>> GetIterator() const
    {
        return elements->GetIterator();
    }

    const IDictionary<KeyValue<int, int>, TElement>& GetElements() const {
        return *elements;
    }

private:
    int rows;
    int columns;
    UnqPtr<IDictionary<KeyValue<int, int>, TElement>> elements;
};

