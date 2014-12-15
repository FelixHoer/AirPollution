#ifndef _BEZIER_H_
#define _BEZIER_H_

#include <vector>
#include <glm/glm.hpp>

class Bezier
{
public:

  static glm::vec2 curve(std::vector<glm::vec2>& points, float t);
  static glm::vec2 tangent(std::vector<glm::vec2>& points, float t);

  static glm::dvec2 curve(std::vector<glm::dvec2>& points, float t);
  static glm::dvec2 tangent(std::vector<glm::dvec2>& points, float t);

};

#endif

