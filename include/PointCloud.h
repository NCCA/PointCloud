#ifndef POINTCLOUD_H_
#define POINTCLOUD_H_

#include <vector>
#include <ngl/Vec3.h>
#include <string_view>
class PointCloud
{
  public :
    PointCloud()=default;
    PointCloud(const PointCloud &)=delete;
    PointCloud & operator=(const PointCloud &)=delete;
    PointCloud(PointCloud &&_p)=default;
    PointCloud & operator=(PointCloud &&)=default;
    PointCloud(const std::string_view &_fname);
    bool load(const std::string_view &_name);

  private :
    std::vector<ngl::Vec3> m_points;
};


#endif
