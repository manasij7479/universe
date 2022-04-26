#ifndef SCENE_H
#define SCENE_H

#include <fstream>
#include <iostream>
#include <string>

#include "Object.h"
#include "Galaxy.h"
#include "Nebula.h"

/* Syntax - Lines consisting of:
 * ObjectType FileName Location
 */

class Scene {
public:
  Scene(std::string File) {
    std::ifstream in(File);
    std::string Type, FileName;
    float x, y, z;
    while (in  >> Type >> FileName >> x >> y >> z) {
      Object *O = nullptr;
      if (Type == "Galaxy" || Type == "G" || Type == "g") {
        O = new Galaxy();
      } else if (Type == "EmissionNebula" || Type == "EN" || Type == "en") {
        O = new EmissionNebula();
      } else if (Type == "ReflectionNebula" || Type == "RN" || Type == "rn") {
        O = new ReflectionNebula();
      } else if (Type == "BigStar" || Type == "BS" || Type == "bs") {
        O = new BigStar();
        LightLocs.push_back({x, y, z});
      } else if (Type == "GalaxySprites" || Type == "GS" || Type == "gs") {
        O = new GalaxySprites();
      } else {
        std::cerr << "Failed to read " << Type << " " << FileName << "\n";
        continue;
      }
      if (!O) {
        std::cerr << "Failed to read " << Type << " " << FileName << "\n";
        continue;
      }
      O->read(FileName);
      O->setup();
      Objects.push_back(O);
      Locs.push_back({x, y, z});
    }

    for (int i = 0; i < Objects.size(); ++i) {
      if (auto *RN = dynamic_cast<ReflectionNebula *>(Objects[i])) {
        std::vector<cyVec3f> LightDirs;
        for (auto LL : LightLocs) {
          LightDirs.push_back(Locs[i] - LL);
        }
        RN->setLightLocs(LightDirs);
      }
    }

  }

  void operator()(TransformFunc F) {

    for (int i = 0; i < Objects.size(); ++i) {
      Objects[i]->draw(F, Locs[i]);
    }
  }
private:
  std::vector<Object *> Objects;
  std::vector<cyVec3f> Locs;
  std::vector<cyVec3f> LightLocs;

};

#endif
