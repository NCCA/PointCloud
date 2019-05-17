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


