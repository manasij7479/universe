#include <fstream>
#include <SFML/Graphics.hpp>
#include <random>

int main(int argc, char **argv) {
  sf::Image I;
  I.loadFromFile(argv[1]);
  std::ofstream G(argv[2]);
  std::ofstream N(argv[3]);
  
  int X = I.getSize().x;
  int Y = I.getSize().y;
  
  std::random_device rd;
  std::mt19937 gen(rd());
  
  std::map<int, int> count_;

  for (int i = 0; i < X; ++i) {
    for (int j = 0; j < Y; ++j) {
      auto C = I.getPixel(i, j);
      auto luminance = (C.r * 0.3) + (C.g * 0.59) + (C.b * 0.11);
//       G << uint(C.r) << uint(C.g) << uint(C.b) << "\n";
//       G << luminance << "\n";
      if (luminance > 200) {
        if (count_[i * 7 ^ j * 3] > 5) continue;
        count_[i * 7 ^ j * 3] ++;
        
        std::normal_distribution<float> x(0, 10);
        G << 1.0* i + x(gen) - X/2  << ' ';
        
        G << 1.0* j + x(gen) - Y/2 << ' ';
        
        std::normal_distribution<float> z(0, 10);
        G << z(gen) << ' ';
        
        std::normal_distribution<float> s(0.2, 0.3);
        G << 0.1 << ' ';
        
        G << 1 << "\n";
        
      } 
    }
  }
  
}
