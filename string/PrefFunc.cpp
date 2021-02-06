#include <iostream>
#include <vector>
#include <string>

template <typename It, typename T = uint32_t>
std::vector<T> CalculatePrefixFunc(It begin, It end) {
    static_assert(
            std::is_same<std::random_access_iterator_tag,
                    typename std::iterator_traits<It>::iterator_category>::value,
            "To calcualte prefix function use random access iterators.\n"
    );
    size_t len = std::distance(begin, end);
    std::vector<T> result(len);
    for (auto i = std::next(begin); i < end; ++i) {
        size_t ind = std::distance(begin, i);
        result[ind] = result[ind - 1];
        while (result[ind] > 0 && *(begin + result[ind]) != *i) {
            result[ind] = result[result[ind] - 1];
        }
        if (*(begin + result[ind]) == *i) {
            result[ind]++;
        }
    }
    return result;
}
