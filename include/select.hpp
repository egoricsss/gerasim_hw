#include <iostream>
#include <optional>
#include <random>
#include <span>
#include <vector>

#include "split.hpp"

namespace accessed_homework
{
template <Comparable T>
const T& get_random_element(std::span<T> arr) noexcept
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, arr.size() - 1);
    return arr[dist(gen)];
}

template <Comparable T>
std::optional<T> select(std::span<T> arr, std::size_t k) noexcept
{
    if (arr.empty()) return std::nullopt;
    T p = get_random_element(std::span{arr});
    auto split_result = split(std::span{arr}, p);
    if (!split_result) return std::nullopt;
    auto [l, r] = *split_result;
    if (k < l) return select(arr.subspan(0, l), k);
    if (k <= r) return p;
    return select(arr.subspan(r + 1), k - r - 1);
}
}  // namespace accessed_homework