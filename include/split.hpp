#ifndef SPLIT_HPP
#define SPLIT_HPP

#include <algorithm>
#include <optional>
#include <ranges>
#include <span>

template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

template <Comparable T>
[[nodiscard("UNUSED SPLIT - LOW, HIGH")]] std::optional<std::pair<std::size_t, std::size_t>> split(std::span<T> a, const T& p) noexcept
{
    if (a.empty()) return std::nullopt;
    std::size_t low = 0, mid = 0, high = a.size() - 1;
    while (mid <= high)
    {
        if (a[mid] < p)
        {
            std::ranges::swap(a[low], a[mid]);
            ++low;
            ++mid;
        }
        else if (a[mid] == p)
        {
            ++mid;
        }
        else
        {
            std::ranges::swap(a[mid], a[high]);
            --high;
        }
    }

    return (low > high) ? std::nullopt : std::make_optional(std::pair{low, high});
}

#endif  // SPLIT_HPP