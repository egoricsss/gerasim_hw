#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <optional>
#include <utility>
#include <vector>

template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

template <Comparable T> std::optional<std::pair<std::size_t, std::size_t>> split(std::vector<T> &a, const T &p) {
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
            high = (high == 0) ? 0 : high - 1;
        }
    }

    return (low > high) ? std::nullopt : std::make_optional<std::pair<std::size_t, std::size_t>>(low, high);
}

int main() {
    std::size_t n;
    if (!(std::cin >> n) || n == 0) {
        std::cerr << "Invalid input size\n";
        return EXIT_FAILURE;
    }

    std::vector<int> arr(n);
    std::ranges::for_each(arr, [](auto &el) {
        if (!(std::cin >> el)) {
            std::cerr << "Invalid input element\n";
            std::exit(EXIT_FAILURE);
        }
    });

    int p;
    if (!(std::cin >> p)) {
        std::cerr << "Invalid pivot value\n";
        return EXIT_FAILURE;
    }

    if (auto result = split(arr, p)) {
        const auto &[low, high] = *result;

        std::ranges::copy(arr, std::ostream_iterator<int>(std::cout, " "));
        std::cout << "\n" << low << " " << high << "\n"; // 9. '\n' вместо endl
    } else {
        std::cerr << "Partitioning failed\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}