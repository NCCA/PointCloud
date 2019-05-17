#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_
#include <ngl/Vec3.h>
#include <vector>
class BoundingBox
{
  public :
    BoundingBox()=default;
    BoundingBox(const ngl::Vec3 &_min, const ngl::Vec3 _max) noexcept;
    BoundingBox(ngl::Real _minX, ngl::Real _minY, ngl::Real _minZ, ngl::Real _maxX, ngl::Real _maxY,ngl::Real _maxZ ) noexcept;
    BoundingBox(const std::vector<ngl::Vec3> &_p ) noexcept;
    ngl::Vec3 center() const;
    void set(ngl::Real _minX, ngl::Real _minY, ngl::Real _minZ, ngl::Real _maxX, ngl::Real _maxY,ngl::Real _maxZ );
    void set(const ngl::Vec3 &_min, const ngl::Vec3 _max );
    void set(const std::vector<ngl::Vec3> &_p) noexcept;
    bool inside(const ngl::Vec3 &_p) const noexcept;
    bool inside(ngl::Real _x, ngl::Real _y, ngl::Real _z) const noexcept;
    bool intersect(const ngl::Vec3 &_rpos,const ngl::Vec3 &_rdir) const noexcept;

    BoundingBox(const BoundingBox &)=default;
    BoundingBox( BoundingBox &&)=default;
    BoundingBox & operator=(const BoundingBox &)=default;
    BoundingBox & operator=( BoundingBox &&)=default;
    ~BoundingBox()=default;
    ngl::Vec3 min() const noexcept;
    ngl::Vec3 max() const noexcept;

  private :
    void calcExtents(const std::vector<ngl::Vec3> &_p);

    ngl::Vec3 m_min;
    ngl::Vec3 m_max;


};


#endif
