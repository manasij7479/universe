#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "cyCodeBase/cyTriMesh.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyGL.h"

#include "utils/Galaxy.h"
#include "utils/Nebula.h"

#include <iostream>

void setup() {
  glewInit();
  glClearColor(0.0f, 0.0f, 0.0f, 1);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void draw(GLuint VAO, size_t Count, size_t Instances) {
  glBindVertexArray(VAO);
//   glDrawArrays(GL_TRIANGLES, 0, Count);
  glDrawArraysInstanced(GL_TRIANGLES, 0, Count, Instances);
}

int main(int argc, char **argv) {
  sf::ContextSettings settings;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  settings.attributeFlags = sf::ContextSettings::Core;

  sf::Window win(sf::VideoMode(argc > 2 ? std::stoi(argv[2]) : 1024,
                               argc > 3 ? std::stoi(argv[3]) : 768, 32),
                               "Universe", sf::Style::Default, settings);

  setup();
  glViewport(0, 0, 1024, 768);

  auto Proj = cy::Matrix4f::Perspective(90 * 3.142 / 180, 1024.0f/768.0f, 10.1f, 15000.0f);


  Galaxy BG;
  BG.Read("data/g/backdrop-10k-100k.txt");
  BG.setup();

  Galaxy G;
  G.Read("data/g/elliptical-galaxy-200-200-300-10000.txt");
  G.setup();

  Nebula N;
  N.Read("/tmp/emi.txt");
  N.setup();

  Galaxy G2;
  G2.Read("data/g/elliptical-galaxy-400-500-300-10000.txt");
  G2.setup();

  bool Left = false, Right = false;
  bool W = false, S = false;
  int x, y;
  float dist = 0, rx = 0, ry = 0;
  float f = 0.01;


  while(win.isOpen()) {
    auto Camera = cy::Matrix4<float>(1);
    cy::Vec3f CameraPos = {0, 0, 0};
    Camera *= cy::Matrix4f::Translation(CameraPos) ;
    Camera *= cy::Matrix4f::RotationX(rx);
    Camera *= cy::Matrix4f::RotationY(ry);

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);



    BG.draw(Proj * Camera.GetInverse() * cy::Matrix4f::Translation({0, 0, -100 - dist}) * Camera);
    G.draw(Proj * Camera.GetInverse() * cy::Matrix4f::Translation({0, 0, -100 - dist}) * Camera);

    N.draw(Proj * Camera.GetInverse() * cy::Matrix4f::Translation({-1000, -1000, -100 - dist}) * Camera);

//     G2.draw(Proj * Camera.GetInverse() * cy::Matrix4f::Translation({1000, 9000, -100 - dist}) * Camera);
//     G2.draw(Proj * Camera.GetInverse() * cy::Matrix4f::Translation({7000, 6000, -100 - dist}) * Camera);
//     G.draw(Proj * Camera.GetInverse() * cy::Matrix4f::Translation({2400, 2400, -100 - dist}) * Camera);

    f += 0.01;

    sf::Event eve;
    while(win.pollEvent(eve)) {
      if(eve.type == sf::Event::Closed) {
        win.close();
      } else if (eve.type == sf::Event::KeyPressed) {
        if (eve.key.code == sf::Keyboard::Escape) {
          win.close();
        }
        if (eve.key.code == sf::Keyboard::F6) {
          std::cout << "Recompiling Shaders.\n";
          cy::GLSLProgram NewProg;
          if (!NewProg.BuildFiles("vert.glsl", "frag.glsl", nullptr, nullptr, nullptr, 0, nullptr, &std::cerr)) {
            std::cerr << "Failed to recompile shaders.\n";
            return 1;
          }
          glUseProgram(NewProg.GetID());
          G.ProgS.Delete();
          G.ProgS = NewProg;
        }
        if (eve.key.code == sf::Keyboard::W) {
          W = true;
        }
        if (eve.key.code == sf::Keyboard::S) {
          S = true;
        }

      } else if (eve.type == sf::Event::KeyReleased) {
        if (eve.key.code == sf::Keyboard::W) {
          W = false;
        }
        if (eve.key.code == sf::Keyboard::S) {
          S = false;
        }
      } else if (eve.type == sf::Event::MouseButtonPressed) {
        x = eve.mouseButton.x;
        y = eve.mouseButton.y;
        if (eve.mouseButton.button == sf::Mouse::Right) {
          Right = true;
        }
        if (eve.mouseButton.button == sf::Mouse::Left) {
          Left = true;
        }
      } else if (eve.type == sf::Event::MouseButtonReleased) {
        if (eve.mouseButton.button == sf::Mouse::Right) {
          Right = false;
        }
        if (eve.mouseButton.button == sf::Mouse::Left) {
          Left = false;
        }
      } else if (eve.type == sf::Event::MouseMoved) {
        int dx = eve.mouseMove.x - x;
        int dy = eve.mouseMove.y - y;

        if (Left) {
          rx = dy * 3.14 / 180;
          ry = dx * 3.14 / 180;
        }
//         if (Right) {
//           dist = (abs(dx) + abs(dy));
//         }
      }
    }
    if (W) {
      dist++;
    }
    if (S) {
      dist--;
    }
    std::cout << dist << '\n';
    win.display();
  }
}

