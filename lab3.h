#ifndef LAB3_H
#define LAB3_H

#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include <chrono>

class RandomGenerator {
public:
    virtual ~RandomGenerator() = default;
    virtual std::vector<int> generate(int size, int min, int max) = 0;
};

class LCG : public RandomGenerator {
private:
    uint64_t a = 6364136223846793005ULL; 
    uint64_t c = 1442695040888963407ULL;
    uint64_t m = (1ULL << 63);
    uint64_t seed;
    uint64_t mix(uint64_t x) {
        x ^= x >> 12;
        x ^= x << 25;
        x ^= x >> 27;
        return x * 2685821657736338717ULL;
    }

public:
    LCG(uint64_t seed) : seed(seed) {}

    std::vector<int> generate(int size, int min, int max) override;
};

class MCG : public RandomGenerator {
private:
    uint64_t a = 6364136223846793005ULL;
    uint64_t m = (1ULL << 63);
    uint64_t seed;

    uint64_t mix(uint64_t x) {
        x ^= x >> 12;
        x ^= x << 25;
        x ^= x >> 27;
        return x * 2685821657736338717ULL;
    }

public:
    MCG(uint64_t seed) : seed(seed) {}

    std::vector<int> generate(int size, int min, int max) override ;
};

class MT : public RandomGenerator {
private:
    std::mt19937_64 mt;
    uint64_t seed;

public:
    MT(uint64_t seed = std::random_device{}()) : seed(seed), mt(seed) {}

    std::vector<int> generate(int size, int min, int max) override;
};

double mean(const std::vector<int> &data);
double standard_deviation(const std::vector<int> &data);
double coefficient_of_variation(const std::vector<int> &data);
void chi_square_test(const std::vector<int> &data, int bins, 
                     std::ofstream &output);
void run_tests(RandomGenerator &generator, int &samples, int size, 
               int &min, int &max, std::string &filename, 
               std::ofstream &output);
void save_sample_as_text(const std::vector<int> &sample, 
                         const std::string &filename);
void run_test_nist(RandomGenerator &generator, int samples, int size, 
                   int &min, int max, std::string &filename);
void test_nist(std::string &filename, int &samples);
void run_test_time(RandomGenerator &generator, int &samples, int &size,
                   int &min, int &max, std::ofstream &output);

#endif // LAB3_H
