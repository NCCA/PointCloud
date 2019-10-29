#ifndef POINTCLOUD_H_
#define POINTCLOUD_H_

#include <vector>
#include <ngl/Vec3.h>
#include "BoundingBox.h"
#include <string_view>

class PointCloud
{
  public :
    PointCloud()=default ;
    PointCloud(const PointCloud &)=delete;
    PointCloud & operator=(const PointCloud &)=delete;
    PointCloud(PointCloud &&_p)=default;
    PointCloud & operator=(PointCloud &&)=default;
    PointCloud(const std::string_view &_fname) noexcept;
    // note load will auto calculate Bounding box and sphere
    bool load(const std::string_view &_name, bool _sort=false) noexcept;
    void removeDuplicates() noexcept;
    void calculateBoundingBox() noexcept;
    void calculateBoundingSphere() noexcept;
    ngl::Real radius() const noexcept;
    ngl::Vec3 sphereCenter() const noexcept;
    size_t size() const noexcept;
    std::vector<ngl::Vec3> &points() noexcept;
    void addPoint(const ngl::Vec3 &_p) noexcept;
    BoundingBox &getBBox() noexcept;
    // scale all points to 1 unit size
    void normalize();
    // canonicalize (scale to fit in NDC / Canonical volume -1 -> +1 in all directions)
    void unitize();

    bool writeObj(const std::string &_fname) const;
  private :
    std::vector<ngl::Vec3> m_points;
    BoundingBox m_bbox;
    ngl::Vec3 m_boundingSphereCenter={0.0f,0.0f,0.0f};
    ngl::Real m_boundingSphereRadius=0.0f;
};


#endif
