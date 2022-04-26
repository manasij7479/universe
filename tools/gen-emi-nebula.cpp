#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>
int main(int argc, char **argv) {
  int X = std::stoi(argv[1]),
      Y = std::stoi(argv[2]),
      Z = std::stoi(argv[3]),
      Stars = std::stoi(argv[4]);
      
  std::random_device rd;
  std::mt19937 gen(rd());
  
  std::srand(std::time(0));
  
  
  for (int i = 0; i < Stars * 0.8; ++i) {
    std::normal_distribution<float> x(0, X);
    std::cout << x(gen) << ' ';
    std::normal_distribution<float> y(0, Y);
    std::cout << y(gen) << ' ';
    std::normal_distribution<float> z(0, Z);
    std::cout << z(gen) << ' ';

    std::normal_distribution<float> s(60, 20);
    std::cout << s(gen) << ' ';

    int col = 1;
    if (rand() % 100 < 92) {
      col = 4;
    }
    std::cout << col << '\n';
  }

  for (int i = 0; i < Stars /5; ++i) {
    std::normal_distribution<float> x(0, X);
    std::cout << std::rand() % X - X/2 << ' ';
    std::normal_distribution<float> y(0, Y);
    std::cout << std::rand() % Y - Y/2 << ' ';
    std::normal_distribution<float> z(0, Z);
    std::cout << std::rand() % Z - Z/2 << ' ';

    std::normal_distribution<float> s(60, 20);
    std::cout << s(gen) << ' ';

    int col = 1;
    if (rand() % 100 < 92) {
      col = 4;
    }
    std::cout << col << '\n';
  }
  
}
