#include "lab3.h"

#include <climits>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip> 

namespace fs = std::filesystem;

int main() {
    fs::create_directory("output");
    std::ofstream output("output/output.csv");

    int samples = 20;
    int size = 1000;
    int min = 0;
    int max = 5000;

    output << std::fixed << std::setprecision(3);

    output << "Generator" << ","
           << "Size" << ","
           << "Mean" << ","
           << "StdDev" << ","
           << "CV" << ","
           << "ChiSquare" << ","
           << "k\n"
           << "Теор." << ","
           << "-,"
           << static_cast<double>((max + min) / 2) << ","
           << static_cast<double>((max - min) / sqrt(12)) << ","
           << static_cast<double>((max - min) / (sqrt(3) * (max + min))) << ","
           << "-,"
           << "-\n";

    LCG lcg(12345);
    MCG mcg(12345);
    MT mt;

    std::string filename = "LCG";
    std::cout << "Testing LCG:\n";
    run_tests(lcg, samples, size, min, max, filename, output);
    run_test_nist(lcg, 5, 32000, min, INT_MAX-1, filename);
    
    filename = "MCG";
    std::cout << "Testing MCG:\n";
    run_tests(mcg, samples, size, min, max, filename, output);
    run_test_nist(mcg, 5, 32000, min, INT_MAX-1, filename);
    
    filename = "MT";
    std::cout << "Testing MT:\n";
    run_tests(mt, samples, size, min, max, filename, output);
    run_test_nist(mt, 5, 32000, min, INT_MAX-1, filename);

    std::system("rm -f output/nist* output/data* ");

    std::ofstream output_time("output/output_time.csv");
    output_time << "Generator,Size,Time(ms)\n"; 
    for (int i = 1000; i < 1000000; i=i+1000) {
        output_time << "LCG,";
        run_test_time(lcg, samples, i, min, max, output_time); 
    }
    for (int i = 1000; i < 1000000; i=i+1000) {
        output_time << "MCG,";
        run_test_time(mcg, samples, i, min, max, output_time); 
    }
    for (int i = 1000; i < 1000000; i=i+1000) {
        output_time << "MT,";
        run_test_time(mt, samples, i, min, max, output_time); 
    }

    return 0;
}
