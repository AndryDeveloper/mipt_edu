#include <iostream>
#include <cassert>

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

    size_t get_axis_size(size_t index){
        return dimentions[index];
    }

    auto operator[](size_t index) const {
        if constexpr(N == 1){
            return Grid<T, N>()
        }
    }
};

int main() {
    Grid<float, 2> g(0.0f, 3, 2);
    assert(3 == g.get_axis_size(0));
    assert(2 == g.get_axis_size(1));

    // Тесты для 2 части

    for (size_t y_idx = 0; y_idx != g.get_axis_size(0); ++y_idx)
        for (size_t x_idx = 0; x_idx != g.get_axis_size(1); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);

    for (size_t y_idx = 0; y_idx != g.get_axis_size(0); ++y_idx)
        for (size_t x_idx = 0; x_idx != g.get_axis_size(1); ++x_idx)
            g[y_idx][x_idx] = 1.0f;

    for (size_t y_idx = 0; y_idx != g.get_axis_size(0); ++y_idx)
        for (size_t x_idx = 0; x_idx != g.get_axis_size(1); ++x_idx)
            assert(1.0f == g[y_idx][x_idx]);

    // Тесты для 3 части
    // Grid<float, 3> g3(1.0f, 2, 3, 4);
    // assert(1.0f == g3[1][1][1]);

    // Grid<float, 2> g2(2.0f, 2, 5);
    // assert(2.0f == g2[1][1]);

    // g2 = g3[1];
    // assert(1.0f == g2[1][1]);
    return 0;
}