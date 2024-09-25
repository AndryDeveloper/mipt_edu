#include <iostream>
#include <vector>

template <typename T, std::size_t N>
class MultiDimArray {
private:
    std::vector<size_t> dimensions; // Размеры каждого измерения
    std::vector<T> data; // Хранилище данных

    size_t computeIndex(const std::array<size_t, N>& indices) const {
        size_t index = 0;
        size_t stride = 1;
        for (int i = N - 1; i >= 0; --i) {
            index += indices[i] * stride;
            stride *= dimensions[i];
        }
        return index;
    }

public:
    MultiDimArray(const std::array<size_t, N>& dims) : dimensions(dims.begin(), dims.end()) {
        size_t totalSize = 1;
        for (auto dim : dimensions) {
            totalSize *= dim;
        }
        data.resize(totalSize);
    }

    T& operator[](const std::array<size_t, N>& indices) {
        return data[computeIndex(indices)];
    }

    const T& operator[](const std::array<size_t, N>& indices) const {
        return data[computeIndex(indices)];
    }

    // Метод для работы с динамическим количеством измерений
    template <std::size_t DimIndex>
    class Dim {
    private:
        MultiDimArray<T, N>& array;
        size_t currentIndex;

    public:
        Dim(MultiDimArray<T, N>& arr, size_t index) 
            : array(arr), currentIndex(index) {}

        auto operator[](size_t index) {
            if constexpr (DimIndex < N - 1) {
                return Dim<DimIndex + 1>(array, array.computeIndex({currentIndex, index}));
            } else {
                return array.data[array.computeIndex({currentIndex, index})];
            }
        }
    };

    Dim<0> operator[](size_t index) {
        return Dim<0>(*this, index);
    }
};

int main() {
    MultiDimArray<int, 3> arr({3, 4, 5}); // 3D массив

    // Пример использования
    arr[{1, 2, 0}] = 42;
    std::cout << arr[{1, 2, 0}] << std::endl; // Вывод: 42

    return 0;
}
