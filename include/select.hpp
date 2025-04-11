#include <iostream>
#include <optional>
#include <random>
#include <span>
#include <vector>

#include "split.hpp"

namespace accessed_homework
{
// Функция get_random_element:
// Вход:
//  - arr[0..n-1]: - массив из n ≥ 1 элемента
// Выход: случайный элемент arr[0..n-1] (рановероятно)
template <typename T>
const T& get_random_element(std::span<T> arr) noexcept
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, arr.size() - 1);
    return arr[dist(gen)];
}

// Функция select:
// Вход:
//  - arr[0..n-1]: массив из n ≥ 1 элемента типа T
//  - k: индекс искомого порядкового статистического элемента, k ∈ 0..n-1
//
// Выход:
//  - std::optional<T>:
//      * Значение - если массив не пуст и индекс корректный (k ∈ 0..n-1)
//      * std::nullopt - если массив пуст
//
// Побочные эффекты:
//  - Модифицирует (переставляет) элементы массива `arr` в процессе выполнения
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