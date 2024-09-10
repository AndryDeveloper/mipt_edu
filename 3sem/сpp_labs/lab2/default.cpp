template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;
private:
    T * const data;
    size_type const y_size, x_size;
    Grid(T *data, size_type y_size, size_type x_size): 
        data(data), y_size(y_size), x_size(x_size) { }

    Grid(Grid<T> const &) = delete;
    Grid(Grid<T>&&) = delete;
    Grid<T>& operator=(Grid<T>&) = delete;
    Grid<T>& operator=(Grid<T>&&) = delete;

    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
        }

    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
        }

    Grid<T>& operator=(T const &t) {
        for (
            auto it = data, end = data + x_size * y_size;
            it != end; ++it
            ) *it = t;
            return *this;
            }
            size_type get_y_size() const { return y_size; }
            size_type get_x_size() const { return x_size; }
            };

int main(){
    return 0;
}