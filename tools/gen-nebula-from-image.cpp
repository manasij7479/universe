#include <fstream>
#include <SFML/Graphics.hpp>
#include <random>
#include <cstdlib>

int main(int argc, char **argv) {
  sf::Image I;
  I.loadFromFile(argv[1]);
  int count = std::stoi(argv[2]);
  std::ofstream N(argv[3]);
  
  int X = I.getSize().x;
  int Y = I.getSize().y;
  
  std::random_device rd;
  std::mt19937 gen(rd());
  
  std::map<int, int> count_;
  
  std::normal_distribution<float> x(X/2, X/4), y(Y/2, Y/4);
  std::srand(0);
  int n = 0;
  while (n < count) {
    int x_ = 0, y_ = 0;
    if (n > 500) {
      x_ = int(x(gen));
      y_ = int(y(gen));
    } else {
      x_ = std::rand() % X;
      y_ = std::rand() % Y;
    }

    if (x_ < 0 || x_ >= X || y_ < 0 || y_ >= Y) continue;
    
    auto C = I.getPixel(x_, y_);
    auto luminance = (C.r * 0.3) + (C.g * 0.59) + (C.b * 0.11);
    
    if (C.r > 100) {
      std::normal_distribution<float> off(0, 10);
      N << (x_ + off(gen) - X/2) * 4  << ' ';
      
      N << (y_ + off(gen) - Y/2) * 4 << ' ';
      
      std::normal_distribution<float> z(0, 10);
      N << z(gen) << ' ';
      
      std::normal_distribution<float> s(20, 10);
      N << rand() % 20 + 30 << ' ';
      
      N << 4 << "\n";
      n++;
    }
    if (C.b > 200) {
      std::normal_distribution<float> off(0, 10);
      N << (x_ + off(gen) - X/2) * 4  << ' ';
      
      N << (y_ + off(gen) - Y/2) * 4 << ' ';
      
      std::normal_distribution<float> z(0, 300);
      N << z(gen) << ' ';
      
      std::normal_distribution<float> s(60, 20);
      N << s(gen) << ' ';
      
      N << 0 << "\n";
      n++;
    }
//     if (luminance > 200) {
//       std::normal_distribution<float> off(0, 10);
//       N << (x_ + off(gen) - X/2) * 4  << ' ';
//       
//       N << (y_ + off(gen) - Y/2) * 4 << ' ';
//       
//       std::normal_distribution<float> z(0, 10);
//       N << z(gen) << ' ';
//       
//       std::normal_distribution<float> s(20, 10);
//       N << rand() % 20 + 30 << ' ';
//       
//       N << 1 << "\n";
//       n++;
//     }
    
  }

//   for (int i = 0; i < X; ++i) {
//     for (int j = 0; j < Y; ++j) {
//       auto C = I.getPixel(i, j);
//       auto luminance = (C.r * 0.3) + (C.g * 0.59) + (C.b * 0.11);
// //       G << uint(C.r) << uint(C.g) << uint(C.b) << "\n";
// //       G << luminance << "\n";
//       if (luminance > 200) {
//         if (count_[i * 7 ^ j * 3] > 5) continue;
//         count_[i * 7 ^ j * 3] ++;
//         
//         std::normal_distribution<float> x(0, 10);
//         G << 1.0* i + x(gen) - X/2  << ' ';
//         
//         G << 1.0* j + x(gen) - Y/2 << ' ';
//         
//         std::normal_distribution<float> z(0, 10);
//         G << z(gen) << ' ';
//         
//         std::normal_distribution<float> s(0.2, 0.3);
//         G << 0.1 << ' ';
//         
//         G << 1 << "\n";
//         
//       } 
//     }
//   }
  
}
