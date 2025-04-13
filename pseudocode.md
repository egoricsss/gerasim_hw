# Домашняя работа №1

**Студент:** Сыров Егор Романович

**Группа:** 5130901/30005

# Псевдокод и спецификация алгоритмов

## Функция `split`

**Назначение:**

Реорганизует массив на месте, разделяя его на три части:

- элементы `< p`,
- элементы `== p`,
- элементы `> p`.

**Вход:**

- `arr`: массив элементов (`arr[0..n-1]`), допускающих операции определения порядка,
- `p`: опорный элемент.

**Выход:**

- `(l, r)` - границы отрезка элементов, равных `p`:
  - `arr[0..l-1] < p`,
  - `arr[l..r] == p`,
  - `arr[r+1..n-1] > p`.
- `std::nullopt`, если массив пуст.

**Побочный эффект:**

- Изменяет (переставляет) порядок элементов массива `arr`, группируя элементы в  заданном порядке.

``````pseudocode
Функция split(arr[1..n], p)
// Вход: массив arr[1..n] из n ≥ 1 элементов, допускающих сравнение;
// опорный элемент p ∈ arr[1..n]
// Выход: пара индексов (l, r), таких что:
//        - arr[1 .. l−1] < p
//        - arr[l .. r] == p
//        - arr[r+1 .. n] > p
// Побочные эффекты: переставляет элементы массива arr на месте
    i ← 1
    // Первый проход: выносим все элементы < p в начало массива
    для j от 1 до n:
        если arr[j] < p:
            обмен arr[i] и arr[j]
            i ← i + 1
    l ← i
    // Второй проход: выносим все элементы == p после элементов < p
    для j от l до n:
        если a[j] == p:
            обмен arr[i] и arr[j]
            i ← i + 1
    r ← i − 1
    вернуть (l, r)
``````

## Функция `select`

**Назначение**

Находит **k-й порядковый статистический элемент** массива - то есть элемент, который оказался бы на позиции `k` после сортировки (по неубыванию).

**Вход:**

- `arr`: массив элементов (`arr[0..n-1]`), допускающих сравнение,
- `k`: индекс порядковой статистики, `0 ≤ k < n`.

**Выход:**

- Элемент, который находится на позиции `k` в отсортированном массиве,

- `nullopt`, если массив пуст.

**Побочный эффект:**

- Модифицирует (переставляет массив `arr`).

``````pseudocode
функция select(arr[1..n], k)
// Вход: массив arr[1..n] из n ≥ 1 элементов, допускающих сравнение; 
// целое k ∈ 1..n — позиция порядковой статистики (по возрастанию)
// Выход: значение, стоящее в отсортированном массиве arr на позиции k
// Побочные эффекты: изменяет массив a (переставляет элементы)

    выбрать случайный индекс j из 1..n
    p ← arr[j]
    (l, r) ← split(arr, p)
    если k < l:
        вернуть select(arr[1 .. l−1], k)
    если k ≤ r:
        вернуть p
    иначе:
        вернуть select(arr[r+1 .. n], k − r)
``````

# Доказательство корректности функции `split`

##  1. Формулировка

Функция `split` делит массив на три части:

1. Элементы **меньше** опорного значения `p`.
2. Элементы **равные** `p`.
3. Элементы **больше** `p`.

**Пример:**

Пусть массив `[3, 1, 4, 1, 5]` и `p = 1`.

После `split` должно получиться:

- `< p`: `[ ]` (пусто, так как 1 - минимальный элемент).
- `== p`: `[1, 1]`.
- `> p`: `[3, 4, 5]`.

## 2. Как работает функция `split`

Функция выполняет два последовательных цикла:

1. **Первый цикл**: Собирает элементы `< p` в начале массива.
2. **Второй цикл**: Собирает элементы `== p` после элементов `< p`.

**Код функции:**

``````c++
template <Comparable T>
std::optional<std::pair<std::size_t, std::size_t>> split(std::span<T> arr, const T& p) {
    if (arr.empty()) return std::nullopt;

    std::size_t n = arr.size();
    std::size_t i = 0;

    // Первый цикл: элементы < p
    for (std::size_t j = 0; j < n; ++j) {
        if (arr[j] < p) {
            std::ranges::swap(arr[i], arr[j]);
            ++i;
        }
    }

    std::size_t l = i;

    // Второй цикл: элементы == p
    for (std::size_t j = i; j < n; ++j) {
        if (arr[j] == p) {
            std::ranges::swap(arr[i], arr[j]);
            ++i;
        }
    }

    return std::make_optional(std::pair{l, i - 1});
}
``````

## 3. Доказательство корректности 

 **Первый цикл**: элементы `< p`.

**Инвариант цикла:**

- Перед каждой итерацией:
  - `arr[0..i-1]` содержит элементы `< p`.
  - `arr[i..j-1]` содержит элементы `≥ p`.

**Доказательство инварианта:**

- **База:** Перед началом цикла `i = 0`, `j = 0`. Оба сегмента пусты $\implies$ инвариант выполняется.
- **Шаг цикла:**
  - Если `arr[j] < p`:
    - Меняем местами `arr[i]` и `arr[j]`.
    - Теперь `arr[i]` гарантированно `< p`.
    - Увеличиваем `i` на 1 $\implies$ сегмент `[0..i-1]` остаётся корректным.

  - Если `arr[j] ≥ p`:
    - Просто увеличиваем `j` $\implies$ сегмент `[i..j-1]` расширяется, сохраняя условие `≥ p`.

- **После цикла:**
  - `j` достигает `n` $\implies$ все элементы обработаны.
  - `arr[0..i-1]` содержит все элементы `< p`.
  - Остальные элементы `≥ p`.


**Результат:** `l = i` $\implies$ граница между `< p` и `≥ p`.

---

**Второй цикл**: элементы `== p`.

**Инвариант цикла:**

- Перед каждой итерацией:
  - `arr[0..l-1]` содержит элементы `< p`.
  - `arr[l..i-1]` содержит элементы `== p`.
  - `arr[i..j-1]` содержит элементы `> p`.

**Доказательство инварианта:**

- **База:** Перед началом цикла `i = 1`, `j = 1`. Сегменты `[l..i-1]` и `[i..j-1]` пусты $\implies$ инвариант выполняется.
- **Шаг цикла:**
  - Если `arr[j] == p`:
    - Меняем местами `arr[i]` и `arr[j]`.
    - Теперь `arr[i]` гарантированно `== p`.
    - Увеличиваем `i` на 1 $\implies$ сегмент  `[l..i-1]` остаётся корректным.
  - Если `arr[j] > p`:
    - Просто увеличиваем `j` $\implies$ сегмент `[i..j-1]` расширяется, сохраняя условие `> p`.
- **После цикла:**
  - `j` достигает `n` $\implies$ все элементы обработаны.
  - `arr[l..i-1]` содержит все элементы `== p`.
  - Остальные элементы `> p`.

**Результат:** `r = i - 1` $\implies$ граница между `== p` и `> p`.

## 4. Время работы алгоритма 

- **Первый цикл:**
  - Выполняется `n` итераций (от `j = 0` до `j = n-1`).
  - В каждой итерации:
    - Сравнение (`arr[j] < p`) - $O(1)$.
    - Обмен и инкремент - $O(1)$.
  - **Итого:** $O(n)$.
- **Второй цикл:**
  - Выполняется `n - 1` итераций (от `j = l` до `j = n-1`).
  - В каждой итерации:
    - Сравнение (`arr[j] == p`) - $O(1)$.
    - Обмен и инкремент - $O(1)$.
  - **Итого:** $O(n)$.

**Общее время работы:** $O(n) + O(n) = O(n)$.

## 5. Проверка крайних случаев

**Случай 1:** Все элементы `<` **p**.

- Первый цикл переместит все элементы в начало $\implies$ `l = n-1`
- Второй цикл не выполнится $\implies$ `r = l - 1 = n - 1`.
- Результат: `< p` - весь массив, кроме `p`, `== p` - `[p]`, `> p`  - `[]` (`l = r = n-1`).

**Случай 2:** Все элементы `==` **p**.

- Первый цикл не переместит элементы $\implies$ `l = 0`.
- Второй цикл переместит все элементы $\implies$ `r = n-1`.
- Результат: `> p` - весь массив (`l = 0`, `r = n-1`).

**Случай 3:** Все элементы `>` **p**.

- Первый цикл не переместит элементы $\implies$ `l = 0`.
- Второй цикл не переместит элементы $\implies$ `r = l = 0`.
- Результат: `> p` - весь массив (`l = r = 0`).

****

# Исходный код 

## `main.cpp`

```c++
#include <iostream>
#include <limits>
#include <span>
#include <stdexcept>
#include <vector>

#include "bfprt.hpp"
#include "select.hpp"

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
            std::cout << *median << "\n";
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
```

## `split.hpp`

``````c++
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
``````

## `select.hpp`

``````c++
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
``````

# Пример работы 

## Набор входных данных №1

**stdin**

``````bash
5
9 7 3 7 8
7
``````

**stdout**

``````bash
3 7 7 9 8 
1 2
Median: 7
``````

## Набор входных данных №2

**stdin**

``````bash
15
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 
8
``````

**stdout**

``````bash
7 6 5 4 3 2 1 8 15 14 13 12 11 10 9 
7 7
Median: 8
``````

## Набор входных данных №3

**stdin**

``````bash
17
12 13 14 15 1 2 3 4 5 100 101 102 103 51 50 49 48
48
``````

**stdout**

``````bash
12 13 14 15 1 2 3 4 5 48 101 102 103 51 50 49 100 
9 9
Median: 15
``````



# Дополнительные источники 

- https://dl.spbstu.ru/course/view.php?id=6648 (курс лекций "Алгоритмы и  Структуры данных" Герасимов А.С.)
- https://en.cppreference.com/w/cpp/standard_library (Документация C++ Standard Library)

- https://www.geeksforgeeks.org/stdmt19937-class-in-cpp/ (работа с std::mt19937 - генерация случайных чисел в заданном диапазоне)