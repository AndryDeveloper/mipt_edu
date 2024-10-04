#include <random>
#include <iostream>
#include <memory>

#define SEED 42
#define MAX1 50
#define ITERS 10000

class AbstractDice {
public:
    virtual unsigned roll() = 0;
    virtual ~AbstractDice() = default;
};

class Dice: virtual public AbstractDice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), dstr(1, max), reng(seed) { }
    Dice(const Dice& other):
        max(other.max), dstr(other.dstr), reng(other.reng) { }

    unsigned roll() override{
        return dstr(reng);
        }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};


class PenaltyDice: virtual public Dice{
public:
    PenaltyDice(unsigned max, unsigned seed): Dice(max, seed) {}
    PenaltyDice(Dice* d): Dice(*d) {}
    virtual unsigned roll() override{
        return std::min(Dice::roll(), Dice::roll());
        }
};


class BonusDice: virtual public Dice{
public:
    BonusDice(unsigned max, unsigned seed): Dice(max, seed) {}
    BonusDice(Dice* d): Dice(*d) {}
    virtual unsigned roll() override{
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


class DoubleDice: public BonusDice, public PenaltyDice{
public:
    DoubleDice(Dice* d1): Dice(*d1), BonusDice(d1), PenaltyDice(d1) { }
    
    unsigned roll() override{
            return BonusDice::roll() + PenaltyDice::roll();
        }
}; 


class DoubleDice_: public AbstractDice {
public:
    DoubleDice_(Dice* d1): 
        d1(d1), d2(d1){}
    
    unsigned roll() override{
            return d1->roll() + d2->roll();
        }
    
private:
    std::unique_ptr<Dice> d1, d2;
};


double expected_value(Dice &d, unsigned number_of_rolls = 1) {
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
    double probs[MAX1*2];

    Dice dice(MAX1, SEED);
    DoubleDice dd(&dice);

    std::cout << expected_value(dd, ITERS) << std::endl;
    
    std::cout << "DoubleDice" << std::endl;
    for (unsigned value=0; value < MAX1*2; value++) {
        probs[value] = value_probability(value+1, dd, ITERS);
    }
    hist(probs, MAX1*2);
    return 0;
}
