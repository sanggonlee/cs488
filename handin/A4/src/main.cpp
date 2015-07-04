#include <iostream>
#include <string.h>
#include "scene_lua.hpp"
#include "a4.hpp"

bool useMirrorEffect;

int main(int argc, char** argv)
{
  //std::string filename = "scene.lua";
  std::string filename = "nonhier.lua";
  if (argc >= 2) {
    filename = argv[1];
  }
  
  if (argc >= 3 && strcmp(argv[2], "-nomirror") == 0) {
  	useMirrorEffect = false;
  } else {
  	useMirrorEffect = true;
  }

  if (!run_lua(filename)) {
    std::cerr << "Could not open " << filename << std::endl;
    return 1;
  }
}

