#include <vector>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <cassert>
#include <string>

#include "maybe.hpp"
#include "error.hpp"

template<class K, class V, class Hash = std::hash<K>>
class HashMap {
public:
  HashMap();

  Maybe<void> Insert(K key, V value);

  Maybe<void> Delete(const K &key);

  Maybe<V>    Get(const K &key);

private:

  class Element {
  public:
    Element() : key_(), value_(), is_deleted_(false), is_empty_(true) {}
    Element(const K& k, const V& v) : key_(k), value_(v), is_deleted_(false), is_empty_(false) {}

    [[nodiscard]] bool Deleted() const { return is_deleted_; }
    [[nodiscard]] bool Empty() const { return is_empty_; }

    K key_;
    V value_;
    bool is_deleted_;
    bool is_empty_;
  };

  enum OperationType : int {
    SEARCH,
    INSERT,
    DELETE
  };

  size_t GetNextPosition(size_t pos);
  bool IsValid(OperationType type, size_t pos);

  double GetLoadFactor();

  void Rehash(size_t new_capacity);

  constexpr static size_t INITIAL_CAPACITY = 8;
  constexpr static double MIN_LOAD_FACTOR = 0.25;
  constexpr static double MAX_LOAD_FACTOR = 0.75;


  size_t size;
  size_t capacity;
  std::vector<Element> storage_;
};



template<class K, class V, class Hash>
HashMap<K, V, Hash>::HashMap() : size(0), capacity(INITIAL_CAPACITY) {
  storage_.resize(INITIAL_CAPACITY);
}

template<class K, class V, class Hash>
Maybe<void> HashMap<K, V, Hash>::Insert(K key, V value) {
  if (GetLoadFactor() > MAX_LOAD_FACTOR) {
    Rehash(capacity * 2);
  }
  size_t index = Hash{}(key) % capacity;
  while (IsValid(INSERT, index)) {
    if (storage_[index].key_ == key) {
      return make_result::Fail(KEY_EXIST);
    } else {
      index = GetNextPosition(index);
    }
  }
  storage_[index] = Element(key, value);
  size++;
  return make_result::Ok();
}

template<class K, class V, class Hash>
bool HashMap<K, V, Hash>::IsValid(HashMap::OperationType type, size_t pos) {
  switch (type) {
    case DELETE: [[fallthrough]];
    case SEARCH:
      if (storage_[pos].Empty() && !storage_[pos].Deleted()) {
        return false;
      }
      return true;
    case INSERT:
      if (storage_[pos].Empty() || storage_[pos].Deleted()) {
        return false;
      }
      return true;
  }
}

template<class K, class V, class Hash>
size_t HashMap<K, V, Hash>::GetNextPosition(size_t pos) {
  // Linear probing
  return (pos + 1) % capacity;
}

template<class K, class V, class Hash>
Maybe<void> HashMap<K, V, Hash>::Delete(const K &key) {
  if (GetLoadFactor() < MIN_LOAD_FACTOR) {
    Rehash(capacity / 2);
  }
  size_t index = Hash{}(key) % capacity;
  while (IsValid(DELETE, index)) {
    if (!storage_[index].Deleted() && storage_[index].key_ == key) {
      storage_[index].is_deleted_ = true;
      storage_[index].is_empty_ = true;
      size--;
      return make_result::Ok();
    }
    index = GetNextPosition(index);
  }
  return make_result::Fail(DELETE_FAIL);
}

template<class K, class V, class Hash>
Maybe<V> HashMap<K, V, Hash>::Get(const K &key) {
  size_t index = Hash{}(key) % capacity;
  while (IsValid(SEARCH, index)) {
    if (storage_[index].key_ == key) {
      if (!storage_[index].Deleted())
        return make_result::Ok(storage_[index].value_);
      else
        return make_result::Fail(KEY_NOT_EXIST);
    }
    index = GetNextPosition(index);
  }
  return make_result::Fail(KEY_NOT_EXIST);
}

template<class K, class V, class Hash>
double HashMap<K, V, Hash>::GetLoadFactor() {
  return size / double(capacity);
}

template<class K, class V, class Hash>
void HashMap<K, V, Hash>::Rehash(size_t new_capacity) {
  capacity = new_capacity;
  auto tmp = std::move(storage_);
  storage_.resize(new_capacity);
  for (auto& i : tmp) {
    if (!i.Empty()) {
      Insert(i.key_, i.value_);
    }
  }
}


int main() {
//  HashMap<int, int> a;
//  auto start = std::chrono::high_resolution_clock::now();
//  const int SIZE = 1000;
//  for (int i = 0; i < SIZE; ++i) {
//    a.Insert(i, i);
//  }
//  __asm volatile (
//    ".intel_syntax noprefix;\n\t"
//    "mov rax, 5;\n\t"
//    ".att_syntax prefix;\n\t"
//    : "=r"(a)
//    : "r"(a)
//    : );
  const int SIZE = 1'000;
  HashMap<std::string, int> a;
  std::vector<std::string> s;
  auto start = std::chrono::high_resolution_clock::now();
  s.resize(SIZE);
  for (int i = 0; i < SIZE; ++i) {
    s[i] = std::to_string(i);
  }
  for (int i = 0; i < SIZE; ++i) {
    a.Insert(s[i], i);
  }

  for (int i = 0; i < SIZE * SIZE; ++i) {
    a.Get(s[i % SIZE]);
  }
  auto finish = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << std::endl;
  return 0;
}