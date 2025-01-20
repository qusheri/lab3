#pragma once

#include "Sequence.h"
#include "ShrdPtr.h"
#include <memory>
#include <stdexcept>

template <typename T>
class LinkedListSmart : public Sequence<T> {
private:
    struct Node {
        T data;
        Node* next;

        explicit Node(const T& item) : data(item), next(nullptr) {}
    };

    Node* head;
    size_t length;

public:
    LinkedListSmart() : head(nullptr), length(0) {}
    ~LinkedListSmart() override = default;

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= static_cast<int>(length) || startIndex > endIndex)
            throw std::out_of_range("Invalid subsequence indices");

        auto subseq = new LinkedListSmart<T>();
        auto current = head;
        for (int i = 0; i <= endIndex; ++i) {
            if (i >= startIndex) {
                subseq->Append(current->data);
            }
            current = current->next;
        }
        return subseq;
    }

    int GetLength() const override {
        return static_cast<int>(length);
    }

    void Append(const T& item) override {
        auto newNode = new Node(item);
        if (!head) {
            head = newNode;
        } else {
            auto current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++length;
    }

    void Prepend(const T& item) override {
        auto newNode = new Node(item);
        newNode->next = head;
        head = newNode;
        ++length;
    }

    void InsertAt(const T& item, int index) override {
        if (index < 0 || index > static_cast<int>(length))
            throw std::out_of_range("Index out of range");
        if (index == 0) {
            Prepend(item);
        } else {
            auto current = head;
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            auto newNode = new Node(item);
            newNode->next = current->next;
            current->next = newNode;
            ++length;
        }
    }

    void RemoveAt(int index) override {
        if (index < 0 || index >= static_cast<int>(length))
            throw std::out_of_range("Index out of range");

        if (index == 0) {
            if (head) {
                head = head->next;
                --length;
            }
        } else {
            auto current = head;
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;
            }

            if (current->next) {
                current->next = current->next->next;
                --length;
            }
        }
    }

    Sequence<T>* Concat(Sequence<T>* list) const override {
        auto newList = new LinkedListSmart<T>();

        auto current = head;
        while (current) {
            newList->Append(current->data);
            current = current->next;
        }

        auto itOther = list->GetIterator();
        while (!itOther->atEnd()) {
            T &elem = **itOther;
            newList->Append(elem);
            ++(*itOther);
        }

        return newList;
    }

    SequenceIterator<T>* GetIterator() override {
        if (!head) {
            return new Iterator(nullptr, this);
        }
        return new Iterator(head, this);
    }
private:
    class Iterator : public SequenceIterator<T> {
    private:
        LinkedListSmart* list;
        Node* currentNode;

    public:
        explicit Iterator(Node* node, LinkedListSmart* list) : list(list), currentNode(node) {}

        explicit Iterator(LinkedListSmart* list)
            : list(list), currentNode(list->head) {}

        void Reset() override {
            currentNode = list->head;
        }

        const size_t& GetSize() const {
            return list->GetLength();
        }

        T& operator*() const override {
            if (!currentNode) throw std::out_of_range("Iterator out of range");
            return currentNode->data;
        }

        T* operator->() const override {
            if (!currentNode) throw std::out_of_range("Iterator out of range");
            return &currentNode->data;
        }

        Iterator& operator++() override {
            return *this += 1;
        }

        Iterator& operator+=(size_t steps) override {
            if (steps > 0) {
                while (steps > 0) {
                    --steps;
                    if (currentNode && currentNode->next) {
                        currentNode = currentNode->next;
                    }
                    else {
                        currentNode = nullptr;
                    }
                }
            } else {
                throw std::logic_error("Decrement is not supported for singly linked list iterators.");
            }
            return *this;
        }

        bool operator==(const SequenceIterator<T>& other) const override {
            auto* otherIterator = dynamic_cast<const Iterator*>(&other);
            return otherIterator && currentNode == otherIterator->currentNode;
        }

        bool operator!=(const SequenceIterator<T>& other) const override {
            return !(*this == other);
        }

        bool atEnd() const override {
            return !currentNode;
        }
    };
    template <typename TKey, typename TElement>
    friend class HashTable;
};