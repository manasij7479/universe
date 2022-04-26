#ifndef GALAXY_IMPORTER_H
#define GALAXY_IMPORTER_H

#include <vector>
#include <fstream>

#include "Object.h"

struct Galaxy : public Object {
  std::vector<float> XOffsets, YOffsets, ZOffsets;
  std::vector<float> Size;
  std::vector<uint> Temp; // 0 blue, 1 white, 2 yellow, 3 orange, 4 red
  std::vector<float> Lum; // TODO

  void read(std::string File) override {
    std::ifstream in(File);

    float x, y, z;
    float s;
    uint t;
    while(in >> x >> y >> z >> s >> t) {
      XOffsets.push_back(x);
      YOffsets.push_back(y);
      ZOffsets.push_back(z);
      Size.push_back(s);
      Temp.push_back(t);
    }
  }

  void setup() override {
  Billboard.loadFromFile("sprites/star3.png");

  BB.Initialize();
  BB.SetImageRGBA(Billboard.getPixelsPtr(), Billboard.getSize().x, Billboard.getSize().y);
  BB.BuildMipmaps();
  BB.SetFilteringMode(GL_LINEAR, GL_LINEAR);
  BB.SetAnisotropy(2.0f);


  std::vector<float> Triangles = {-PS, -PS, 0,  PS, PS, 0,  -PS, PS, 0  , -PS, -PS, 0,   PS, -PS, 0,   PS, PS, 0};
  std::vector<float> PlaneTex = {0, 0,     1, 1 ,      0 , 1,      0, 0 ,     1 , 0,      1 , 1};
  std::vector<float> VNormals =  {0, 0, 1,  0, 0, 1,   0, 0, 1   ,  0, 0, 1,  0, 0, 1,  0, 0, 1 };

//   unsigned int VAOP;
  glGenVertexArrays(1, &VAOP);

  glBindVertexArray(VAOP);

//   unsigned int VBOP[3];
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

//   unsigned int InVBO[3];
  glGenBuffers(5, InVBO);

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * XOffsets.size(), XOffsets.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(3, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * YOffsets.size(), YOffsets.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(4, 1); // This sets the vertex attribute to instanced attribute.

    glBindBuffer(GL_ARRAY_BUFFER, InVBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ZOffsets.size(), ZOffsets.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(5, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Size.size(), Size.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(6, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(int) * Temp.size(), Temp.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(7);
  glVertexAttribIPointer(7, 1, GL_UNSIGNED_INT, sizeof(GLuint), (GLvoid*)0);
  glVertexAttribDivisor(7, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, 0);

//   std::ofstream out("/tmp/out.txt");
//
//   for (uint i = 0; i < Size.size(); ++i) {
//     out << Offsets[3 * i] << ' ' << Offsets[3 * i + 1] << ' ' << Offsets[3 * i + 2] << ' ' << Size[i] << ' ' << Temp[i] << '\n';
//   }

//   cy::GLSLProgram ProgS;
  if (!ProgS.BuildFiles("shaders/inst-id.vert", "shaders/star.frag", nullptr, nullptr, nullptr, 0, nullptr, &std::cerr)) {
    std::cerr << "Shader failed to compile.\n";
    return;
  }

  }

  void draw(TransformFunc F, cyVec3f Loc) override {
    glUseProgram(ProgS.GetID());
    BB.Bind(1);
    ProgS["tex"] = 1;
    ProgS["mvp"] = F(cy::Matrix4f::Translation(Loc));
    glBindVertexArray(VAOP);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, Size.size());
  }

  unsigned int VAOP;
  unsigned int VBOP[3];
  unsigned int InVBO[5];
  cy::GLSLProgram ProgS;
  sf::Image Billboard;
  cy::GLTexture2D BB;

  float PS = 8;

};

struct BigStar : public Object {
    std::vector<float> Offsets;
    std::vector<float> Size;
    std::vector<uint> Temp; // 0 blue, 1 white, 2 yellow, 3 orange, 4 red

  void read(std::string File) override {
    Offsets.push_back(0);
    Offsets.push_back(0);
    Offsets.push_back(0);
//     float size;
//     int temp;
//     std::ifstream in(File);
//     in >> size >> temp;
    Size.push_back(130);
    Temp.push_back(1);
//
//     float x, y, z;
//     float s;
//     uint t;
//     while(in >> x >> y >> z >> s >> t) {
//       Offsets.push_back(x);
//       Offsets.push_back(y);
//       Offsets.push_back(z);
//       Size.push_back(s);
//       Temp.push_back(t);
//     }
  }

  void setup() override {
//   auto &Billboard = Billboard;
  Billboard.loadFromFile("sprites/star3.png");

//   auto &BB = BB;
  BB.Initialize();
  BB.SetImageRGBA(Billboard.getPixelsPtr(), Billboard.getSize().x, Billboard.getSize().y);
  BB.BuildMipmaps();
  BB.SetFilteringMode(GL_LINEAR, GL_LINEAR);
  BB.SetAnisotropy(2.0f);


  std::vector<float> Triangles = {-PS, -PS, 0,  PS, PS, 0,  -PS, PS, 0  , -PS, -PS, 0,   PS, -PS, 0,   PS, PS, 0};
  std::vector<float> PlaneTex = {0, 0,     1, 1 ,      0 , 1,      0, 0 ,     1 , 0,      1 , 1};
  std::vector<float> VNormals =  {0, 0, 1,  0, 0, 1,   0, 0, 1   ,  0, 0, 1,  0, 0, 1,  0, 0, 1 };

//   unsigned int VAOP;
  glGenVertexArrays(1, &VAOP);

  glBindVertexArray(VAOP);

//   unsigned int VBOP[3];
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

//   unsigned int InVBO[3];
  glGenBuffers(3, InVBO);

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Offsets.size(), Offsets.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(3, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Size.size(), Size.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(4, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(int) * Temp.size(), Temp.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(5);
  glVertexAttribIPointer(5, 1, GL_UNSIGNED_INT, sizeof(GLuint), (GLvoid*)0);
  glVertexAttribDivisor(5, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, 0);

//   cy::GLSLProgram ProgS;
  if (!ProgS.BuildFiles("shaders/gsp.vert", "shaders/bigstar.frag", nullptr, nullptr, nullptr, 0, nullptr, &std::cerr)) {
    std::cerr << "Shader failed to compile.\n";
    return;
  }

  }

  void draw(TransformFunc F, cyVec3f Loc) override {
    glUseProgram(ProgS.GetID());
    BB.Bind(1);
    ProgS["tex"] = 1;
    ProgS["mvp"] = F(cy::Matrix4f::Translation(Loc));
    glBindVertexArray(VAOP);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, Size.size());
  }

  unsigned int VAOP;
  unsigned int VBOP[3];
  unsigned int InVBO[3];
  cy::GLSLProgram ProgS;
  sf::Image Billboard;
  cy::GLTexture2D BB;

  float PS = 10;

};

struct GalaxySprites : public Object {
  std::vector<float> Offsets;
  std::vector<float> Size;
  std::vector<uint> Temp; // 0 blue, 1 white, 2 yellow, 3 orange, 4 red
  std::vector<float> Lum; // TODO

  void read(std::string File) override {
    std::ifstream in(File);

    float x, y, z;
    float s;
    uint t;
    while(in >> x >> y >> z >> s >> t) {
      Offsets.push_back(x);
      Offsets.push_back(y);
      Offsets.push_back(z);
      Size.push_back(s);
      Temp.push_back(t);
    }
  }

  void setup() override {

  std::vector<std::string> Sprites = {
    "sprites/galaxies/mw.png",
    "sprites/galaxies/ngc1365.png",
    "sprites/galaxies/sp.png",
    "sprites/galaxies/ssd.png"
  };

  for (auto S : Sprites) {
    Billboard.loadFromFile(S);
    cy::GLTexture2D t;
    t.Initialize();
    t.SetImageRGBA(Billboard.getPixelsPtr(), Billboard.getSize().x, Billboard.getSize().y);
    t.BuildMipmaps();
    t.SetFilteringMode(GL_LINEAR, GL_LINEAR);
    t.SetAnisotropy(2.0f);
    BB.push_back(t);
  }

  std::vector<float> Triangles = {-PS, -PS, 0,  PS, PS, 0,  -PS, PS, 0  , -PS, -PS, 0,   PS, -PS, 0,   PS, PS, 0};
  std::vector<float> PlaneTex = {0, 0,     1, 1 ,      0 , 1,      0, 0 ,     1 , 0,      1 , 1};
  std::vector<float> VNormals =  {0, 0, 1,  0, 0, 1,   0, 0, 1   ,  0, 0, 1,  0, 0, 1,  0, 0, 1 };

//   unsigned int VAOP;
  glGenVertexArrays(1, &VAOP);

  glBindVertexArray(VAOP);

//   unsigned int VBOP[3];
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

//   unsigned int InVBO[3];
  glGenBuffers(3, InVBO);

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Offsets.size(), Offsets.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(3, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Size.size(), Size.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(4, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, InVBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(int) * Temp.size(), Temp.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(5);
  glVertexAttribIPointer(5, 1, GL_UNSIGNED_INT, sizeof(GLuint), (GLvoid*)0);
  glVertexAttribDivisor(5, 1); // This sets the vertex attribute to instanced attribute.

  glBindBuffer(GL_ARRAY_BUFFER, 0);

//   cy::GLSLProgram ProgS;
  if (!ProgS.BuildFiles("shaders/gsp.vert", "shaders/starsprites.frag", nullptr, nullptr, nullptr, 0, nullptr, &std::cerr)) {
    std::cerr << "Shader failed to compile.\n";
    return;
  }

  }

  void draw(TransformFunc F, cyVec3f Loc) override {
    glUseProgram(ProgS.GetID());

    for (int i = 0; i < BB.size(); ++i) {
      BB[i].Bind(i+1);
      std::string name = "tex" + std::to_string(i+1);
      ProgS[name.c_str()] = i+1;
    }

    ProgS["mvp"] = F(cy::Matrix4f::Translation(Loc));
    glBindVertexArray(VAOP);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, Size.size());
  }

  unsigned int VAOP;
  unsigned int VBOP[3];
  unsigned int InVBO[3];
  cy::GLSLProgram ProgS;
  sf::Image Billboard;
  std::vector<cy::GLTexture2D> BB;

  float PS = 100;

};

#endif
