#include <iostream>
#include <utility>
#include <vector>

/// \brief: Builds suffix array on given string ending with a sentinel
/// (such symbol that sentinel < container is always true)
template <typename Container>
class SuffixArray {
private:
    // Building consists of log N phases. On k-th step we sort cyclic shifts of length 2^k.
    uint32_t first_step();
    uint32_t next_step(size_t k);

    Container container_;
    std::vector<uint32_t> color_;
    std::vector<uint32_t> permutation_;
    std::vector<uint32_t> lcp_;
    const size_t ALPHABET_SIZE;

public:
    explicit SuffixArray(Container  input, size_t alphabet_size = 256);
    void BuildLCP();
    [[nodiscard]] std::vector<uint32_t> GetLCP() const { return lcp_; }
    [[nodiscard]] std::vector<uint32_t> GetArray() const { return permutation_; }
};

template<typename Container>
uint32_t SuffixArray<Container>::first_step() {
    std::vector<int32_t> count(ALPHABET_SIZE);
    for (const auto& item : container_) count[item]++;

    for (size_t i = 1; i < ALPHABET_SIZE; ++i)
        count[i] += count[i - 1];

    ssize_t size = container_.size();
    for (ssize_t i = size - 1; i >= 0; --i) {
        permutation_[--count[container_[i]]] = i;
    }

    for (size_t i = 1; i < size; ++i) {
        color_[permutation_[i]] = color_[permutation_[i - 1]];
        if (container_[permutation_[i]] != container_[permutation_[i - 1]]) {
            color_[permutation_[i]]++;
        }
    }
    return color_[permutation_.back()] + 1;
}


template<typename Container>
SuffixArray<Container>::SuffixArray(Container input, size_t alphabet_size) :
                                    ALPHABET_SIZE(alphabet_size),
                                    container_(std::move(input)),
                                    color_(container_.size()),
                                    permutation_(container_.size()),
                                    lcp_(container_.size()) {
    uint32_t different_colors = first_step();
    for (size_t k = 1; k < container_.size(); k <<= 1) {
        if (different_colors == container_.size()) break;
        different_colors = next_step(k);
    }
}

template<typename Container>
uint32_t SuffixArray<Container>::next_step(size_t k) {
    for (auto & i : permutation_) i = (i - k + permutation_.size()) % permutation_.size();

    std::vector<int32_t> count(container_.size());
    std::vector<uint32_t> new_permutation(container_.size());
    for (const auto & item : permutation_) count[color_[item]]++;

    ssize_t size = container_.size();
    for (size_t i = 1; i < size; ++i)
        count[i] += count[i - 1];
    for (ssize_t i = size - 1; i >= 0; --i) {
        new_permutation[--count[color_[permutation_[i]]]] = permutation_[i];
    }
    permutation_ = std::move(new_permutation);

    std::vector<uint32_t> new_color(size);
    for (size_t i = 1; i < size; ++i) {
        new_color[permutation_[i]] = new_color[permutation_[i - 1]];
        if (color_[permutation_[i]] != color_[permutation_[i - 1]] ||
            color_[(permutation_[i] + k) % size] != color_[(permutation_[i - 1] + k) % size]) {
                new_color[permutation_[i]]++;
        }
    }
    color_ = std::move(new_color);
    return color_[permutation_.back()] + 1;
}

template<typename Container>
void SuffixArray<Container>::BuildLCP() {
    const auto& inverse = color_;
    int32_t cur_lcp = 0;
    for (size_t i = 0; i < permutation_.size() - 1; ++i) {
        cur_lcp = std::max(cur_lcp - 1, 0);
        size_t prev = permutation_[inverse[i] - 1]; // suffix that goes before current one in suff array
        while (container_[i + cur_lcp] == container_[prev + cur_lcp]) cur_lcp++;
        lcp_[inverse[i]] = cur_lcp;
    }
}