#include <cmath>
#include <iostream>

class Sin{
public:
    Sin(double x, unsigned long N): x(x), N(N), factor(x) { }
    double calculate(){
        for (unsigned long i = 1; i < N; i++){
            result += factor;
            factor = - x * x * factor / (2*i * (2*i + 1));
        }
        return result;
    }
private:
    double x;
    unsigned long N;
    double factor;
    double result = 0;
};

int main(){
    Sin sin(0.523599, 1000000);
    std::cout << sin.calculate() << std::endl;
    return 0;
}