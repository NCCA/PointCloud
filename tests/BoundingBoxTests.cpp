#include <gtest/gtest.h>
#include "BoundingBox.h"
#include <ngl/NGLStream.h>
#include <ngl/Vec3.h>
TEST(BoundingBox,defaultCtor)
{
  BoundingBox b;
  EXPECT_EQ(b.min(),  ngl::Vec3::zero());
  EXPECT_EQ(b.max(),  ngl::Vec3::zero());
}

TEST(BoundingBox,vectorCtor)
{
  BoundingBox b({-1.0f,-1.0f,-1.0f},{1.0f,1.0f,1.0f});
  EXPECT_EQ(b.min(),  ngl::Vec3(-1.0f,-1.0f,-1.0f));
  EXPECT_EQ(b.max(),  ngl::Vec3(1.0f,1.0f,1.0f));
}
