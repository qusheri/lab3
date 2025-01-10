#pragma once

template <typename K, typename V>
struct KeyValue {
    K key;
    V value;

    KeyValue() : key(), value() {}
    KeyValue(const K& k, const V& v) : key(k), value(v) {}
};

