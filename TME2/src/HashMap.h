#pragma once

#include <vector>
#include <forward_list>
#include <utility>
#include <cstddef>
#include <functional> // pour std::hash

template<typename K, typename V>
class HashMap {
public:
    // 🔹 Entry stores a const key and a mutable value
    struct Entry {
        const K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    using Bucket = std::forward_list<Entry>;
    using Table  = std::vector<Bucket>;

    // 🔹 Construct with a number of buckets (must be >= 1)
    HashMap(std::size_t nbuckets = 16) : buckets_(nbuckets), count_(0) {}

    // 🔹 Return pointer to value associated with key, or nullptr if not found.
    V* get(const K& key) {
        std::size_t h = hash(key);
        for (Entry& e : buckets_[h]) {
            if (e.key == key) return &e.value;
        }
        return nullptr;
    }
    //ajouter fct
template<typename K, typename V>
std::vector<std::pair<K,V>> HashMap<K,V>::toKeyValuePairs() const {
    std::vector<std::pair<K,V>> result;
    for (const auto& bucket : buckets_) {
        for (const auto& entry : bucket) {
            result.push_back({entry.key, entry.value});
        }
    }
    return result;
}

    // 🔹 Insert or update (key,value)
    bool put(const K& key, const V& value) {
        std::size_t h = hash(key);
        for (Entry& e : buckets_[h]) {
            if (e.key == key) {
                e.value = value; // mise à jour
                return true;     // existant mis à jour
            }
        }
        // si pas trouvé, insertion
        buckets_[h].push_front(Entry(key, value));
        ++count_;
        return false; // nouvelle entrée
    }

    // 🔹 Current number of stored entries
    std::size_t size() const {
        return count_;
    }

    // 🔹 Optional: Convert table contents to a vector of key/value pairs
    std::vector<std::pair<K,V>> toKeyValuePairs() const {
        std::vector<std::pair<K,V>> result;
        for (const auto& bucket : buckets_) {
            for (const auto& e : bucket) {
                result.push_back({e.key, e.value});
            }
        }
        return result;
    }

private:
    Table buckets_;
    std::size_t count_ = 0;

    std::size_t hash(const K& key) const {
        return std::hash<K>{}(key) % buckets_.size();
    }
};
