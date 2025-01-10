#pragma once

template <typename TKey, typename TElement>
class IDictionaryIterator
{
public:
    virtual ~IDictionaryIterator() {}

    virtual bool MoveNext() = 0;

    virtual void Reset() = 0;

    virtual TKey GetCurrentKey() const = 0;

    virtual TElement GetCurrentValue() const = 0;
};