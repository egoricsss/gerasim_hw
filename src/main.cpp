#include <iostream>
#include <limits>
#include <optional>
#include <random>
#include <ranges>
#include <span>
#include <stdexcept>
#include <vector>

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
template <accessed_homework::Comparable T>
std::optional<T> select(std::span<T> arr, std::size_t k) noexcept
{
    if (arr.empty()) return std::nullopt;
    T p = get_random_element(std::span{arr});
    auto split_result = accessed_homework::split(std::span{arr}, p);
    if (!split_result) return std::nullopt;
    auto [l, r] = *split_result;
    if (k < l) return select(arr.subspan(0, l), k);
    if (k <= r) return p;
    return select(arr.subspan(r + 1), k - r - 1);
}
}  // namespace accessed_homework

int main()
{
    try
    {
        std::size_t n;
        if (!(std::cin >> n) || n == 0)
        {
            throw std::invalid_argument("Invalid array size");
        }
        std::vector<int> numbers(n);
        for (auto& el : numbers)
        {
            if (!(std::cin >> el))
            {
                throw std::invalid_argument("Invalid array element");
            }
        }
        int pivot;
        if (!(std::cin >> pivot))
        {
            throw std::invalid_argument("Invalid pivot element");
        }
        auto split_result = accessed_homework::split(std::span{numbers}, pivot);
        if (split_result)
        {
            const auto& [low, high] = *split_result;
            for (const auto& num : numbers)
            {
                std::cout << num << " ";
            }
            std::cout << "\n" << low << " " << high << "\n";
        }
        else
        {
            std::cerr << "Error: partitioning failed\n";
        }
        const std::size_t median_pos = numbers.size() / 2;
        if (auto median = accessed_homework::select(std::span{numbers}, median_pos))
        {
            std::cout << "median: " << *median << "\n";
        }
        else
        {
            std::cerr << "Error: median selection failed\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}