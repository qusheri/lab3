#pragma once

template <typename TElement>
class SequenceIterator
{
public:
    virtual ~SequenceIterator() {}

    virtual void Reset() = 0;

    virtual TElement& operator*() const = 0;
    virtual TElement* operator->() const = 0;

    virtual SequenceIterator& operator++() = 0;
    virtual SequenceIterator& operator+=(size_t) = 0;

    virtual bool operator==(const SequenceIterator& other) const = 0;
    virtual bool operator!=(const SequenceIterator& other) const = 0;

    virtual bool atEnd() const = 0;
};