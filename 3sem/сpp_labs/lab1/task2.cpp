#include <random>
#include <iostream>
#include <memory>

#define SEED 42
#define MAX1 100
#define MAX2 6
#define ITERS 10000

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


class PenaltyDice: public Dice{
public:
    PenaltyDice(unsigned max, unsigned seed): Dice(max, seed) {}
    unsigned roll() override{
        return std::min(Dice::roll(), Dice::roll());
        }
};


class BonusDice: public Dice{
public:
    BonusDice(unsigned max, unsigned seed): Dice(max, seed) {}
    unsigned roll() override{
        return std::max(Dice::roll(), Dice::roll());
        }
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


double value_probability(unsigned value, AbstractDice &d, unsigned number_of_rolls = 1) {
    unsigned count = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        if (d.roll() == value) count++;
    return
        static_cast<double>(count) / static_cast<double>(number_of_rolls);
}


void hist(double* probs, unsigned n_bins) {
    const int height = 10;
    double max_prob = 0;
    for (unsigned i = 0; i < n_bins; i++){
        max_prob = std::max(max_prob, probs[i]);
    }

    for (int level = height; level > 0; --level) {
        for (unsigned i = 0; i < n_bins; ++i) {
            if (probs[i] / max_prob * height >= level) {
                std::cout << "*";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }

    for (unsigned i = 0; i < n_bins; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;
}


int main(){
    double probs1[MAX1];
    double probs2[MAX2*3];
    
    std::cout << "BonusDice" << std::endl;
    BonusDice bd(MAX1, SEED);
    for (unsigned value=0; value < MAX1; value++) {
        probs1[value] = value_probability(value+1, bd, ITERS);
    }
    hist(probs1, MAX1);

    std::cout << "PenaltyDice" << std::endl;
    PenaltyDice pd(MAX1, SEED);
    for (unsigned value=0; value < MAX1; value++) {
        probs1[value] = value_probability(value+1, pd, ITERS);
    }
    hist(probs1, MAX1);

    std::cout << "Dice" << std::endl;
    Dice d(MAX1, SEED);
    for (unsigned value=0; value < MAX1; value++) {
        probs1[value] = value_probability(value+1, d, ITERS);
    }
    hist(probs1, MAX1);

    std::cout << "3BonusDice" << std::endl;
    ThreeDicePool bd3(new BonusDice(MAX2, SEED), new BonusDice(MAX2, SEED+1), new BonusDice(MAX2, SEED+2));
    for (unsigned value=0; value < MAX2*3; value++) {
        probs2[value] = value_probability(value+1, bd3, ITERS);
    }
    hist(probs2, MAX2*3);

    std::cout << "3PenaltyDice" << std::endl;
    ThreeDicePool pd3(new PenaltyDice(MAX2, SEED), new PenaltyDice(MAX2, SEED+1), new PenaltyDice(MAX2, SEED+2));
    for (unsigned value=0; value < MAX2*3; value++) {
        probs2[value] = value_probability(value+1, pd3, ITERS);
    }
    hist(probs2, MAX2*3);

    std::cout << "3Dice" << std::endl;
    ThreeDicePool d3(new Dice(MAX2, SEED), new Dice(MAX2, SEED+1), new Dice(MAX2, SEED+2));
    for (unsigned value=0; value < MAX2*3; value++) {
        probs2[value] = value_probability(value+1, d3, ITERS);
    }
    hist(probs2, MAX2*3);
    return 0;
}
