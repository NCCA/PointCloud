#include "BoundingBox.h"

BoundingBox::BoundingBox(const ngl::Vec3 &_min, const ngl::Vec3 _max)  noexcept :
  m_min{_min}, m_max{_max} {}



ngl::Vec3 BoundingBox::min() const noexcept
{
  return m_min;
}
ngl::Vec3  BoundingBox::max() const noexcept
{
  return m_max;
}
