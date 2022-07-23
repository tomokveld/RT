#ifndef MengerSponge_hpp
#define MengerSponge_hpp

#include "Shape.hpp"
#include "Group.hpp"
#include "CSG.hpp"
#include "Cube.hpp"

std::shared_ptr<CSG> MengerSponge(int m);

std::shared_ptr<Group> MakeCubes(double x1, double y1, double x2, double y2, int n, int max);

#endif /* MengerSponge_hpp */
