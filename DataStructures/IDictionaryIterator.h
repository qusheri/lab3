#pragma once

template <typename TKey, typename TElement>
class IDictionaryIterator
{
public:
    virtual ~IDictionaryIterator() {}

    virtual void Reset() = 0;

    virtual TElement& operator*() const = 0;
    virtual TKey& GetCurrentKey() const = 0;
    virtual TElement* operator->() const = 0;

    virtual IDictionaryIterator& operator++() = 0;
    virtual IDictionaryIterator& operator+=(size_t) = 0;

    virtual bool operator==(const IDictionaryIterator& other) const = 0;
    virtual bool operator!=(const IDictionaryIterator& other) const = 0;

    virtual bool atEnd() const = 0;
};