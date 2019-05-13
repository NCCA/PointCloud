#include "PointCloud.h"
#include <iostream>
#include <fstream>
#include <ngl/pystring.h>
#include <numeric>


PointCloud::PointCloud(const std::string_view &_fname)
{
  load(_fname);
}


bool PointCloud::load(const std::string_view &_name)
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
    bool status=true;
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
//        if(tokens.size()==6)
//        {
//          float nx=std::stof(tokens[3]);
//          float ny=std::stof(tokens[4]);
//          float nz=std::stof(tokens[5]);
//          m_normalCloud.push_back({nx,ny,nz});
//        }
      }
    } // str.size()
    // early out sanity checks!
    if(status == false)
      return false;
  } // while



  in.close();

  std::sort(std::begin(m_points),std::end(m_points),
            [](const ngl::Vec3 &a,const ngl::Vec3 &b) {return a.m_x > b.m_x; }
            );
  ngl::msg->addMessage(fmt::format("Point Cloud Loaded {0} points",m_points.size()));
  return true;


}
