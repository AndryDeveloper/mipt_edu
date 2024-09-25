#include <iostream>


template <typename T, std::size_t N>
class Grid {
private:
    size_t dimentions[N];
    size_t mult_dimentions[N];
    size_t total_size;
    T* data;

public:
    template<typename First, typename... Args>
    Grid(T&& t, First f, Args... args) {
        if constexpr (std::is_same_v<T, First> && sizeof...(args) + 2 == N){
            dimentions[0] = t;
            dimentions[1] = f;
            size_t index = 2;
            (..., (dimentions[index++] = args));
        }
        else {
            dimentions[0] = f;
            size_t index = 1;
            (..., (dimentions[index++] = args));
        }

        total_size = 1;
        for (size_t i = 0; i < N; ++i){
            total_size *= dimentions[i];
        }

        mult_dimentions[N - 1] = 1;
        for (size_t i = 0; i < N - 1; ++i){
            mult_dimentions[N - i - 2] = mult_dimentions[N - i - 1] * dimentions[N - i - 1];
        }

        data = new T[total_size];

        for (size_t i = 0; i < total_size; ++i){
            if constexpr (std::is_same_v<T, First> && sizeof...(args) + 2 == N){
                data[i] = T();
            }
            else {
                data[i] = t;
            }
        }
    }

    template<typename... Args>
    Grid(Args... args) {
        size_t index = 0;
        (..., (dimentions[index++] = args));

        total_size = 1;
        for (size_t i = 0; i < N; ++i){
            total_size *= dimentions[i];
        }

        data = new T[total_size];

        for (size_t i = 0; i < total_size; ++i){
            data[i] = T();
        }

        mult_dimentions[N - 1] = 1;
        for (size_t i = 0; i < N - 1; ++i){
            mult_dimentions[N - i - 2] = mult_dimentions[N - i - 1] * dimentions[N - i - 1];
        }
    }

    ~Grid() {
        delete[] data;
    }

    template <std::size_t DimIndex>
    class Dim {
    private:
        Grid<T, N>& array;
        size_t currentIndex;

    public:
        Dim(Grid<T, N>& arr, size_t index) 
            : array(arr), currentIndex(index) {}

        auto operator[](size_t index) {
            if constexpr (DimIndex + 3 < N) {
                return Dim<DimIndex + 1>(array, currentIndex + index * array.mult_dimentions[DimIndex]);
            } 
            else {
                return LastDim(array, currentIndex + index * array.mult_dimentions[DimIndex]);
            }
        }
    };


    class LastDim {
    private:
        Grid<T, N>& array;
        size_t currentIndex;
    
    public:
        LastDim(Grid<T, N>& arr, size_t index) : array(arr), currentIndex(index){ }

        T operator[](size_t index) const {
            return array.data[currentIndex + index * array.mult_dimentions[N - 1]];
        }

        T& operator[](size_t index) {
            return array.data[currentIndex + index * array.mult_dimentions[N - 1]];
        }
    };

    Dim<0> operator[](size_t index) {
        return Dim<0>(*this, index);
    }
};

int main() {
    Grid<int, 3> arr(3, 4, 5);

    // Пример использования
    arr[1][2][0] = 42;
    std::cout << arr[1][0][0] << std::endl; // Вывод 0
    std::cout << arr[1][2][0] << std::endl; // Вывод: 42
    std::cout << std::endl;

    Grid<int, 3> arr2(10, 3, 4, 5); // 3D массив

    // Пример использования
    arr2[1][2][0] = 42;
    std::cout << arr2[1][0][0] << std::endl; // Вывод 10
    std::cout << arr2[1][2][0] << std::endl; // Вывод: 42
    std::cout << std::endl;

    Grid<double, 3> arr3(10., 3, 4, 5); // 3D массив

    // Пример использования
    arr3[1][2][0] = 42;
    std::cout << arr3[1][0][0] << std::endl; // Вывод 10
    std::cout << arr3[1][2][0] << std::endl; // Вывод: 42

    return 0;
}