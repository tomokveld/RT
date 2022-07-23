#ifndef Types_hpp
#define Types_hpp

#pragma once

#include <map>
#include <memory>

class Shape;
using ShapePtr = std::shared_ptr<Shape>;
using ShapeConstPtr = std::shared_ptr<Shape const>;

class Pattern;
using PatternPtr = std::shared_ptr<Pattern>;
using PatternConstPtr = std::shared_ptr<Pattern const>;

class Light;
using LightPtr = std::shared_ptr<Light>;
using LightConstPtr = std::shared_ptr<Light const>;

#endif /* Types_hpp */
