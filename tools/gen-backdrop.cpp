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
  
  
  for (int i = 0; i < Stars; ++i) {
    std::uniform_real_distribution<float> x(-X, X);
    std::cout << x(gen) << ' ';
    std::uniform_real_distribution<float> y(-Y, Y);
    std::cout << y(gen) << ' ';
    std::uniform_real_distribution<float> z(-Z, Z);
    std::cout << z(gen) << ' ';
    
    std::normal_distribution<float> s(1, 0.3);
    std::cout << s(gen) << ' ';
    
    int col = 1;
    if (rand() % 100 < 10) {
      col = 0;
    } else if (rand() % 1000 < 2) {
      col = 2;
    } else if (rand() % 1000 < 1) {
      col = 3;
    } else if (rand() % 1000 < 5) {
      col = 4;
    }
    std::cout << col << '\n';
  }
  
}
