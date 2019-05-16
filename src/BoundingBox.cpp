#include "BoundingBox.h"

BoundingBox::BoundingBox(const ngl::Vec3 &_min, const ngl::Vec3 _max)  noexcept :
  m_min{_min}, m_max{_max} {}

BoundingBox::BoundingBox(ngl::Real _minX, ngl::Real _minY, ngl::Real _minZ, ngl::Real _maxX, ngl::Real _maxY,ngl::Real _maxZ ) noexcept
{
  m_min.set(_minX,_minY,_minZ);
  m_max.set(_maxX,_maxY,_maxZ);
}

void BoundingBox::set(ngl::Real _minX, ngl::Real _minY, ngl::Real _minZ, ngl::Real _maxX, ngl::Real _maxY,ngl::Real _maxZ )
{
  m_min.set(_minX,_minY,_minZ);
  m_max.set(_maxX,_maxY,_maxZ);
}
void BoundingBox::set(const ngl::Vec3 &_min, const ngl::Vec3 _max )
{
  m_min=_min;
  m_max=_max;
}


BoundingBox::BoundingBox(const std::vector<ngl::Vec3> &_p ) noexcept
{

  m_min.set(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
  m_max.set(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min());
  for(auto v : _p)
  {
    if     (v.m_x >m_max.m_x) { m_max.m_x=v.m_x; }
    else if(v.m_x <m_min.m_x) { m_min.m_x=v.m_x; }
    if     (v.m_y >m_max.m_y) { m_max.m_y=v.m_y; }
    else if(v.m_y <m_min.m_y) { m_min.m_y=v.m_y; }
    if     (v.m_z >m_max.m_z) { m_max.m_z=v.m_z; }
    else if(v.m_z <m_min.m_z) { m_min.m_z=v.m_z; }
  }

}

ngl::Vec3 BoundingBox::center() const
{
  return (m_min+m_max)/2.0f;
}


ngl::Vec3 BoundingBox::min() const noexcept
{
  return m_min;
}
ngl::Vec3  BoundingBox::max() const noexcept
{
  return m_max;
}
