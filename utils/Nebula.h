#ifndef NEBULA_IMPORTER_H
#define NEBULA_IMPORTER_H

#include <vector>
#include <fstream>

struct EmissionNebula : public Object {
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
  if (!ProgS.BuildFiles("shaders/nebula.vert", "shaders/nebula.frag", nullptr, nullptr, nullptr, 0, nullptr, &std::cerr)) {
    std::cerr << "Shader failed to compile.\n";
    return;
  }

  }

  void draw(TransformFunc F, cyVec3f Loc) override {
    glUseProgram(ProgS.GetID());
    BB.Bind(1);
    ProgS["tex"] = 1;
    ProgS["mvp"] = F(cy::Matrix4f::Translation(Loc));;
    glBindVertexArray(VAOP);
//   glDrawArrays(GL_TRIANGLES, 0, Count);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, Size.size());
  }

  unsigned int VAOP;
  unsigned int VBOP[3];
  unsigned int InVBO[3];
  cy::GLSLProgram ProgS;
  sf::Image Billboard;
  cy::GLTexture2D BB;

  float PS = 20;

};

struct ReflectionNebula : public Object {
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
  Billboard.loadFromFile("sprites/star3.png");
  BB.Initialize();
  BB.SetImageRGBA(Billboard.getPixelsPtr(), Billboard.getSize().x, Billboard.getSize().y);
  BB.BuildMipmaps();
  BB.SetFilteringMode(GL_LINEAR, GL_LINEAR);
  BB.SetAnisotropy(2.0f);

  Norm.loadFromFile("sprites/nebula-normal.jpg");
  NT.Initialize();
  NT.SetImageRGBA(Norm.getPixelsPtr(), Norm.getSize().x, Norm.getSize().y);
  NT.BuildMipmaps();
  NT.SetFilteringMode(GL_LINEAR, GL_LINEAR);
  NT.SetAnisotropy(2.0f);

  std::vector<float> Triangles = {-PS, -PS, 0,  PS, PS, 0,   -PS, PS, 0,  -PS, -PS, 0,  PS, -PS, 0,  PS, PS, 0};
  std::vector<float> PlaneTex =  {0, 0,         1, 1 ,       0 , 1,       0, 0 ,        1 , 0,       1, 1};
  std::vector<float> VNormals =  {1, 1, 0,    -1, -1, 0,     1, -1, 0,    1, 1, 0,    -1, 1, 0,    -1, -1, 0 };

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
  if (!ProgS.BuildFiles("shaders/reflection-nebula.vert", "shaders/reflection-nebula.frag", nullptr, nullptr, nullptr, 0, nullptr, &std::cerr)) {
    std::cerr << "Shader failed to compile.\n";
    return;
  }

  }

  void draw(TransformFunc F, cyVec3f Loc) override {
    glUseProgram(ProgS.GetID());

    BB.Bind(1);
    NT.Bind(2);
    ProgS["tex"] = 1;
    ProgS["norm"] = 2;

    ProgS["mvp"] = F(cy::Matrix4f::Translation(Loc));;
    glBindVertexArray(VAOP);
//   glDrawArrays(GL_TRIANGLES, 0, Count);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, Size.size());
  }

  unsigned int VAOP;
  unsigned int VBOP[3];
  unsigned int InVBO[3];
  cy::GLSLProgram ProgS;
  sf::Image Billboard;
  cy::GLTexture2D BB;

  sf::Image Norm;
  cy::GLTexture2D NT;

  float PS = 20;

};

#endif
