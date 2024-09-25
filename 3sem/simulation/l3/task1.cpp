#include <cmath>
#include <iostream>
#include <cstring>
#include <bitset>

#define N 100
#define step 2


void printBinary(double number) {
    uint64_t value;
    std::memcpy(&value, &number, sizeof(double));

    std::bitset<64> bits(value);
    for (int i = 63; i >= 0; --i) {
        std::cout << bits[i];
        if (i == 52) std::cout << " "; // Разделим мантиссу
        if (i == 63) std::cout << " "; // Разделим знак
    }
}


int main(){
    double result = 0;
    for (unsigned i = 1; i < N; i++){
        if ((i-1) % step == 0){
            std::cout << i << ' ' << result << ' ';
            printBinary(result);
            std::cout << std::endl;
        }
        result += std::pow(0.5, i);
    }
    std::cout << result << std::endl;
    return 0;
}