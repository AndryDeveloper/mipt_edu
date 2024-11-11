#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <initializer_list>
#include <algorithm>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

typedef float calc_t;

template <typename T>
class State{
private:
    size_t size = 0;
    T* state = nullptr;

public:
    State() = default;
    State(size_t size): size(size), state(new T[size]) {}

    State(std::initializer_list<T> list): size(list.size()) {
        state = new T[size];
        std::copy(list.begin(), list.end(), state);
    }

    State(State<T>&& other): size(other.size), state(other.state){
        other.state = nullptr;
        other.size = 0;
    }

    State(const State<T>& other) : size(other.size), state(new T[other.size]) {
        for (size_t i = 0; i < size; ++i) {
            state[i] = other.state[i];
        }
    }

    State& operator=(State&& other) {
        if (this != &other) {
            delete state;
            state = other.state;
            other.state = nullptr;
            size = other.size;
            other.size = 0;
        }
        return *this;
    }

    ~State(){
        delete[] state;
    }

    T& operator[](size_t index){
        return state[index];
    }

    T operator[](size_t index) const{
        return state[index];
    }

    size_t length() const{
        return size;
    }

    std::string visualize(){
        std::ostringstream oss;
        for (size_t i = 0; i < size; ++i) {
            if (i != 0) oss << ",";
            oss << state[i];
        }
        return oss.str();
    }
};


template <typename T>
class Equation{
public:
    virtual State<T> dfdt([[maybe_unused]] T t, const State<T>& state) const = 0;
    virtual ~Equation() = default;
};


template <typename T>
class Solver{
public:
    virtual State<T> step(T dt, T t, const State<T>& state, const Equation<T>& equation) const = 0;
    virtual ~Solver() = default;
};


template <typename T>
class SinPend: public Equation<T> {
private:
    T w_0;
    T gamma;
    T f_0;
    T w;

public:
    SinPend(T w_0, T gamma, T f_0, T w): w_0(w_0), gamma(gamma), f_0(f_0), w(w) {}

    State<T> dfdt([[maybe_unused]] T t, const State<T>& state) const override {
        State<T> diff_state(state.length());
        diff_state[0] = state[1];
        diff_state[1] = - w_0 * w_0 * state[0] - gamma * state[1] + f_0 * std::cos(w * t);
        return diff_state;
    }
};


template <typename T>
class SquarePend: public Equation<T> {
private:
    T w_0;
    T gamma;
    T f_0;
    T T_0;

public:
    SquarePend(T w_0, T gamma, T f_0, T w): w_0(w_0), gamma(gamma), f_0(f_0), T_0(2 * M_PI / w) {}

    State<T> dfdt([[maybe_unused]] T t, const State<T>& state) const override {
        State<T> diff_state(state.length());
        diff_state[0] = state[1];
        if (std::fmod(t, T_0) < T_0 / 2.){
            diff_state[1] = - w_0 * w_0 * state[0] - gamma * state[1] + f_0;
        }
        else{
            diff_state[1] = - w_0 * w_0 * state[0] - gamma * state[1] - f_0;
        }
        return diff_state;
    }
};


template <typename T>
class Euler: public Solver<T> {
public:
    State<T> step(T dt, T t, const State<T>& state, const Equation<T>& equation) const override {
        State<T> diff_state(equation.dfdt(t, state));
        State<T> new_state(state.length());

        for (size_t i = 0; i < state.length(); ++i){
            new_state[i] = state[i] + dt * diff_state[i];
        }
        return new_state;
    }
};


template <typename T>
class RK4: public Solver<T> {
public:
    State<T> step(T dt, T t, const State<T>& state, const Equation<T>& equation) const override {
        State<T> new_state(state);
        // k1
        State<T> k1(equation.dfdt(t, new_state));

        //k2
        for (size_t i = 0; i < state.length(); ++i){
            new_state[i] = state[i] + dt / 2. * k1[i];
        }
        State<T> k2(equation.dfdt(t + dt / 2., new_state));

        //k3
        for (size_t i = 0; i < state.length(); ++i){
            new_state[i] = state[i] + dt / 2. * k2[i];
        }
        State<T> k3(equation.dfdt(t + dt / 2., new_state));

        //k4
        for (size_t i = 0; i < state.length(); ++i){
            new_state[i] = state[i] + dt * k3[i];
        }
        State<T> k4(equation.dfdt(t + dt, new_state));

        for (size_t i = 0; i < state.length(); ++i){
            new_state[i] = state[i] + dt / 6. * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]);
        }

        return new_state;
    }
};


int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cout << "Неправильный ввод" << std::endl;
        return -1;
    }

    std::ifstream ifs(argv[1]);
    json jf = json::parse(ifs);

    calc_t w = jf["w"];
    calc_t w_0 = jf["w_0"];
    calc_t f_0 = jf["f_0"];
    calc_t gamma = jf["gamma"];

    calc_t x = jf["x_0"];
    calc_t v = jf["v_0"];
    calc_t dt = jf["dt"];
    size_t n_steps = jf["n_steps"];

    State<calc_t>* states = new State<calc_t>[n_steps]{{x, v}};

    Solver<calc_t>* solver = nullptr;
    Equation<calc_t>* eq = nullptr;

    if (jf["solver"] == "RK4"){
        solver = new RK4<calc_t>();
    }
    else if (jf["solver"] == "Euler"){
        solver = new Euler<calc_t>();
    }

    if (jf["equation"] == "sin"){
        eq = new SinPend<calc_t>(w_0, gamma, f_0, w);
    }
    else if (jf["equation"] == "sq1"){
        eq = new SquarePend<calc_t>(w_0, gamma, f_0, w);
    }

    calc_t t = 0;
    for (size_t step = 1; step < n_steps; ++step){
        states[step] = solver->step(dt, t, states[step - 1], *eq);
        t += dt;
    }

    delete eq;
    delete solver;

    std::ofstream out(argv[2]);
    out << "time,position,velocity" << std::endl;;
    for (size_t i = 0; i < n_steps; ++i){
        out << i * dt << ',' << states[i].visualize() << std::endl;
    }
    out.close();

    delete[] states;
    return 0;
}