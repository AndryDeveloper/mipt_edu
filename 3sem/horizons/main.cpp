#include <cmath>
#include <random>
#include <iostream>
#include <fstream>
#include <optional>
#include <nlohmann/json.hpp>
#define _USE_MATH_DEFINES

using json = nlohmann::json;

class Vector3d{
public:
    double x;
    double y;
    double z;

    Vector3d(): x(0), y(0), z(0) {}
    Vector3d(double x, double y, double z): x(x), y(y), z(z) {}

double norm() {
    return std::sqrt(x*x + y*y + z*z);
}
};

Vector3d operator%(const Vector3d &a, const double b){
    return Vector3d((a.x > 0) ? std::fmod(a.x, b) : std::fmod(a.x, b) + b, 
                    (a.y > 0) ? std::fmod(a.y, b) : std::fmod(a.y, b) + b, 
                    (a.z > 0) ? std::fmod(a.z, b) : std::fmod(a.z, b) + b);
}

Vector3d operator+(const Vector3d &a, const Vector3d &b){
    return Vector3d(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3d operator-(const Vector3d &a, const Vector3d &b){
    return Vector3d(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3d operator-(const Vector3d &a){
    return Vector3d(-a.x, -a.y, -a.z);
}

Vector3d operator*(const double a, const Vector3d &vec){
    return Vector3d(a*vec.x, a*vec.y, a*vec.z);
}

Vector3d operator*(const Vector3d &vec, const double a){
    return Vector3d(a*vec.x, a*vec.y, a*vec.z);
}

Vector3d operator/(const Vector3d &vec, const double a){
    return Vector3d(vec.x / a, vec.y / a, vec.z / a);
}


class Molecule{
public:
    Vector3d position;
    Vector3d velocity;
    double mass;

    Molecule(Vector3d position, Vector3d velocity, double mass): 
        position(position), velocity(velocity), mass(mass) {}
    
    void add_force(Vector3d force){
        force_ = force_ + force;
    }

    void reset_force(){
        force_prev_ = force_;
        force_ = Vector3d();
    }

    Vector3d get_accelerate(){
        return force_ / mass;
    }

    Vector3d get_prev_accelerate(){
        return force_prev_ / mass;
    }
private:
    Vector3d force_ = Vector3d();
    Vector3d force_prev_ = Vector3d();
};


class Environment{
public:
    Environment(char* config_path, char* coords_path, char* distribution_path, char* energy_path): 
        config_path(config_path), coords_path(coords_path), distribution_path(distribution_path), energy_path(energy_path)
    {
        std::ifstream ifs(config_path);
        json jf = json::parse(ifs);

        std::ofstream out1(coords_path);
        out1.close();
        std::ofstream out3(distribution_path);
        out3.close();
        std::ofstream out2(energy_path);
        out3.close();

        molecule_mass = jf["molecule_mass"];
        eps = jf["eps"];
        sigma = jf["sigma"];
        temperatute = jf["temperatute"];
        env_size = jf["env_size"];
        molecules_count = jf["molecules_count"];
        bool grid_init_mode = jf["grid_init_mode"];
        max_step_count = jf["max_step_count"];
        save_period = jf["save_period"];
        dt = jf["dt"];
        *reng = std::default_random_engine(jf["seed"]);
        
            
        ec2 = 4*eps / (sigma * sigma);
        unsigned mc3 = static_cast<unsigned>(std::pow(molecules_count, 1./3));
        this->molecules_count = mc3 * mc3 * mc3;
        double grid_size = static_cast<double>(env_size) / static_cast<double>(mc3);
        
        std::uniform_real_distribution<double> dstr(0, 1);
        std::normal_distribution<double> dstr_normal(0, std::sqrt(kb * temperatute / molecule_mass));

        for (unsigned i = 0; i < mc3; i++){
            for (unsigned j = 0; j < mc3; j++){
                for (unsigned k = 0; k < mc3; k++){
                    if (grid_init_mode) {
                        Vector3d position(grid_size * (i + 1. / 2), 
                                                    grid_size * (j + 1. / 2), 
                                                    grid_size * (k + 1. / 2));
                        
                        Vector3d velocity_n(dstr_normal(*reng), 
                                            dstr_normal(*reng), 
                                            dstr_normal(*reng));
                        
                        molecules.push_back(Molecule(position, velocity_n, molecule_mass));
                    }
                    else {
                        Vector3d position(grid_size * (i + dstr(*reng) - 1. / 2), 
                                                    grid_size * (j + dstr(*reng) - 1. / 2), 
                                                    grid_size * (k + dstr(*reng) - 1. / 2));
                        
                        Vector3d velocity_n(dstr_normal(*reng), 
                                            dstr_normal(*reng), 
                                            dstr_normal(*reng));
                        
                        molecules.push_back(Molecule(position, velocity_n, molecule_mass));
                    }
            }
        }
        }
    }
    void simulate(){
        for (current_step = 0; current_step < max_step_count; current_step++){
            if (current_step % save_period == 0){
                save_data();
            }
            step();
        }
    }

private:
    char* config_path;
    char* coords_path;
    char* distribution_path;
    char* energy_path;

    const double kb = 1.38e-23;

    double molecule_mass;
    double eps;
    double sigma;
    double temperatute;
    double env_size;
    unsigned molecules_count;

    unsigned max_step_count;
    unsigned save_period;
    unsigned current_step = 0;
    double dt;
    std::vector<Molecule> molecules;
    std::optional<std::default_random_engine> reng;
    double ec2;

    Vector3d calc_force(const Molecule &v1, const Molecule &v2) {
        Vector3d r = v2.position - v1.position;
        double p = sigma / r.norm();
        return ec2 * (6 * std::pow(p, 8) - 12 * std::pow(p, 14)) * r;
    }

    double calc_energy() {
        double energy = 0;
        for (unsigned i = 0; i < molecules_count; i++){
            for (unsigned j = i + 1; j < molecules_count; j++){
                Vector3d r = molecules[i].position - molecules[j].position;
                double p = std::pow(sigma / r.norm(), 6);
                energy += 4 * eps * (p * p - p);
            }
            energy += molecules[i].mass * std::pow(molecules[i].velocity.norm(), 2) / 2;
        }
        return energy / molecules_count;
    }
    
    void save_data(){
        std::ofstream outfile(coords_path, std::ios::app);
        outfile << molecules_count << std::endl;
        outfile << "Frame " << current_step << std::endl;
        for (unsigned i = 0; i < molecules_count; i++) {
            outfile << "O " << molecules[i].position.x << " " << molecules[i].position.y << " " << molecules[i].position.z << std::endl;
        }
        outfile.close();

        // save distribution
        std::ofstream distribution_file(distribution_path, std::ios::app);
        for (unsigned i = 0; i < molecules_count; i++){
            distribution_file << molecules[i].velocity.norm() << " ";
        }
        distribution_file << std::endl;
        distribution_file.close();

        std::ofstream energy_file(energy_path, std::ios::app);
        energy_file << calc_energy() << std::endl;
        distribution_file.close();
    }

    void update_forces(){
        for (unsigned i = 0; i < molecules_count; i++){
            molecules[i].reset_force();
        }
        for (unsigned i = 0; i < molecules_count; i++){
            for (unsigned j = 0; j < molecules_count; j++){
                for (int k1 = -1; k1 < 2; k1++){
                    for (int k2 = -1; k2 < 2; k2++){
                        for (int k3 = -1; k3 < 2; k3++){
                            if (k1 == 0 && k2 == 0 && k3 == 0 && i == j){
                                continue; 
                            }
                            Molecule virtual_m(
                                molecules[j].position + env_size * Vector3d(k1, k2, k3), 
                                molecules[j].velocity,
                                molecules[j].mass
                            );
                            molecules[i].add_force(calc_force(molecules[i], virtual_m));
                        }
                    }
                }
            }
        }
    }

    void step() {
        // Можно получше переписать, в принципе под вопросом.
        update_forces();
        for (unsigned i = 0; i < molecules_count; i++){
            Molecule molecule = molecules[i];
            molecules[i].position = molecule.position + molecule.velocity*dt + 0.5 * molecule.get_accelerate() * dt * dt;
            molecules[i].position = molecules[i].position % env_size;
        }
        update_forces();
        for (unsigned i = 0; i < molecules_count; i++){
            Molecule molecule = molecules[i];
            molecules[i].velocity = molecule.velocity + 0.5 * dt * (molecule.get_accelerate() + molecule.get_prev_accelerate());
        }
    }
};

int main(int argc, char* argv[]){
    // Проверка, что ввод config_path.json, output_path.xyz, distribution.txt, energy.txt
    if (argc != 5) {
        std::cout << "Неправильный ввод" << std::endl;
        return -1;
    }

    Environment env(argv[1], argv[2], argv[3], argv[4]);
    env.simulate();
    return 0;
}