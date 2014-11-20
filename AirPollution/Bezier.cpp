#include "Bezier.h"

glm::vec2 Bezier::curve(std::vector<glm::vec2>& points, float t_curve)
{
  size_t points_size = points.size();
  int segments = (points_size - 1) / 3;

  int segment = int(t_curve * segments);
  if (segment < 0)
    return points[0];
  else if (segment >= segments)
    return points[points_size - 1];

  // select 4 points of segment
  glm::vec2 p0 = points[segment * 3 + 0];
  glm::vec2 p1 = points[segment * 3 + 1];
  glm::vec2 p2 = points[segment * 3 + 2];
  glm::vec2 p3 = points[segment * 3 + 3];

  // find t_s for t in that segment
  float t = (t_curve - segment * 1.0f / segments) * segments;

  // use bernstein coefficients to calculate value
  float b0 = -t*t*t + 3.0f*t*t - 3.0f*t + 1.0f;
  float b1 = 3.0f*t*t*t - 6.0f*t*t + 3.0f*t;
  float b2 = -3.0f*t*t*t + 3.0f*t*t;
  float b3 = t*t*t;

  return b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
}

glm::vec2 Bezier::tangent(std::vector<glm::vec2>& points, float t_curve)
{
  size_t points_size = points.size();
  int segments = (points_size - 1) / 3;

  int segment = int(t_curve * segments);
  if (segment < 0)
    return points[0];
  else if (segment >= segments)
    return points[points_size - 1];

  // select 4 points of segment
  glm::vec2 p0 = points[segment * 3 + 0];
  glm::vec2 p1 = points[segment * 3 + 1];
  glm::vec2 p2 = points[segment * 3 + 2];
  glm::vec2 p3 = points[segment * 3 + 3];

  // find t_s for t in that segment
  float t = (t_curve - segment * 1.0f / segments) * segments;

  // use derived bernstein coefficients to calculate value
  float b0 = -3.0f*t*t + 6.0f*t - 3.0f;
  float b1 = 9.0f*t*t - 12.0f*t + 3.0f;
  float b2 = -9.0f*t*t + 6.0f*t;
  float b3 = 3.0f*t*t;

  return b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
}