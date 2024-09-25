#include <cmath>
#include <iostream>
#include <cstring>
#include <bitset>

#define N 10000000
#define step 100000

void printBinary(float number) {
    uint64_t value;
    std::memcpy(&value, &number, sizeof(float));

    std::bitset<32> bits(value);
    for (int i = 31; i >= 0; --i) {
        std::cout << bits[i];
        if (i == 26) std::cout << " "; // Разделим мантиссу
        if (i == 31) std::cout << " "; // Разделим знак
    }
}

int main(){
    float result = 0;
    for (unsigned i = 1; i < N; i++){
        if ((i - 1) % step == 0){
            std::cout << i << ' ' << result << ' ';
            printBinary(result);
            std::cout << std::endl;
        }
        result += 1. / i;
    }
    std::cout << result << std::endl;
    return 0;
}