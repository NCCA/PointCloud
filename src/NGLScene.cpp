#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/ShaderLib.h>
#include <ngl/NGLStream.h>
#include <ngl/Random.h>
#include <iostream>
#include <ngl/pystring.h>
#include <numeric>
#include <unordered_set>
#include <algorithm>

NGLScene::NGLScene(const std::string &_fname) : m_filename{_fname}
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Point Cloud Viewer");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

template<>
    struct std::hash<ngl::Vec3>
    {
      int
      operator()(const ngl::Vec3 & obj) const
      {
        //return std::hash<int>()(
        //      obj.m_x+obj.m_y+obj.m_z);
        int hash=  int32_t(obj.m_x * 73856093) ^ int32_t(obj.m_y * 19349663) ^ int32_t(obj.m_z * 83492791);
        return hash;
      }
    };

    template< typename tPair >
    struct second_t {
        typename tPair::second_type operator()( const tPair& p ) const { return     p.second; }
    };

    template< typename tMap >
    second_t< typename tMap::value_type > second( const tMap& m ) { return second_t<     typename tMap::value_type >(); }




void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  auto shader=ngl::ShaderLib::instance();
  shader->use(ngl::nglColourShader);
  shader->setUniform("Colour",0.8f,0.8f,0.8f,1.0f);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.setf(std::ios::showpoint);
  if(m_filename.size() ==0)
    m_pointCloud.load("data/b2N.xyz");
  else
    m_pointCloud.load(m_filename);

  shader->loadShader("PointCloudShader","shaders/PCVertex.glsl","shaders/PCFragment.glsl");
  m_pointCloud.unitize();
  //m_pointCloud.normalize();
  createVAO();
 // m_view=ngl::lookAt(m_pointCloud.sphereCenter(),m_pointCloud.getBBox().center(),{0,1,0});
 // m_project=ngl::perspective(45.0f,float(width())/height(),0.1f,1000.0f);
  std::cout<<"Bounding Box Center "<<m_pointCloud.getBBox().center()<<'\n';
  std::cout<<"Bounding Sphere Center "<<m_pointCloud.sphereCenter()<<'\n';
  calculateCamera();
  ngl::VAOPrimitives::instance()->createSphere("bsphere",m_pointCloud.radius(),20);

}





void NGLScene::createVAO()
{
  m_pointCloudVAO=ngl::VAOFactory::createVAO(ngl::multiBufferVAO,GL_POINTS);
  m_pointCloudVAO->bind();

  // in this case we are going to set our data as the vertices above
  m_pointCloudVAO->setData(ngl::MultiBufferVAO::VertexData(m_pointCloud.size()*sizeof(ngl::Vec3),m_pointCloud.points()[0].m_x));
  // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
  m_pointCloudVAO->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_pointCloudVAO->setNumIndices(m_pointCloud.size());

 // now unbind
  m_pointCloudVAO->unbind();

}


void NGLScene::calculateCamera()
{
  auto bbox=m_pointCloud.getBBox();
  auto center = bbox.center();

  auto size = bbox.size();
  // get the max side of the bounding box (fits to width OR height as needed )
  auto maxDim = std::max( size.m_x, std::max(size.m_y, size.m_z ));
  auto fov = ngl::radians(m_fov);
  ngl::Real cameraZ=maxDim;

  cameraZ *= 1.25f; // zoom out a little so that objects don't fill the screen

  ngl::Vec3 eye=m_pointCloud.sphereCenter() ;
  eye.m_z=cameraZ;

  auto minZ = bbox.min().m_z;
  auto cameraToFarEdge = ( minZ < 0 ) ? -minZ + cameraZ : cameraZ - minZ;
  //m_far = cameraToFarEdge * 3.0f;
  m_view=ngl::lookAt(eye,center,{0,1,1});
  m_project=ngl::perspective(m_fov,float(width())/height(),m_near,m_far);
  std::cout<<"Eye "<<eye<<'\n';
  std::cout<<"Look "<<center<<'\n';
  std::cout<<"near "<<m_near<<" far "<<m_far<<'\n';
}


//void NGLScene::createGridVAO()
//{
//  m_gridVAO=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_LINES);
//  m_gridVAO->bind();
//  // so in this case the data is z is up.
//  // First we will create the lines along the Y Axis
//  // from Ymin -> yMax along the x
//  std::vector<ngl::Vec3> verts;
//  auto xmin=m_pointCloudBBox.minX();
//  auto xmax=m_pointCloudBBox.maxX();
//  for(float y=m_pointCloudBBox.minY(); y<m_pointCloudBBox.maxY(); y+=m_gridStepY )
//  {
//    verts.push_back({xmin,y,m_gridY});
//    verts.push_back({xmax,y,m_gridY});
//  }


//  auto ymin=m_pointCloudBBox.minY();
//  auto ymax=m_pointCloudBBox.maxY();
//  for(float x=m_pointCloudBBox.minX(); x<m_pointCloudBBox.maxX(); x+=m_gridStepX )
//  {
//    verts.push_back({x,ymin,m_gridY});
//    verts.push_back({x,ymax,m_gridY});
//  }

//  // in this case we are going to set our data as the vertices above
//  m_gridVAO->setData(ngl::SimpleVAO::VertexData(verts.size()*sizeof(ngl::Vec3),verts[0].m_x));
//  // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
//  m_gridVAO->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
//  m_gridVAO->setNumIndices(verts.size());

//  m_gridVAO->unbind();
// // ngl::msg->addMessage(fmt::format("Num of grid elements {0}",verts.size()));
//}


void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_win.spinXFace);
  rotY.rotateY(m_win.spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat4 M;
  M=m_mouseGlobalTX;
  MV=  m_view*M;
  MVP= m_project*MV;
  auto shader=ngl::ShaderLib::instance();
  shader->use("PointCloudShader");
  shader->setUniform("MVP",MVP);
  glPointSize(m_pointSize);
  m_pointCloudVAO->bind();
  m_pointCloudVAO->draw();
  m_pointCloudVAO->unbind();

  shader->use("nglColourShader");
  shader->setUniform("MVP",MVP);

//  if (m_showBBox)
//  {
//    shader->setUniform("Colour",0.8f,0.0f,0.0f,1.0f);
//    m_pointCloudBBox.draw();
//  }

//  if (m_showGrid)
//  {
//    createGridVAO();

//    shader->setUniform("Colour",0.8f,0.8f,0.0f,1.0f);
//    m_gridVAO->bind();
//    m_gridVAO->draw();
//    m_gridVAO->unbind();
//  }

  if(m_showSphere)
  {
    shader->setUniform("Colour",0.0f,0.9f,0.0f,1.0f);
    ngl::Mat4 translate;
    auto tx=m_pointCloud.sphereCenter();
    translate.translate(tx.m_x,tx.m_y,tx.m_z);

    M=m_mouseGlobalTX*translate;
    MV=  m_view*M;
    MVP= m_project*MV;
    shader->setUniform("MVP",MVP);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    ngl::VAOPrimitives::instance()->draw("bsphere");
  }
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

  break;
  //case Qt::Key_Down : m_gridY-=0.1f; break;
  //case Qt::Key_Up : m_gridY+=0.1f; break;
  case Qt::Key_G : m_showGrid^=true; break;
  case Qt::Key_B : m_showBBox^=true; break;
  case Qt::Key_S : m_showSphere^=true; break;
  //case Qt::Key_V : savePointCloud("data/save.xyz"); break;
  case Qt::Key_Equal : m_pointSize++; break;
  case Qt::Key_Minus : m_pointSize--; break;
  //case Qt::Key_W : writePointCloud("debug.xyz"); break;

  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}


