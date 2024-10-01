#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;


template <typename T>
class HarmonicSolver{
private:
    char* config_path;
    char* output_path;

    T dt;
    T w;
    T x;
    T v;

    unsigned time_step = 0;
    unsigned n_steps;
    std::ofstream out;

    std::string solver;
public:
    HarmonicSolver(char* config_path, char* output_path): 
        config_path(config_path), output_path(output_path), out(output_path)
    {   
        out << "time,position,velocity,energy" << std::endl;;

        std::ifstream ifs(config_path);
        json jf = json::parse(ifs);

        dt = jf["dt"];
        w = jf["w"];
        x = jf["x_0"];
        v = jf["v_0"];
        n_steps = jf["n_steps"];
        solver = jf["solver"];
    }

    ~HarmonicSolver(){
        out.close();
    }

    void solve(){
        for (time_step = 0; time_step < n_steps; ++time_step){
            if (solver == "euler") euler_step();
            else if (solver == "heun") hoin_step();
            save_data();
        }
    }
private:
    T get_energy(){
        return v * v / 2. + w * w * x * x / 2.;
    }

    void euler_step(){
        T x_;
        T v_;

        x_ = x + dt * v;
        v_ = v - dt * w * w * x;

        x = x_;
        v = v_;
    }

    void hoin_step(){
        T x_, x__, v_, v__;

        x_ = x + dt * v;
        v_ = v - dt * w * w * x;

        x__ = x + dt / 2. * (v + v_);
        v__ = v - dt / 2. * w * w * (x + x_);

        x = x__;
        v = v__;
    }

    void runge_cutta(){

    }

    void save_data(){
        out << time_step * dt << ',' << x << ',' << v << ',' << get_energy() << std::endl;
    }
};


int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cout << "Неправильный ввод" << std::endl;
        return -1;
    }

    HarmonicSolver<double> env(argv[1], argv[2]);
    env.solve();
    return 0;
}