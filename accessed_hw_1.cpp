#include <cstdlib>
#include <ctime>
#include <iostream>
#include <utility>
#include <vector>

template <class T>

std::pair<size_t, size_t> split(std::vector<T> &a, T p) {
  int low = 0, mid = 0, high = a.size() - 1;
  while (mid <= high) {
    if (a[mid] < p) {
      std::swap(a[low], a[mid]);
      low++;
      mid++;
    } else if (a[mid] == p) {
      mid++;
    } else {
      std::swap(a[mid], a[high]);
      high--;
    }
  }
  if (low > high)
    return {-1, -1};
  return {low, high};
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