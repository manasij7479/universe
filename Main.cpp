#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "cyCodeBase/cyTriMesh.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyGL.h"

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
                               "Hello World", sf::Style::Default, settings);

  setup();
  glViewport(0, 0, 1024, 768);

  auto Proj = cy::Matrix4f::Perspective(110 * 3.142 / 180, 1024.0f/768.0f, 0.01f, 1000.0f);

  sf::Image Billboard;
  Billboard.loadFromFile("sprites/star3.png");

  cy::GLTexture2D BB;
  BB.Initialize();
  BB.SetImageRGBA(Billboard.getPixelsPtr(), Billboard.getSize().x, Billboard.getSize().y);
  BB.BuildMipmaps();
  BB.SetFilteringMode(GL_LINEAR, GL_LINEAR);
  BB.SetAnisotropy(2.0f);

  float PS = 30;
  std::vector<float> Triangles = {0, 0, 0,  PS, PS, 0,  0, PS, 0  , 0, 0, 0,   PS, 0, 0,   PS, PS, 0};
  std::vector<float> PlaneTex = {0, 0,     1, 1 ,      0 , 1,      0, 0 ,     1 , 0,      1 , 1};
  std::vector<float> VNormals =  {0, 0, 1,  0, 0, 1,   0, 0, 1   ,  0, 0, 1,  0, 0, 1,  0, 0, 1 };

  unsigned int VAOP;
  glGenVertexArrays(1, &VAOP);

  glBindVertexArray(VAOP);

  unsigned int VBOP[3];
  glGenBuffers(3, VBOP);

  glBindBuffer(GL_ARRAY_BUFFER, VBOP[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Triangles.size(), Triangles.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, VBOP[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VNormals.size(), VNormals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VBOP[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * PlaneTex.size(), PlaneTex.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(2);

  std::vector<float> Off = {0, 0, 0, 50, 50, 15, 37, 56, 300};
  unsigned int InVBO;
  glGenBuffers(1, &InVBO);

  glBindBuffer(GL_ARRAY_BUFFER, InVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Off.size(), Off.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(3, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  cy::GLSLProgram ProgS;
  if (!ProgS.BuildFiles("shaders/inst-id.vert", "shaders/simpletex.glsl", nullptr, nullptr, nullptr, 0, nullptr, &std::cerr)) {
    return 1;
  }

  BB.Bind(1);
  ProgS["tex"] = 1;


  bool Left = false, Right = false;
  int x, y;
  float dist = 0, rx = 0, ry = 0;
  float f = 0.01;
  while(win.isOpen()) {
    auto Camera = cy::Matrix4<float>(1);
    cy::Vec3f CameraPos = {0, 0, 0};
    Camera *= cy::Matrix4f::Translation(CameraPos) ;
    Camera *= cy::Matrix4f::RotationX(0);
    Camera *= cy::Matrix4f::RotationY(ry);

//     Prog.SetUniform("mvp", Proj * Camera.GetInverse());

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
//     glDepthMask(GL_FALSE);

//     glUseProgram(Prog.GetID());
//     draw(VAO, CubeSize);
//     glDepthMask(GL_TRUE);

    glUseProgram(ProgS.GetID());
    cy::Matrix4<float> Model(1);
    Model *= cy::Matrix4f::Translation({30 , -10, -30 + dist});
    Model *= Camera; // billboarding
//     Model *= cy::Matrix4f::RotationX(-1.52);
    Model *= cy::Matrix4f::RotationZ(1.52);
//     Model *= cy::Matrix4f::View(CameraPos, CameraPos, {0.0f, 1.0f, 0.0f});
    auto MV = Camera.GetInverse() * Model;
    ProgS.SetUniform("mvp", Proj * MV);
    ProgS.SetUniform("model", Model);
    ProgS.SetUniform("nt", Model.GetSubMatrix3().GetInverse().GetTranspose());
    draw(VAOP, 6, Off.size() / 3);
//     glEnable(GL_CULL_FACE);

//     Model = cy::Matrix4<float>(1);
//     Model *= cy::Matrix4f::Translation({-10, -20, -40});
//     Model *= cy::Matrix4f::RotationX(-1.52);
//     Model *= cy::Matrix4f::RotationZ(0);
//     MV = Camera.GetInverse() * Model;
//     ProgS.SetUniform("mvp", Proj * MV);
//     ProgS.SetUniform("model", Model);
//     ProgS.SetUniform("nt", Model.GetSubMatrix3().GetInverse().GetTranspose());
    f += 0.01;
//     draw(VAOS, SphereSize);
    glDisable(GL_CULL_FACE);

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
          ProgS.Delete();
          ProgS = NewProg;
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
        if (Right) {
          dist = (dx + dy)/2;
        }
      }
    }
    win.display();
  }
}

