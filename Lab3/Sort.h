//
// Created by Takex on 25.12.2022.
//

#ifndef RIDERS_LAB2_SORT_H
#define RIDERS_LAB2_SORT_H

#include <algorithm>
#include <stdexcept>

static constexpr int insertionSortThreshold = 16; // 12 -> 2^4

template<typename T, typename Compare>
inline void insertion_sort(T *first, T *last, Compare comp) {
    for (auto i = first; i <= last; i++) {
        auto j = i - 1;

        T value = *(j + 1);
        while (j >= first && comp(value, *j)) {
            *(j + 1) = std::move(*j);
            j--;
        }
        *(j + 1) = value;
    }
}

template<typename T, typename Compare>
void sort(T *first, T *last, Compare comp) {
    if (!first || !last) throw std::runtime_error("nullptr was passed");

    last--;
    while (first < last) {
        if (last - first < insertionSortThreshold) {
            insertion_sort(first, last, comp);
            return;
        }

        T *middle = first + (last - first) / 2;

        if (comp(*last, *first))
            std::swap(*last, *first);

        if (comp(*middle, *first))
            std::swap(*middle, *first);

        if (comp(*last, *middle))
            std::swap(*last, *middle);

        std::swap(*middle, *(last - 1));

        auto pivot = *(last - 1);

        auto i = first;
        auto j = last - 1;

        while (true) {
            while (comp(*(++i), pivot));
            while (comp(pivot, *(--j)));
            if (i >= j) break;

            std::swap(*(i), *(j));
        }
        std::swap(*(last - 1), *(i));
        *(i) = pivot;

        if (i - first < last - i) {
            sort(first, i, comp);
            first = i + 1;
        } else {
            sort(i + 1, last + 1, comp);
            last = i - 1;
        }
    }
}

#endif //RIDERS_LAB2_SORT_H
