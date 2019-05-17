#include <gtest/gtest.h>
#include "BoundingBox.h"
#include <ngl/NGLStream.h>
#include <ngl/Vec3.h>
#include <iomanip>
TEST(BoundingBox,defaultCtor)
{
  BoundingBox b;
  EXPECT_EQ(b.min(),ngl::Vec3::zero());
  EXPECT_EQ(b.max(),ngl::Vec3::zero());
}

TEST(BoundingBox,Vec3Ctor)
{
  BoundingBox b({-1.0f,-1.0f,-1.0f},{1.0f,1.0f,1.0f});
  EXPECT_EQ(b.min(),ngl::Vec3(-1.0f,-1.0f,-1.0f));
  EXPECT_EQ(b.max(),ngl::Vec3(1.0f,1.0f,1.0f));
}

TEST(BoundingBox,floatCtor)
{
  auto min=std::numeric_limits<float>::min();
  auto max=std::numeric_limits<float>::max();

  BoundingBox b(min,min,min,max,max,max);
  EXPECT_FLOAT_EQ(b.min().m_x,min);
  EXPECT_FLOAT_EQ(b.min().m_y,min);
  EXPECT_FLOAT_EQ(b.min().m_z,min);

  EXPECT_FLOAT_EQ(b.max().m_x,max);
  EXPECT_FLOAT_EQ(b.max().m_y,max);
  EXPECT_FLOAT_EQ(b.max().m_z,max);
}

TEST(BoundingBox,pointsCtor)
{
  std::vector<ngl::Vec3> points={
    {-10.0f,-10.0f,-10.0f},
    {0.0f,0.0f,0.0f},
    {1.0f,-1.0f,-1.0f},
    {10.0f,10.0f,10.0f}
  };

  BoundingBox b(points);
  EXPECT_EQ(b.min(),ngl::Vec3(-10.0f,-10.0f,-10.0f));
  EXPECT_EQ(b.max(),ngl::Vec3(10.0f,10.0f,10.0f));
  EXPECT_EQ(b.center(),ngl::Vec3::zero());
}


TEST(BoundingBox,center)
{
  auto min=std::numeric_limits<float>::min();
  auto max=std::numeric_limits<float>::max();

  BoundingBox b(min,min,min,0.0f,0.0f,0.0f);
  auto c=b.center();
  EXPECT_FLOAT_EQ(c.m_x,min/2.0f);
  EXPECT_FLOAT_EQ(c.m_y,min/2.0f);
  EXPECT_FLOAT_EQ(c.m_z,min/2.0f);
  b.set(0,0,0,max,max,max);
  c=b.center();
  EXPECT_FLOAT_EQ(c.m_x,max/2.0f);
  EXPECT_FLOAT_EQ(c.m_y,max/2.0f);
  EXPECT_FLOAT_EQ(c.m_z,max/2.0f);

  b.set({-2.0f,0.0023f,-100.043f},{{35.0034f,30.0023f,20.043f}});
  c=b.center();
  EXPECT_FLOAT_EQ(c.m_x,16.5017f);
  EXPECT_FLOAT_EQ(c.m_y,15.0023f);
  EXPECT_FLOAT_EQ(c.m_z,-40.0f);
}

TEST(BoundingBox,pointInBB)
{
  BoundingBox b({-1.0f,-1.0f,-1.0f},{1.0f,1.0f,1.0f});
  EXPECT_TRUE(b.inside({0.0f,0.0f,0.0f}));
  EXPECT_TRUE(b.inside({0.1f,0.0f,-0.2f}));
  EXPECT_FALSE(b.inside(1.2f,0.0f,-0.2f));
  EXPECT_FALSE(b.inside({99.2f,0.0f,0.2f}));

}


TEST(BoundingBox,intercept)
{
  BoundingBox b({-1.0f,-1.0f,-1.0f},{1.0f,1.0f,1.0f});
  // ray through center in X
  EXPECT_TRUE(b.intersect({-10.0f,0.0f,0.0f},{10.0f,0.0f,0.0f}));
  // ray through center in Y
  EXPECT_TRUE(b.intersect({0.0f,10.0f,0.0f},{0.0f,-1.0f,0.0f}));
  // ray through center in Z
  EXPECT_TRUE(b.intersect({0.0f,0.0f,2.0f},{0.0f,0.0f,-.5f}));
  // ray from corner to corner
  EXPECT_TRUE(b.intersect(b.min(),(b.max()-b.min())));

  // ray start outside and down
  EXPECT_FALSE(b.intersect({2.0f,2.0f,2.0f},{0.0f,-1.0f,0.0f}));
  // ray start outside and up
  EXPECT_FALSE(b.intersect({2.0f,2.0f,2.0f},{0.0f,1.0f,0.0f}));

}
