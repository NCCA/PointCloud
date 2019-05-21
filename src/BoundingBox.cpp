#include "BoundingBox.h"

BoundingBox::BoundingBox(const ngl::Vec3 &_min, const ngl::Vec3 _max)  noexcept :
  m_min{_min}, m_max{_max} {}

BoundingBox::BoundingBox(ngl::Real _m_minX, ngl::Real _m_minY, ngl::Real _m_minZ, ngl::Real _m_maxX, ngl::Real _m_maxY,ngl::Real _m_maxZ ) noexcept
{
  m_min.set(_m_minX,_m_minY,_m_minZ);
  m_max.set(_m_maxX,_m_maxY,_m_maxZ);
}

void BoundingBox::set(ngl::Real _m_minX, ngl::Real _m_minY, ngl::Real _m_minZ, ngl::Real _m_maxX, ngl::Real _m_maxY,ngl::Real _m_maxZ )
{
  m_min.set(_m_minX,_m_minY,_m_minZ);
  m_max.set(_m_maxX,_m_maxY,_m_maxZ);
}
void BoundingBox::set(const ngl::Vec3 &_m_min, const ngl::Vec3 _m_max )
{
  m_min=_m_min;
  m_max=_m_max;
}

void BoundingBox::set(const std::vector<ngl::Vec3> &_p ) noexcept
{
  calcExtents(_p);
}


bool BoundingBox::inside(const ngl::Vec3 &_p) const noexcept
{
  return  (_p.m_x >= m_min.m_x && _p.m_x <= m_max.m_x &&
           _p.m_y >= m_min.m_y && _p.m_y <= m_max.m_y &&
           _p.m_z >= m_min.m_z && _p.m_z <= m_max.m_z
           );
}
bool BoundingBox::inside(ngl::Real _x, ngl::Real _y, ngl::Real _z) const noexcept
{
  return  (_x >= m_min.m_x && _x <= m_max.m_x &&
           _y >= m_min.m_y && _y <= m_max.m_y &&
           _z >= m_min.m_z && _z <= m_max.m_z
           );}

void BoundingBox::calcExtents(const std::vector<ngl::Vec3> &_p)
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

BoundingBox::BoundingBox(const std::vector<ngl::Vec3> &_p ) noexcept
{
  calcExtents(_p);
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

ngl::Real BoundingBox::width() const noexcept
{
  return m_max.m_x-m_min.m_x;
}
ngl::Real BoundingBox::height() const noexcept
{
  return m_max.m_y-m_min.m_y;

}
ngl::Real BoundingBox::depth() const noexcept
{
  return m_max.m_z-m_min.m_z;

}
// vector combining width, height and depth
ngl::Vec3 BoundingBox::size() const noexcept
{
  return m_max-m_min;

}

// based on
// https://tavianator.com/fast-branchless-raybounding-box-intersections/
//
bool BoundingBox::intersect(const ngl::Vec3 &_rpos, const ngl::Vec3 &_rdir) const noexcept
{
  float tm_min = (m_min.m_x - _rpos.m_x) / _rdir.m_x;
    float tm_max = (m_max.m_x - _rpos.m_x) / _rdir.m_x;

    if (tm_min > tm_max) std::swap(tm_min, tm_max);

    float tym_min = (m_min.m_y - _rpos.m_y) / _rdir.m_y;
    float tym_max = (m_max.m_y - _rpos.m_y) / _rdir.m_y;

    if (tym_min > tym_max) std::swap(tym_min, tym_max);

    if ((tm_min > tym_max) || (tym_min > tm_max))
        return false;

    if (tym_min > tm_min)
        tm_min = tym_min;

    if (tym_max < tm_max)
        tm_max = tym_max;

    float tzm_min = (m_min.m_z - _rpos.m_z) / _rdir.m_z;
    float tzm_max = (m_max.m_z - _rpos.m_z) / _rdir.m_z;

    if (tzm_min > tzm_max) std::swap(tzm_min, tzm_max);

    if ((tm_min > tzm_max) || (tzm_min > tm_max))
        return false;

    if (tzm_min > tm_min)
        tm_min = tzm_min;

    if (tzm_max < tm_max)
        tm_max = tzm_max;

    return true;
}

//bool intersect(const Ray &r) const
//{
//    float tm_min, tm_max, tym_min, tym_max, tzm_min, tzm_max;

//    tm_min = (bounds[r.sign[0]].m_x - _rpos.m_x) * r.invdir.m_x;
//    tm_max = (bounds[1-r.sign[0]].m_x - _rpos.m_x) * r.invdir.m_x;
//    tym_min = (bounds[r.sign[1]].m_y - _rpos.m_y) * r.invdir.m_y;
//    tym_max = (bounds[1-r.sign[1]].m_y - _rpos.m_y) * r.invdir.m_y;

//    if ((tm_min > tym_max) || (tym_min > tm_max))
//        return false;
//    if (tym_min > tm_min)
//        tm_min = tym_min;
//    if (tym_max < tm_max)
//        tm_max = tym_max;

//    tzm_min = (bounds[r.sign[2]].m_z - _rpos.m_z) * r.invdir.m_z;
//    tzm_max = (bounds[1-r.sign[2]].m_z - _rpos.m_z) * r.invdir.m_z;

//    if ((tm_min > tzm_max) || (tzm_min > tm_max))
//        return false;
//    if (tzm_min > tm_min)
//        tm_min = tzm_min;
//    if (tzm_max < tm_max)
//        tm_max = tzm_max;

//    return true;
//}
