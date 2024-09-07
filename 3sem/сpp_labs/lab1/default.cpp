#include <random>
#include <iostream>

class Dice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), dstr(1, max), reng(seed) { }

    unsigned roll() {
        return dstr(reng);
        }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool {
public:
    ThreeDicePool(unsigned max,
        unsigned seed_1, unsigned seed_2, unsigned seed_3):
        max(max), dstr_1(1, max), dstr_2(1, max), dstr_3(1, max),
        reng_1(seed_1), reng_2(seed_2), reng_3(seed_3) { }

        unsigned roll() {
            return dstr_1(reng_1) + dstr_2(reng_2) + dstr_3(reng_3);
        }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr_1, dstr_2, dstr_3;
    std::default_random_engine reng_1, reng_2, reng_3;
};


double expected_value(Dice &d, unsigned number_of_rolls = 1) {
    unsigned long long accum = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return
        static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

double expected_value(ThreeDicePool &tdp, unsigned number_of_rolls = 1) {
    unsigned long long accum = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += tdp.roll();
    return
        static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

int main(){
    Dice dice(10, 0);
    std::cout << expected_value(dice, 1000) << std::endl;
    ThreeDicePool three_dice(10, 0, 0, 0);
    std::cout << expected_value(three_dice, 1000) << std::endl;
    return 0;
}
