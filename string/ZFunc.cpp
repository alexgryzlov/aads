#include <iostream>
#include <string>
#include <vector>

template <typename It, typename T = uint32_t>
std::vector<T> CalculateZFunc(It begin, It end) {
    static_assert(
            std::is_same<std::random_access_iterator_tag,
            typename std::iterator_traits<It>::iterator_category>::value,
            "To calcualte Z function use random access iterators.\n"
            );

    size_t len = std::distance(begin, end);
    std::vector<T> result(len);
    result[0] = len;
    for (It left = begin + 1, z_block_start = left, z_block_end = left; left < end; ++left) {
        size_t ind = std::distance(begin, left);
        if (left < z_block_end) {
            result[ind] = std::min(result[left - z_block_start], T(z_block_end - left));
        }
        while (left + result[ind] < end && *(left + result[ind]) == *(begin + result[ind])) {
            result[ind]++;
        }
        if (left + result[ind] > z_block_end) {
            z_block_end = left + result[ind];
            z_block_start = left;
        }
    }
    return result;
}
