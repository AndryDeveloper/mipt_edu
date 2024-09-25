#include <cmath>
#include <iostream>

#define N 1000
#define step 1

int main(){
    double result = 0;
    for (unsigned i = 1; i < N; i++){
        if (i % step == 0){
            std::cout << i << ' ' << result << std::endl;
        }
        result += std::pow(0.5, i);
    }
    std::cout << result << std::endl;
    return 0;
}