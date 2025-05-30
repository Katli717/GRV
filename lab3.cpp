#include "lab3.h"

#include <algorithm>
#include <bitset>
#include <chrono>
#include <fstream>
#include <unistd.h>

std::vector<int> LCG::generate(int size, int min, int max) {
  std::vector<int> numbers;
  numbers.reserve(size);
  uint64_t range = uint64_t(max) - uint64_t(min) + 1;
  for (int i = 0; i < size; ++i) {
    seed = a * seed + c;
    uint64_t val = mix(seed);
    int res = min + static_cast<int>(val % range);
    numbers.push_back(res);
  }
  return numbers;
}

std::vector<int> QCG::generate(int size, int min, int max) {
  std::vector<int> numbers;
  numbers.reserve(size);
          
  for (int i = 0; i < size; ++i) {
    int random_number = next() % (max - min + 1) + min;
    numbers.push_back(random_number);
  }
  return numbers;
}

std::vector<int> EnhancedRandom::generate(int size, int min, int max) {
  std::vector<int> numbers;
  numbers.reserve(size);
    
  for (int i = 0; i < size; ++i) {
    int random_number = next() % (max - min + 1) + min; 
    numbers.push_back(random_number);
  }
  return numbers;
}

double mean(const std::vector<int> &data) {
  double sum = 0;
  for (int num : data) {
    sum += num;
  }
  return sum / data.size();
}

double standard_deviation(const std::vector<int> &data) {
  double avg = mean(data);
  double sum = 0;
  for (int num : data) {
    sum += (num - avg) * (num - avg);
  }
  return sqrt(sum / data.size());
}

double coefficient_of_variation(const std::vector<int> &data) {
  return standard_deviation(data) / mean(data);
}

void chi_square_test(const std::vector<int> &data, int bins, 
                     std::ofstream &output) {
  if (data.empty() || bins <= 0) {
    std::cerr << "Ошибка: пустые данные или неверное количество интервалов."
              << std::endl;
    return;
  }

  int n = data.size();

  int min_val = *std::min_element(data.begin(), data.end());
  int max_val = *std::max_element(data.begin(), data.end());

  double bin_width = static_cast<double>(max_val - min_val + 1) / bins;

  std::vector<int> observed(bins, 0);
  for (int val : data) {
    int bin_index = static_cast<int>((val - min_val) / bin_width);
    if (bin_index == bins) bin_index = bins - 1; 
    observed[bin_index]++;
  }

  double expected = static_cast<double>(n) / bins;

  double chi_square = 0.0;
  for (int i = 0; i < bins; ++i) {
    double diff = observed[i] - expected;
    chi_square += diff * diff / expected;
  }

  int df = bins - 1;
  output << chi_square << ","
         << df << "\n";
}

void save_sample_as_text(const std::vector<int> &sample, 
                         const std::string &filename) {
  std::ofstream out(filename);
  if (!out) {
    std::cerr << "Не удалось открыть файл " << filename << " для записи.\n";
    return;
  }

  for (int num : sample) {
    std::bitset<32> bits(num);
    std::string bit_string = bits.to_string();

    for (size_t i = 0; i < bit_string.length(); i += 64) {
      out << bit_string.substr(i, 64) << "\n"; 
    }
  }

  out.close();
}

void run_tests(RandomGenerator &generator, int &samples, int size, 
               int &min, int &max, std::string &filename, 
               std::ofstream &output) {
  for (int i = 0; i < samples; ++i) {
    std::ofstream output_file("output/result" + filename + \
                              std::to_string(i+1) + ".txt");
    if (!output_file.is_open()) {
      std::cerr << "Не удалось открыть файл " << filename << " для записи." 
                << std::endl;
      break;
    }
    auto numbers = generator.generate(size * (i + 1), min, max);
    double avg = mean(numbers);
    double stddev = standard_deviation(numbers);
    double cv = coefficient_of_variation(numbers);

    for (const auto &number : numbers) {
      output_file << number << std::endl;
    }
    
    output << filename << "," 
           << size * (i + 1) << ","
           << static_cast<double>(avg) << ","
           << static_cast<double>(stddev) << ","
           << static_cast<double>(cv) << ",";
    chi_square_test(numbers, 1 + std::log2(size * (i + 1)), output);
  }
}

void run_test_nist(RandomGenerator &generator, int samples, int size, 
                   int &min, int max, std::string &filename) {
  for (int i = 0; i < samples; ++i) {
    std::ofstream data("output/data" + filename + \
                      std::to_string(i+1) + ".txt");
    data << "0\n"
        << "../../programming_techniques/lab3/output/nist"
        << filename + std::to_string(i+1) + ".txt\n" 
        << "1\n0\n10\n0\n";
    auto numbers = generator.generate(size, min, max);
    save_sample_as_text(numbers,"output/nist" + filename + \
                                std::to_string(i+1) + ".txt");
  }
  test_nist(filename, samples);
}

void test_nist(std::string &filename, int &samples) {
  for (int i = 0; i < samples; ++i) {
    std::string cmd = 
        std::string("cd ~/study/NIST-Statistical-Test-Suite/sts && ") + \
        "./assess 100000 <~/study/programming_techniques/lab3/output/data" + \
        filename + std::to_string(i+1) + ".txt";
    int ret = std::system(cmd.c_str());
    cmd = std::string("cp ~/study/NIST-Statistical-Test-Suite/sts/experiments/")+ \
          "AlgorithmTesting/finalAnalysisReport.txt " + \
          "~/study/programming_techniques/lab3/output/resnist" + \
          filename + std::to_string(i+1) + ".txt";
    std::system(cmd.c_str());
    cmd = std::string("./filter_lines.sh output/resnist") + \
          filename + std::to_string(i+1) + ".txt" + \
          " patterns.txt";
    std::system(cmd.c_str());
  }
}

void run_test_time(RandomGenerator &generator, int &samples, int &size, 
                   int &min, int &max, std::ofstream &output) {
  auto start = std::chrono::high_resolution_clock::now();
  auto numbers = generator.generate(size, min, max);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end - start;
  output << size << ","
         << duration.count() << "\n";
}