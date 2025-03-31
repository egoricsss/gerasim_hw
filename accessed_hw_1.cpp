#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

template <typename T>
concept Comparable = requires(T a, T b) {
  { a < b } -> std::convertible_to<bool>;
  { a == b } -> std::convertible_to<bool>;
};

template <Comparable T>
std::optional<std::pair<std::size_t, std::size_t>> split(std::vector<T> &a,
                                                         T p) {
  if (a.empty())
    return std::nullopt;

  std::size_t low = 0, mid = 0, high = a.size() - 1;

  while (mid <= high) {
    if (a[mid] < p) {
      std::ranges::swap(a[low], a[mid]);
      ++low;
      ++mid;
    } else if (a[mid] == p) {
      ++mid;
    } else {
      std::ranges::swap(a[mid], a[high]);
      high = (high == 0) ? 0 : high - 1
    }
  }

  return (low > high) ? std::nullopt : std::make_optional({low, high});
}

int main() {
  size_t n;
  std::cin >> n;
  std::vector<int> arr(n);

  for (auto &arr_el : arr)
    std::cin >> arr_el;

  int p;
  std::cin >> p;

  auto [low, high] = split(arr, p);
  for (int arr_el : arr)
    std::cout << arr_el << ' ';

  std::cout << std::endl << low << ' ' << high << std::endl;

  return 0;
}