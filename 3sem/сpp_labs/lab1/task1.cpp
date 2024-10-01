#include <random>
#include <iostream>
#include <memory>

class AbstractDice {
public:
    virtual unsigned roll() = 0;
};


class Dice: public AbstractDice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), dstr(1, max), reng(seed) { }

    unsigned roll() override{
        return dstr(reng);
        }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};


class ThreeDicePool: public AbstractDice {
public:
    ThreeDicePool(Dice* d1, Dice* d2, Dice* d3): 
        d1(d1), d2(d2), d3(d3){}
    
    unsigned roll() override{
            return d1->roll() + d2->roll() + d3->roll();
        }
    
private:
    std::unique_ptr<Dice> d1, d2, d3;
};


double expected_value(AbstractDice &d, unsigned number_of_rolls = 1) {
    unsigned long long accum = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return
        static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

int main(){
    const unsigned seed = 42;
    const unsigned max = 100;
    const unsigned iters = 10000;

    Dice dice(max, seed);
    std::cout << "Dice: " << expected_value(dice, iters) << std::endl;
    ThreeDicePool three_dice(new Dice(max, seed), new Dice(max, seed*2), new Dice(max, seed*3));
    std::cout << "ThreeDicePool: " << expected_value(three_dice, iters) << std::endl;
    return 0;
}
