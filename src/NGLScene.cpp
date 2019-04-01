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
  //createPointCloud();
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.setf(std::ios::showpoint);
  if(m_filename.size() ==0)
    loadPointCloud("data/b2N.xyz");
  else
    loadPointCloud(m_filename);
  //scalePointCloud({0.0001f,0.0001f,1.0f});
  calculateBoundingBox();
  calculateBoundingSphere();
  std::cout<<m_pointCloudBBox.width()<<' ';
  std::cout<<m_pointCloudBBox.height()<<' ';
  std::cout<<m_pointCloudBBox.depth()<<' ';

  createVAO();
  calculateCamera();
  ngl::VAOPrimitives::instance()->createSphere("sphere",m_boundingSphereRadius,20);
  shader->loadShader("PointCloudShader","shaders/PCVertex.glsl","shaders/PCFragment.glsl");
}

void NGLScene::scalePointCloud(const ngl::Vec3 &_s)
{
  ngl::Mat3 scale;
  scale.scale(_s.m_x,_s.m_y,_s.m_z);
  for(auto &p : m_pointCloud)
    p=scale*p;
}

void NGLScene::calculateBoundingBox()
{
  // Calculate the center of the object.
  m_boundingBoxCenter=0.0;


  //m_boundingBoxCenter/=m_pointCloud.size();
 // 'previous mean' * '(count -1)') + 'new value') / 'count'
  m_boundingBoxCenter=m_pointCloud[0];
  for(size_t i=1; i<m_pointCloud.size(); ++i)
  {
    m_boundingBoxCenter=(m_boundingBoxCenter *(i-1) + m_pointCloud[i] )/i;
  }

  // calculate the extents
  ngl::Real maxX=m_boundingBoxCenter.m_x;
  ngl::Real minX=m_boundingBoxCenter.m_x;
  ngl::Real maxY=m_boundingBoxCenter.m_y;
  ngl::Real minY=m_boundingBoxCenter.m_y;
  ngl::Real maxZ=m_boundingBoxCenter.m_z;
  ngl::Real minZ=m_boundingBoxCenter.m_z;



  for(auto v : m_pointCloud)
  {
    if     (v.m_x >maxX) { maxX=v.m_x; }
    else if(v.m_x <minX) { minX=v.m_x; }
    if     (v.m_y >maxY) { maxY=v.m_y; }
    else if(v.m_y <minY) { minY=v.m_y; }
    if     (v.m_z >maxZ) { maxZ=v.m_z; }
    else if(v.m_z <minZ) { minZ=v.m_z; }
  }

  m_pointCloudBBox.setCenter(m_boundingBoxCenter);
  m_pointCloudBBox.setExtents(minX,maxX,minY,maxY,minZ,maxZ);
  m_pointCloudBBox.recalculate();
}
void NGLScene::calculateBoundingSphere()
{
  // find minimal and maximal extents and indexs into
  // into vert array
  size_t minXI=0; size_t minYI=0; size_t minZI=0;
  size_t maxXI=0; size_t maxYI=0; size_t maxZI=0;
  ngl::Real minX=m_pointCloud[0].m_x; ngl::Real maxX=m_pointCloud[0].m_x;
  ngl::Real minY=m_pointCloud[0].m_y; ngl::Real maxY=m_pointCloud[0].m_y;
  ngl::Real minZ=m_pointCloud[0].m_z; ngl::Real maxZ=m_pointCloud[0].m_z;

  for(size_t i=0; i<m_pointCloud.size(); ++i)
  {
    if(m_pointCloud[i].m_x < minX) { minXI=i; minX=m_pointCloud[i].m_x; }
    if(m_pointCloud[i].m_x > maxX) { maxXI=i; maxX=m_pointCloud[i].m_x; }
    if(m_pointCloud[i].m_y < minY) { minYI=i; minY=m_pointCloud[i].m_y; }
    if(m_pointCloud[i].m_y > maxY) { maxYI=i; maxY=m_pointCloud[i].m_y; }
    if(m_pointCloud[i].m_z < minZ) { minZI=i; minZ=m_pointCloud[i].m_z; }
    if(m_pointCloud[i].m_z > maxZ) { maxZI=i; maxZ=m_pointCloud[i].m_z; }
  }
  // now we find maximally seperated points from the 3 pairs
  // we will use this to initialise the spheres
  ngl::Real dx=m_pointCloud[minXI].m_x-m_pointCloud[maxXI].m_x;
  ngl::Real dy=m_pointCloud[minXI].m_y-m_pointCloud[maxXI].m_y;
  ngl::Real dz=m_pointCloud[minXI].m_z-m_pointCloud[maxXI].m_z;
  ngl::Real diam2x=dx*dx+dy*dy+dz*dz;

  dx=m_pointCloud[minYI].m_x-m_pointCloud[maxYI].m_x;
  dy=m_pointCloud[minYI].m_y-m_pointCloud[maxYI].m_y;
  dz=m_pointCloud[minYI].m_z-m_pointCloud[maxYI].m_z;
  ngl::Real diam2y=dx*dx+dy*dy+dz*dz;

  dx=m_pointCloud[minZI].m_x-m_pointCloud[maxZI].m_x;
  dy=m_pointCloud[minZI].m_y-m_pointCloud[maxZI].m_y;
  dz=m_pointCloud[minZI].m_z-m_pointCloud[maxZI].m_z;
  ngl::Real diam2z=dx*dx+dy*dy+dz*dz;

  ngl::Real diamTwo=diam2x;
  size_t p1i=minXI;
  size_t p2i=maxXI;
  if(diam2y>diamTwo){ diamTwo=diam2y; p1i=minYI; p2i=maxYI;}
  if(diam2z>diamTwo){ diamTwo=diam2z; p1i=minZI; p2i=maxZI;}
  // now we can get the center of the sphere as the average
  // of the two points
  m_boundingSphereCenter=(m_pointCloud[p1i]+m_pointCloud[p2i])/2.0;
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

  for (auto v : m_pointCloud)
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
        ngl::msg->addWarning(fmt::format("something wrong here error margin {0}",dist2-newRad2));
      }
      m_boundingSphereCenter=newCenter;
      rad=newRad;
      radTwo=rad*rad;
    } // end if dist2>rad2
    m_boundingSphereRadius=rad;

  }

  m_boundingSphereRadius=rad;

}


bool NGLScene::loadPointCloud(const std::string &_name)
{
  namespace ps=pystring;

  std::ifstream in(_name.c_str());
  if (in.is_open() != true)
  {
    ngl::msg->addError(fmt::format(" file {0} not found  ",_name.data()));
    return false;
  }
  m_pointCloud.clear();
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
        m_pointCloud.push_back({x,y,z});
        if(tokens.size()==6)
        {
          float nx=std::stof(tokens[3]);
          float ny=std::stof(tokens[4]);
          float nz=std::stof(tokens[5]);
          m_normalCloud.push_back({nx,ny,nz});
        }
      }
    } // str.size()
    // early out sanity checks!
    if(status == false)
      return false;
  } // while



  in.close();

  std::sort(std::begin(m_pointCloud),std::end(m_pointCloud),
            [](const ngl::Vec3 &a,const ngl::Vec3 &b) {return a.m_x > b.m_x; }
            );
  ngl::msg->addMessage(fmt::format("Point Cloud Loaded {0} points",m_pointCloud.size()));
  return true;


}


void NGLScene::createVAO()
{
  m_pointCloudVAO=ngl::VAOFactory::createVAO(ngl::multiBufferVAO,GL_POINTS);
  m_pointCloudVAO->bind();

  // in this case we are going to set our data as the vertices above
  m_pointCloudVAO->setData(ngl::MultiBufferVAO::VertexData(m_pointCloud.size()*sizeof(ngl::Vec3),m_pointCloud[0].m_x));
  // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
  m_pointCloudVAO->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  if(m_normalCloud.size()>0)
  {
    // in this case we are going to set our data as the vertices above
    m_pointCloudVAO->setData(ngl::MultiBufferVAO::VertexData(m_normalCloud.size()*sizeof(ngl::Vec3),m_normalCloud[0].m_x));
    // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
    m_pointCloudVAO->setVertexAttributePointer(1,3,GL_FLOAT,0,0);
  }
  m_pointCloudVAO->setNumIndices(m_pointCloud.size());

 // now unbind
  m_pointCloudVAO->unbind();

}

void NGLScene::calculateCamera()
{

  ngl::Real maxDim = std::max( m_pointCloudBBox.maxX(), std::max(m_pointCloudBBox.maxY(), m_pointCloudBBox.maxZ()) );
  std::cout<<"Cloud min max"<<m_pointCloudBBox.maxX()<<' '<<m_pointCloudBBox.maxY()<<' '<<m_pointCloudBBox.maxZ()<<'\n';
  ngl::Real cameraZ = std::abs( maxDim / 4.0f * tanf( m_fov * 2.0f ) );
  cameraZ*=2.25f;
  ngl::Vec3 eye{m_boundingBoxCenter.m_x,m_boundingBoxCenter.m_y,cameraZ};

  ngl::Real cameraToFarEdge = ( m_pointCloudBBox.minZ() < 0.0f ) ? -m_pointCloudBBox.minZ() + cameraZ : cameraZ - m_pointCloudBBox.minZ();

  m_far = cameraToFarEdge * 10.0f;
  m_view=ngl::lookAt(eye,m_boundingBoxCenter,{0.0f,1.0f,0.0f});
  m_project=ngl::perspective(m_fov,float(m_win.width)/m_win.height,m_near,m_far);
  std::cout<<"Max Dim "<<maxDim<<" CameraZ "<<cameraZ<<" eye "<<eye<<'\n';
}

bool NGLScene::savePointCloud(const std::string &_fname)
{
  std::ofstream out(_fname.c_str());
  if (out.is_open() != true)
  {
    ngl::msg->addError(fmt::format(" file {0} not found  ",_fname.data()));
    return false;
  }
  for(auto p : m_pointCloud)
    out<<p.m_x<<' '<<p.m_y<<' '<<p.m_z<<'\n';

  out.close();
  return true;
}

void NGLScene::createGridVAO()
{
  m_gridVAO=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_LINES);
  m_gridVAO->bind();
  // so in this case the data is z is up.
  // First we will create the lines along the Y Axis
  // from Ymin -> yMax along the x
  std::vector<ngl::Vec3> verts;
  auto xmin=m_pointCloudBBox.minX();
  auto xmax=m_pointCloudBBox.maxX();
  for(float y=m_pointCloudBBox.minY(); y<m_pointCloudBBox.maxY(); y+=m_gridStepY )
  {
    verts.push_back({xmin,y,m_gridY});
    verts.push_back({xmax,y,m_gridY});
  }


  auto ymin=m_pointCloudBBox.minY();
  auto ymax=m_pointCloudBBox.maxY();
  for(float x=m_pointCloudBBox.minX(); x<m_pointCloudBBox.maxX(); x+=m_gridStepX )
  {
    verts.push_back({x,ymin,m_gridY});
    verts.push_back({x,ymax,m_gridY});
  }

  // in this case we are going to set our data as the vertices above
  m_gridVAO->setData(ngl::SimpleVAO::VertexData(verts.size()*sizeof(ngl::Vec3),verts[0].m_x));
  // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
  m_gridVAO->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_gridVAO->setNumIndices(verts.size());

  m_gridVAO->unbind();
  ngl::msg->addMessage(fmt::format("Num of grid elements {0}",verts.size()));
}

void NGLScene::createPointCloud()
{
  return;
  m_pointCloud.clear();
  m_pointCloud.resize(m_numPoints);
  auto rng=ngl::Random::instance();
  std::vector<ngl::Vec3> verts(40);
    for(auto &v : m_pointCloud)
    {
      v=rng->getRandomPoint(m_cloudExtents.m_x,
                            m_cloudExtents.m_y,
                            m_cloudExtents.m_z);
    }
    m_pointCloudVAO=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_POINTS);
    m_pointCloudVAO->bind();

    // in this case we are going to set our data as the vertices above
    m_pointCloudVAO->setData(ngl::MultiBufferVAO::VertexData(m_pointCloud.size()*sizeof(ngl::Vec3),m_pointCloud[0].m_x));
    // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

    m_pointCloudVAO->setVertexAttributePointer(0,3,GL_FLOAT,0,0);

    m_pointCloudVAO->setNumIndices(m_pointCloud.size());

   // now unbind
    m_pointCloudVAO->unbind();

}

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

  if (m_showBBox)
  {
    shader->setUniform("Colour",0.8f,0.0f,0.0f,1.0f);
    m_pointCloudBBox.draw();
  }

  if (m_showGrid)
  {
    createGridVAO();

    shader->setUniform("Colour",0.8f,0.8f,0.0f,1.0f);
    m_gridVAO->bind();
    m_gridVAO->draw();
    m_gridVAO->unbind();
  }

  if(m_showSphere)
  {
    shader->setUniform("Colour",0.0f,0.9f,0.0f,1.0f);
    ngl::Mat4 translate;
    translate.translate(m_boundingSphereCenter.m_x,
                        m_boundingSphereCenter.m_y,
                        m_boundingSphereCenter.m_z);

    M=m_mouseGlobalTX*translate;
    MV=  m_view*M;
    MVP= m_project*MV;
    shader->setUniform("MVP",MVP);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    ngl::VAOPrimitives::instance()->draw("sphere");
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
  case Qt::Key_Down : m_gridY-=0.1f; break;
  case Qt::Key_Up : m_gridY+=0.1f; break;

  case Qt::Key_B : m_showBBox^=true; break;
  case Qt::Key_S : m_showSphere^=true; break;
  case Qt::Key_V : savePointCloud("data/save.xyz"); break;
  case Qt::Key_Equal : m_pointSize++; break;
  case Qt::Key_Minus : m_pointSize--; break;

  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}
