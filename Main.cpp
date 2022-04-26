#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "cyCodeBase/cyTriMesh.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyGL.h"

#include "utils/Scene.h"

#include <iostream>

void setup() {
  glewInit();
  glClearColor(0.0f, 0.0f, 0.0f, 1);

  glEnable(GL_CULL_FACE);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char **argv) {
  sf::ContextSettings settings;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  settings.attributeFlags = sf::ContextSettings::Core;

  auto WIDTH = 1920;
  auto HEIGHT = 1080;

  sf::Window win(sf::VideoMode(argc > 2 ? std::stoi(argv[2]) : WIDTH,
                               argc > 3 ? std::stoi(argv[3]) : HEIGHT, 32),
                               "Universe", sf::Style::Default, settings);

  setup();
  glViewport(0, 0, WIDTH, HEIGHT);

  auto Proj = cy::Matrix4f::Perspective(90 * 3.142 / 180, WIDTH * 1.0f/HEIGHT, 10.1f, 15000.0f);

  Scene Sc(argv[1]);

  bool W = false, S = false, A = false, D = false, Z = false, C = false, Left = false, Right = false, Up = false, Down = false;
  float zdist = 2500, xdist = 0, ydist = 0, rx = 0, ry = 0;

  auto start = clock();
  auto rate = CLOCKS_PER_SEC;
  int fps = 0;
  int frames = 0;

  while(win.isOpen()) {

    auto t = clock() - start;
    frames++;
    if (t - start > rate) {
      fps = frames;
      frames = 0;
      start = t;
    }

    auto title = "Universe - " + std::to_string(xdist) + " " + std::to_string(ydist) + " " + std::to_string(zdist) + " -  FPS " + std::to_string(fps);

    win.setTitle(title);
    auto Camera = cy::Matrix4<float>(1);
    auto CameraRot = cy::Matrix4<float>(1);
    cy::Vec3f CameraPos = {xdist, ydist, zdist};
    CameraRot *= cy::Matrix4f::RotationX(ry);
    CameraRot *= cy::Matrix4f::RotationY(rx);
    Camera *= cy::Matrix4f::Translation(CameraPos) ;
    Camera*= CameraRot;

    auto F = [&] (cyMatrix4f Translation) {
      return Proj * Camera.GetInverse() * Translation * CameraRot;
    };

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    Sc(F);

    sf::Event eve;
    while(win.pollEvent(eve)) {
      if(eve.type == sf::Event::Closed) {
        win.close();
      } else if (eve.type == sf::Event::KeyPressed) {
        if (eve.key.code == sf::Keyboard::Escape) {
          win.close();
        }
        if (eve.key.code == sf::Keyboard::W) {
          W = true;
        }
        if (eve.key.code == sf::Keyboard::S) {
          S = true;
        }
        if (eve.key.code == sf::Keyboard::A) {
          A = true;
        }
        if (eve.key.code == sf::Keyboard::D) {
          D = true;
        }
        if (eve.key.code == sf::Keyboard::Z) {
          Z = true;
        }
        if (eve.key.code == sf::Keyboard::C) {
          C = true;
        }
        if (eve.key.code == sf::Keyboard::Left) {
          Left = true;
        }
        if (eve.key.code == sf::Keyboard::Right) {
          Right = true;
        }
        if (eve.key.code == sf::Keyboard::Up) {
          Up = true;
        }
        if (eve.key.code == sf::Keyboard::Down) {
          Down = true;
        }

      } else if (eve.type == sf::Event::KeyReleased) {
        if (eve.key.code == sf::Keyboard::W) {
          W = false;
        }
        if (eve.key.code == sf::Keyboard::S) {
          S = false;
        }
        if (eve.key.code == sf::Keyboard::A) {
          A = false;
        }
        if (eve.key.code == sf::Keyboard::D) {
          D = false;
        }
        if (eve.key.code == sf::Keyboard::Z) {
          Z = false;
        }
        if (eve.key.code == sf::Keyboard::C) {
          C = false;
        }
        if (eve.key.code == sf::Keyboard::Left) {
          Left = false;
        }
        if (eve.key.code == sf::Keyboard::Right) {
          Right = false;
        }
        if (eve.key.code == sf::Keyboard::Up) {
          Up = false;
        }
        if (eve.key.code == sf::Keyboard::Down) {
          Down = false;
        }

      }
    }
    if (W) {
      zdist--;
    }
    if (S) {
      zdist++;
    }
    if (A) {
      xdist--;
    }
    if (D) {
      xdist++;
    }
    if (Z) {
      ydist--;
    }
    if (C) {
      ydist++;
    }
    if (Left) {
      rx += 0.01;
    }
    if (Right) {
      rx -= 0.01;
    }
    if (Up) {
      ry -= 0.01;
    }
    if (Down) {
      ry += 0.01;
    }
    win.display();
  }
}

