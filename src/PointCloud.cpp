#include "PointCloud.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ngl/pystring.h>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <ngl/NGLStream.h>
PointCloud::PointCloud(const std::string_view &_fname) noexcept
{
  load(_fname);
}

size_t PointCloud::size() const noexcept
{
  return m_points.size();
}



bool PointCloud::load(const std::string_view &_name, bool _sort) noexcept
{
  namespace ps=pystring;

  std::ifstream in(_name.data());
  if (in.is_open() != true)
  {
    ngl::msg->addError(fmt::format(" file {0} not found  ",_name.data()));
    return false;
  }
  m_points.clear();
  std::string str;
  // Read the next line from File untill it reaches the end.
  while (std::getline(in, str))
  {
  // Line contains string of length > 0 then parse it
    if(str.size() > 0)
    {
      std::vector<std::string> tokens;
      ps::split(str,tokens);
      // should have x,y,z
      if(tokens.size() >=3)
      {
        float x=std::stof(tokens[0]);
        float y=std::stof(tokens[1]);
        float z=std::stof(tokens[2]);

        m_points.push_back({x,y,z});

      }
    } // str.size()

  } // while



  in.close();
  if(_sort)
  {
    std::sort(std::begin(m_points),std::end(m_points),
              [](const ngl::Vec3 &a,const ngl::Vec3 &b) {return a.m_x > b.m_x; }
              );
  }
  ngl::msg->addMessage(fmt::format("Point Cloud Loaded {0} points",m_points.size()));
  calculateBoundingBox();
  calculateBoundingSphere();
  return true;


}

std::vector<ngl::Vec3> &PointCloud::points() noexcept
{
  return  m_points;
}

template<>
    struct std::hash<ngl::Vec3>
    {
      size_t
      operator()(const ngl::Vec3 & obj) const
      {
        //return std::hash<int>()(
        //      obj.m_x+obj.m_y+obj.m_z);
      //  size_t hash=  int32_t(obj.m_x * 73856093) ^ int32_t(obj.m_y * 19349663) ^ int32_t(obj.m_z * 83492791);
      //  return hash;
         auto hash= std::hash<float>{}(obj.m_x)+
         std::hash<float>{}(obj.m_y)+
      std::hash<float>{}(obj.m_z);
      //std::cout<<"Hash "<<hash<<'\n';
      return  hash;
      }
    };


template< typename tPair >
struct second_t {
    typename tPair::second_type operator()( const tPair& p ) const { return     p.second; }
};

template< typename tMap >
second_t< typename tMap::value_type > second( const tMap& m ) { return second_t<     typename tMap::value_type >(); }



void PointCloud::removeDuplicates() noexcept
{
  std::unordered_map<size_t,ngl::Vec3> set;
  for( auto d : m_points )
    set[std::hash<ngl::Vec3>{}(d)]=d;
  m_points.clear();
  std::transform( std::begin(set), std::end(set), std::back_inserter( m_points ), second(set) );
}

BoundingBox &PointCloud::getBBox() noexcept
{
  return m_bbox;
}

void PointCloud::calculateBoundingBox() noexcept
{
  m_bbox.set(m_points);
}


void PointCloud::addPoint(const ngl::Vec3 &_p) noexcept
{
  m_points.push_back(_p);
}

void PointCloud::calculateBoundingSphere() noexcept
{
  if(m_points.size() == 0)
    return;
  // find minimal and maximal extents and indexs into
  // into vert array
  size_t minXI=0; size_t minYI=0; size_t minZI=0;
  size_t maxXI=0; size_t maxYI=0; size_t maxZI=0;
  ngl::Real minX=m_points[0].m_x; ngl::Real maxX=m_points[0].m_x;
  ngl::Real minY=m_points[0].m_y; ngl::Real maxY=m_points[0].m_y;
  ngl::Real minZ=m_points[0].m_z; ngl::Real maxZ=m_points[0].m_z;

  for(size_t i=0; i<m_points.size(); ++i)
  {
    if(m_points[i].m_x < minX) { minXI=i; minX=m_points[i].m_x; }
    if(m_points[i].m_x > maxX) { maxXI=i; maxX=m_points[i].m_x; }
    if(m_points[i].m_y < minY) { minYI=i; minY=m_points[i].m_y; }
    if(m_points[i].m_y > maxY) { maxYI=i; maxY=m_points[i].m_y; }
    if(m_points[i].m_z < minZ) { minZI=i; minZ=m_points[i].m_z; }
    if(m_points[i].m_z > maxZ) { maxZI=i; maxZ=m_points[i].m_z; }
  }
  // now we find maximally seperated points from the 3 pairs
  // we will use this to initialise the spheres
  ngl::Real dx=m_points[minXI].m_x-m_points[maxXI].m_x;
  ngl::Real dy=m_points[minXI].m_y-m_points[maxXI].m_y;
  ngl::Real dz=m_points[minXI].m_z-m_points[maxXI].m_z;
  ngl::Real diam2x=dx*dx+dy*dy+dz*dz;

  dx=m_points[minYI].m_x-m_points[maxYI].m_x;
  dy=m_points[minYI].m_y-m_points[maxYI].m_y;
  dz=m_points[minYI].m_z-m_points[maxYI].m_z;
  ngl::Real diam2y=dx*dx+dy*dy+dz*dz;

  dx=m_points[minZI].m_x-m_points[maxZI].m_x;
  dy=m_points[minZI].m_y-m_points[maxZI].m_y;
  dz=m_points[minZI].m_z-m_points[maxZI].m_z;
  ngl::Real diam2z=dx*dx+dy*dy+dz*dz;

  ngl::Real diamTwo=diam2x;
  size_t p1i=minXI;
  size_t p2i=maxXI;
  if(diam2y>diamTwo){ diamTwo=diam2y; p1i=minYI; p2i=maxYI;}
  if(diam2z>diamTwo){ diamTwo=diam2z; p1i=minZI; p2i=maxZI;}
  // now we can get the center of the sphere as the average
  // of the two points
  m_boundingSphereCenter=(m_points[p1i]+m_points[p2i])/2.0;
  // now calculate radius and radius^2 of the initial sphere
  ngl::Real radTwo=diamTwo/4.0f;
  ngl::Real rad=sqrt(radTwo);
  // now check and adjust for outlying points
  ngl::Vec3 newCenter;
  ngl::Real newRad2;
  ngl::Real newRad;
  ngl::Real dist2;
  ngl::Real dist;
  ngl::Real delta;

  for (auto v : m_points)
  {
    dx=v.m_x-m_boundingSphereCenter.m_x;
    dy=v.m_y-m_boundingSphereCenter.m_y;
    dz=v.m_z-m_boundingSphereCenter.m_z;
    // distance squared of old center to current point
    dist2=dx*dx+dy*dy+dz*dz;
    // need to update the sphere if this point is outside the radius
    if(dist2 > radTwo)
    {
      dist=sqrt(dist2);
      newRad=(rad+dist)/2.0f;
      newRad2=newRad*newRad;
      delta=dist-newRad;
      // now compute new center using the weights above
      newCenter.m_x=(newRad*m_boundingSphereCenter.m_x+delta*v.m_x)/dist;
      newCenter.m_y=(newRad*m_boundingSphereCenter.m_y+delta*v.m_y)/dist;
      newCenter.m_z=(newRad*m_boundingSphereCenter.m_z+delta*v.m_z)/dist;
      // now test to see if we have a fit
      dx=v.m_x-newCenter.m_x;
      dy=v.m_y-newCenter.m_y;
      dz=v.m_z-newCenter.m_z;
      dist2=dx*dx+dy*dy+dz*dz;
      if(dist2 > newRad2)
      {
        //ngl::msg->addWarning(fmt::format("something wrong here error margin {0}",dist2-newRad2));
      }
      m_boundingSphereCenter=newCenter;
      rad=newRad;
      radTwo=rad*rad;
    } // end if dist2>rad2
    m_boundingSphereRadius=rad;
}
}

ngl::Real PointCloud::radius() const noexcept
{
  return m_boundingSphereRadius;
}
ngl::Vec3 PointCloud::sphereCenter() const noexcept
{
  return m_boundingSphereCenter;
}



