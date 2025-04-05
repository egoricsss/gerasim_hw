#ifndef BFPRT_HPP
#define BFPRT_HPP

#include <optional>
#include <ranges>
#include <span>
#include <vector>

#include "split.hpp"

namespace detail
{
template <Comparable T>
constexpr void sorting_network(std::span<T> arr) noexcept
{
    switch (arr.size())
    {
        case 2:
            if (arr[0] > arr[1]) std::ranges::swap(arr[0], arr[1]);
            break;
        case 3:
            if (arr[0] > arr[1]) std::ranges::swap(arr[0], arr[1]);
            if (arr[1] > arr[2]) std::ranges::swap(arr[1], arr[2]);
            if (arr[0] > arr[1]) std::ranges::swap(arr[0], arr[1]);
            break;
        case 4:
            if (arr[0] > arr[1]) std::ranges::swap(arr[0], arr[1]);
            if (arr[2] > arr[3]) std::ranges::swap(arr[2], arr[3]);
            if (arr[0] > arr[2]) std::ranges::swap(arr[0], arr[2]);
            if (arr[1] > arr[3]) std::ranges::swap(arr[1], arr[3]);
            if (arr[1] > arr[2]) std::ranges::swap(arr[1], arr[2]);
            break;
        case 5:
            if (arr[0] > arr[1]) std::ranges::swap(arr[0], arr[1]);
            if (arr[3] > arr[4]) std::ranges::swap(arr[3], arr[4]);
            if (arr[0] > arr[3]) std::ranges::swap(arr[0], arr[3]);
            if (arr[1] > arr[4]) std::ranges::swap(arr[1], arr[4]);
            if (arr[1] > arr[2]) std::ranges::swap(arr[1], arr[2]);
            if (arr[2] > arr[3]) std::ranges::swap(arr[2], arr[3]);
            if (arr[1] > arr[2]) std::ranges::swap(arr[1], arr[2]);
            break;
    }
}
}  // namespace detail

namespace bfprt
{
template <Comparable T>
[[nodiscard("UNUSED MEDIAN OF MEDIANS")]] std::optional<T> select(std::span<T> arr, std::size_t k) noexcept
{
    if (arr.size() <= 5)
    {
        detail::sorting_network(std::span{arr});
        return arr[k];
    }

    std::vector<T> medians;
    medians.reserve((arr.size() + 4) / 5);
    for (std::size_t i = 0; i < arr.size(); i += 5)
    {
        std::size_t len = std::min<std::size_t>(5, arr.size() - i);
        std::span<T> group = arr.subspan(i, len);
        detail::sorting_network(group);
        medians.push_back(group[len / 2]);
    }

    auto mom = bfprt::select(std::span{medians}, medians.size() / 2);
    if (!mom) return std::nullopt;

    auto split_result = split(arr, *mom);
    if (!split_result) return std::nullopt;

    auto [low, high] = *split_result;

    if (k < low)
    {
        return bfprt::select(arr.subspan(0, low), k);
    }
    else if (k <= high)
    {
        return mom;
    }
    else
    {
        return bfprt::select(arr.subspan(high + 1), k - high - 1);
    }
}
}  // namespace bfprt
#endif  // BFPRT_HPP