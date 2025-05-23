#ifndef SPLIT_HPP
#define SPLIT_HPP

#include <algorithm>
#include <optional>
#include <ranges>
#include <span>

namespace accessed_homework
{

// Концепт, определяющий требования: тип T должен поддерживать операции сравнения (`<`, `==`)
template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

// Функция split:
// Вход:
//  - arr[0..n-1]: непустой массив сравнимых элементов (std::span<T>), где n ≥ 1
//  - p: опорный элемент, с которым сравниваются элементы массива, где p ∈ arr[0..n-1]
//
// Выход:
//  - Если массив не пуст, возвращает пару индексов (l, r),
//    таких что после завершения функции элемены массива `arr` переупорядочены следующим образом:
//      * a[0 .. l-1] < p
//      * a[l .. r] == p
//      * a[r .. n-1] > p, где n - кол-во элементов в массиве `arr`
//
//  - Если массив пуст, возвращает std::nullopt
//
// Побочные эффекты:
//  - Изменяет массив `a` на месте: переставляет элементы так, чтобы
//    реализовать трёхстороннее разбиение по значению `p`
template <Comparable T>
[[nodiscard("UNUSED SPLIT - L, R")]]
std::optional<std::pair<std::size_t, std::size_t>> split(std::span<T> arr, const T& p) noexcept
{
    if (arr.empty()) return std::nullopt;

    std::size_t n = arr.size();
    std::size_t i = 0;

    for (std::size_t j = 0; j < n; ++j)
    {
        if (arr[j] < p)
        {
            std::ranges::swap(arr[i], arr[j]);
            ++i;
        }
    }

    std::size_t l = i;
    for (std::size_t j = i; j < n; ++j)
    {
        if (arr[j] == p)
        {
            std::ranges::swap(arr[i], arr[j]);
            ++i;
        }
    }

    return std::make_optional(std::pair{l, i - 1});
}
}  // namespace accessed_homework
#endif  // SPLIT_HPP