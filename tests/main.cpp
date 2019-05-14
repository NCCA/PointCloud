#include <gtest/gtest.h>
#include "PointCloud.h"
#include <ngl/NGLStream.h>

TEST(PointCloud,defaultCtor)
{
  PointCloud p;
  EXPECT_EQ(p.size(),0u);
}

TEST(PointCloud,loadEmpty)
{
  PointCloud p;
  auto loaded=p.load("files/empty.xyz");
  EXPECT_EQ(p.size(),0u);
  EXPECT_TRUE(loaded);
}

TEST(PointCloud,loadNotFound)
{
  PointCloud p;
  auto loaded=p.load("files/notthere.xyz");
  EXPECT_EQ(p.size(),0u);
  EXPECT_FALSE(loaded);
}

TEST(PointCloud,loadPoints)
{
  PointCloud p;
  auto loaded=p.load("files/pointsSmall.xyz");
  EXPECT_EQ(p.size(),8u);
  EXPECT_TRUE(loaded);
}

TEST(PointCloud,accessPoints)
{
  PointCloud p;
  auto loaded=p.load("files/pointsSmall.xyz");
  EXPECT_TRUE(loaded);
  auto data=p.points();
  EXPECT_FLOAT_EQ(data[0].m_x,492351.656250f);
  EXPECT_FLOAT_EQ(data[0].m_y,6523055.500000f);
  EXPECT_FLOAT_EQ(data[0].m_z,20.343550f);
  //, 6523055.500000f, 20.343550f)));
  std::cout<<data[0]<<'\n';
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
