#include <iostream>
#include <cassert>

template <typename T, size_t... Dims>
class Grid;


template <typename T, size_t N>
class Grid<T, N> {
private:
    T elements[N];

public:
    Grid(const T& t){
        std::fill(elements, elements + N, t);
    }

    Grid(){
        std::fill(elements, elements + N, T());
    }

    T operator[](size_t idx) const {
        return elements[idx];
    }

    T& operator[](size_t idx) {
        return elements[idx];
    }

    T operator()(size_t idx) const {
    return elements[idx];
    }

    T& operator()(size_t idx) {
        return elements[idx];
    }

    size_t get_axis_size(){
        return N;
    }
};


template <typename T, size_t N, size_t... Dims>
class Grid<T, N, Dims...> {
private:
    Grid<T, Dims...> subgrids[N];
    size_t dimentions[sizeof...(Dims) + 1];

public:
    Grid(const T& t) {
        std::fill(subgrids, subgrids + N, Grid<T, Dims...>(t));

        dimentions[0] = N;
        size_t index = 1;
        (..., (dimentions[index++] = Dims));
    }

    Grid() {
        std::fill(subgrids, subgrids + N, Grid<T, Dims...>());

        dimentions[0] = N;
        size_t index = 1;
        (..., (dimentions[index++] = Dims));
    }

    Grid<T, Dims...> operator[](size_t idx) const {
        return subgrids[idx];
    }

    Grid<T, Dims...>& operator[](size_t idx) {
        return subgrids[idx];
    }

    template<typename... Idxs>
    auto operator()(size_t idx, Idxs... idxs) const {
        return subgrids[idx](idxs...);
    }

    template<typename... Idxs>
    auto& operator()(size_t idx, Idxs... idxs) {
        return subgrids[idx](idxs...);
    }

    size_t get_axis_size(size_t idx){
        return dimentions[idx];
    }
};

int main() {
    // 2 часть
    Grid<float, 3, 2> g(0.0f);
    assert(3 == g.get_axis_size(0));
    assert(2 == g.get_axis_size(1));

    for (size_t y_idx = 0; y_idx != g.get_axis_size(0); ++y_idx)
        for (size_t x_idx = 0; x_idx != g.get_axis_size(1); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);

    for (size_t y_idx = 0; y_idx != g.get_axis_size(0); ++y_idx)
        for (size_t x_idx = 0; x_idx != g.get_axis_size(1); ++x_idx)
            g[y_idx][x_idx] = 1.0f;

    for (size_t y_idx = 0; y_idx != g.get_axis_size(0); ++y_idx)
        for (size_t x_idx = 0; x_idx != g.get_axis_size(1); ++x_idx)
            assert(1.0f == g(y_idx, x_idx));
    

    // 3 часть
    Grid<float, 2, 3, 4> g3(1.0f);
    assert(1.0f == g3(1, 1, 1));

    Grid<float, 3, 4> g2(2.0f);
    assert(2.0f == g2(1, 1));

    g2 = g3[1];
    assert(1.0f == g2(1, 1));
    return 0;
}