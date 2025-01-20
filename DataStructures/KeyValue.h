#pragma once

template <typename K, typename V>
struct KeyValue {
    K key;
    V value;

    KeyValue() : key(), value() {}
    KeyValue(const K& k, const V& v) : key(k), value(v) {}
    bool operator==(const KeyValue& other) const {
        return key == other.key && value == other.value;
    }

    bool operator<(const KeyValue& other) const {
        if (key != other.key)
            return key < other.key;
        return value < other.value;
    }

    bool operator>(const KeyValue& other) const {
        if (key != other.key)
            return key > other.key;
        return value > other.value;
    }
};

namespace std {
    template <typename K, typename V>
    struct hash<KeyValue<K, V>> {
        size_t operator()(const KeyValue<K, V>& p) const {
            return hash<K>()(p.key) ^ (hash<V>()(p.value) << 1);
        }
    };
}