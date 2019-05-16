#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_
#include <ngl/Vec3.h>

class BoundingBox
{
  public :
    BoundingBox()=default;
    BoundingBox(const ngl::Vec3 &_min, const ngl::Vec3 _max) noexcept;
    BoundingBox(const BoundingBox &)=default;
    BoundingBox( BoundingBox &&)=default;
    BoundingBox & operator=(const BoundingBox &)=default;
    BoundingBox & operator=( BoundingBox &&)=default;
    ~BoundingBox()=default;
    ngl::Vec3 min() const noexcept;
    ngl::Vec3 max() const noexcept;

  private :
    ngl::Vec3 m_min;
    ngl::Vec3 m_max;


};


#endif
