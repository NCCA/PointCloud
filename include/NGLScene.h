#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Obj.h>
#include <ngl/MultiBufferVAO.h>


#include "WindowParams.h"
// this must be included after NGL includes else we get a clash with gl libs
#include <QOpenGLWindow>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene(const std::string &_fname);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize the window
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;

private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event ) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event ) override;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event) override;
    /// @brief windows parameters for mouse control etc.
    WinParams m_win;
    /// position for our model
    ngl::Vec3 m_modelPos;
    std::unique_ptr<ngl::AbstractVAO> m_pointCloudVAO;
    std::unique_ptr<ngl::AbstractVAO> m_gridVAO;
    void createPointCloud();
    void createGridVAO();
    void calculateBoundingBox();
    void calculateBoundingSphere();
    void calculateCamera();
    void createVAO();
    void scalePointCloud(const ngl::Vec3 &_s);
    bool savePointCloud(const std::string &_fname);
    bool loadPointCloud(const std::string &_name);
    ngl::BBox m_pointCloudBBox;
    ngl::Real m_boundingSphereRadius;
    ngl::Vec3 m_boundingSphereCenter;
    ngl::Vec3 m_boundingBoxCenter;

    size_t m_numPoints=1<<10;
    ngl::Mat4 m_mouseGlobalTX;
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    ngl::Real m_fov=45.0f;
    ngl::Real m_near=0.1f;
    ngl::Real m_far=1000.0f;
    ngl::Real m_gridStepX=0.5f;
    ngl::Real m_gridStepY=0.5f;

    ngl::Real m_gridY=-1.0f;
    std::vector<ngl::Vec3> m_pointCloud;
    std::vector<ngl::Vec3> m_normalCloud;
    ngl::Vec3 m_cloudExtents{100.0f,100.0f,100.0f};
    bool m_showBBox=true;
    bool m_showSphere=false;
    bool m_showGrid=true;
    int m_pointSize=1;
    std::string m_filename;
};



#endif
