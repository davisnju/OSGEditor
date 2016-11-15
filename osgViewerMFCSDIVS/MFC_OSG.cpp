// MFC_OSG.cpp : implementation of the cOSG class
//
#include "stdafx.h"
#include "MFC_OSG.h"

#include <windows.h>

#include <osg/Geometry>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/GUIEventAdapter>  
#include <osgManipulator/TabBoxDragger>  
#include <osgManipulator/Selection>  
#include <osgManipulator/TrackballDragger>  
#include <osgManipulator/CommandManager> 
#include <osg/Camera>


cOSG::cOSG(HWND hWnd) :
m_hWnd(hWnd)
{
    m_eventHandler = new CEventHandler();
}

cOSG::~cOSG()
{
    mViewer->setDone(true);
    Sleep(1000);
    mViewer->stopThreading();

    delete mViewer;
}

void cOSG::InitOSG(std::string modelname)
{
    // Store the name of the model to load
    m_ModelName = modelname;

    // Init different parts of OSG
    InitManipulators();
    InitSceneGraph();
    InitCameraConfig();
    
    // ------------------事件处理------------------
    mViewer->addEventHandler(m_eventHandler);

    RECT rect;
    // Get the current window size
    ::GetWindowRect(m_hWnd, &rect);
    ref_ptr<osgWidget::WindowManager> wm =
        new osgWidget::WindowManager(mViewer, rect.right - rect.left,
        rect.bottom - rect.top, 0xf0000000);
    Camera *camera = wm->createParentOrthoCamera();
    //添加tabs， 代码见下面
    wm->addChild(_InfoLabelBox.get());
    wm->resizeAllWindows();
    mRoot->addChild(camera);

    mRoot->setName("root");

    // ------------------相机操作器------------------
    m_naviManipulator = new CNaviManipulator();
    mViewer->setCameraManipulator(m_naviManipulator);
    m_naviManipulator->setRotation(osg::Quat(0.3, 0., 0., 1.));
}

void cOSG::InitManipulators(void)
{
    // Create a trackball manipulator
    trackball = new osgGA::TrackballManipulator();

    // Create a Manipulator Switcher
    keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

    // Add our trackball manipulator to the switcher
    keyswitchManipulator->addMatrixManipulator('1', "Trackball", trackball.get());

    // Init the switcher to the first manipulator (in this case the only manipulator)
    keyswitchManipulator->selectMatrixManipulator(0);  // Zero based index Value
    
}


void cOSG::InitSceneGraph(void)
{
    // Init the main Root Node/Group
    mRoot = new Group;

    // Load the Model from the model name
    mModel = osgDB::readNodeFile(m_ModelName);
    if (!mModel) return;

    // Optimize the model
    osgUtil::Optimizer optimizer;
    optimizer.optimize(mModel.get());
    optimizer.reset();

    Vec3 center(0.0f, 0.0f, 0.0f);
    float radius = 50.0f;
    float baseHeight = 0.0f;
    ref_ptr<Node> baseModel = createBase(Vec3(center.x(), center.y(), baseHeight), radius);
    baseModel->setName("BASE");
    ref_ptr<Node> axixModel = createAxis();
    axixModel->setName("AXIS");
    // Add the model to the scene
    MatrixTransform* sceneMT = new MatrixTransform;
    sceneMT->setMatrix(Matrix::rotate(inDegrees(30.0f), 1.0f, 0.0f, -1.0f));
    sceneMT->addChild(mModel.get());
    sceneMT->addChild(baseModel.get());

    osgUtil::Optimizer optimzer;
    optimzer.optimize(sceneMT);

    mRoot->addChild(baseModel.get());
    mRoot->addChild(axixModel.get());

    mRoot->addChild(mModel);

    //selection
    //ref_ptr<osgManipulator::Selection> selection = new osgManipulator::Selection;
    //selection->addChild(mModel);//将需要操控的场景对象添加到Selection下  
    //float scale = mModel->getBound().radius()*1.6;//Dragger的大小是依照物体的外包围球来计算，获取半径，然后稍微放大一点，在将这个值传给Dragger,
    ////设置TrackballDragger  
    //osgManipulator::TrackballDragger* dragger = new osgManipulator::TrackballDragger();
    //dragger->setupDefaultGeometry();
    //dragger->setMatrix(Matrix::scale(scale*0.5, scale*0.5, scale*0.5)*Matrix::translate(mModel->getBound().center()));
    //dragger->addTransformUpdating(selection);
    //dragger->setHandleEvents(true);
    ////设置了这个dragger的启动热键，当前为Ctrl，当按住Ctrl时，单击圆环，方能旋转。 
    //dragger->setActivationModKeyMask(osgGA::GUIEventAdapter::MODKEY_CTRL);

    //mRoot->addChild(selection);
    //mRoot->addChild(dragger);
}

void cOSG::InitCameraConfig(void)
{
    // Local Variable to hold window size data
    RECT rect;

    // Create the viewer for this window
    mViewer = new osgViewer::Viewer();

    // Add a Stats Handler to the viewer
    mViewer->addEventHandler(new osgViewer::StatsHandler);

    mViewer->setCameraManipulator(NULL);

    // Get the current window size
    ::GetWindowRect(m_hWnd, &rect);

    // Init the GraphicsContext Traits
    ref_ptr<GraphicsContext::Traits> traits = new GraphicsContext::Traits;

    // Init the Windata Variable that holds the handle for the Window to display OSG in.
    ref_ptr<Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);
    // x64
    // ref_ptr<Referenced> windata = new osgViewer::GraphicsWindow::
    // Setup the traits parameters
    traits->x = 0;
    traits->y = 0;
    traits->width = rect.right - rect.left;
    traits->height = rect.bottom - rect.top;
    traits->windowDecoration = false;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
    traits->setInheritedWindowPixelFormat = true;
    traits->inheritedWindowData = windata;

    // Create the Graphics Context
    GraphicsContext* gc = GraphicsContext::createGraphicsContext(traits.get());

    // Init Master Camera for this View
    ref_ptr<Camera> camera = mViewer->getCamera();

    // Assign Graphics Context to the Camera
    camera->setGraphicsContext(gc);

    // Set the viewport for the Camera
    camera->setViewport(new Viewport(0, 0, traits->width, traits->height));
    Viewport* cv = camera->getViewport();

    // Set projection matrix and camera attribtues
    camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    camera->setClearColor(Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
    /*camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0, 1000.0);
        */

    // Add the Camera to the Viewer
    // mViewer->addSlave(camera.get());
    mViewer->setCamera(camera.get());

    // Add the Camera Manipulator to the Viewer
    // mViewer->setCameraManipulator(keyswitchManipulator);

    // Set the Scene Data
    mViewer->setSceneData(mRoot.get());

    // Realize the Viewer
    mViewer->realize();

    // Correct aspect ratio
    // double fovy,aspectRatio,z1,z2;
    // mViewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);
    // aspectRatio=double(traits->width)/double(traits->height);
    // mViewer->getCamera()->setProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);

}

void cOSG::PreFrameUpdate()
{
    // osg::Matrix trans;
    // trans.makeTranslate(0., 0., -12.);
    // Matrix rot;
    // angle += 0.1;
    // rot.makeRotate(angle, Vec3(1., 0., 0.));
    // 设置视口矩阵（旋转矩阵和平移矩阵连乘）。
    // mViewer->getCamera()->setViewMatrix(rot * trans);
}

void cOSG::PostFrameUpdate()
{}

void cOSG::Render(void* ptr)
{
    cOSG* osg = (cOSG*)ptr;

    osgViewer::Viewer* viewer = osg->getViewer();

    // You have two options for the main viewer loop
    //      viewer->run()   or
    //      while(!viewer->done()) { viewer->frame(); }

    //viewer->run();
    while (!viewer->done())
    {
        osg->PreFrameUpdate();
        viewer->frame();
        osg->PostFrameUpdate();
        //Sleep(10);         // Use this command if you need to allow other processes to have cpu time
    }

    // For some reason this has to be here to avoid issue:
    // if you have multiple OSG windows up
    // and you exit one then all stop rendering
    AfxMessageBox(_T("Exit Rendering Thread"));

    _endthread();
}

void cOSG::addNewModels(bool a)
{
    m_eventHandler->addModelValid = a;
}

void cOSG::addNewModels()
{
    m_eventHandler->addModelValid = true;
}

void cOSG::addEffects(bool f)
{
    m_eventHandler->addFireValid = f;
}
void cOSG::addEffects()
{
    m_eventHandler->addFireValid = true;
}

void cOSG::dontAddNewModels()
{
    m_eventHandler->addModelValid = false;
}
void cOSG::dontAddEffects()
{
    m_eventHandler->addFireValid = false;
}

void cOSG::setNewModel(CString modelname)
{
    m_eventHandler->m_modelname = modelname;
    //AfxMessageBox(modelname.GetBuffer());
}

void cOSG::_createLabelBox()
{
    RECT rect;
    // Get the current window size
    ::GetWindowRect(m_hWnd, &rect);
    float width = rect.right - rect.left,
        height = rect.bottom - rect.top;
    osgWidget::Label* label = new osgWidget::Label("Infolabel");
    label->setFont("fonts/Vera.ttf");
    label->setFontSize(16.);
    label->setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
    label->setAlignHorizontal(osgWidget::Widget::HA_CENTER);
    label->setPadding(0.0f);
    label->setWidth(width);
    label->setHeight(45.0f);
    label->setLabel("aX:0.0 aZ:0.0 pos:0.0 0.0 0.0");
    _InfoLabelBox = new osgWidget::Box("InfoLabelBox", osgWidget::Box::VERTICAL);
    _InfoLabelBox->setAnchorHorizontal(osgWidget::Window::HA_LEFT);
    _InfoLabelBox->setOrigin(0.0f, height - 60.0f);
    _InfoLabelBox->setVisibilityMode(osgWidget::Window::VM_FULL);
    _InfoLabelBox->addWidget(label);
    _InfoLabelBox->getBackground()->setColor(0.0f, 0.0f, 0.0f, 0.0f);

}

CRenderingThread::CRenderingThread(cOSG* ptr)
: OpenThreads::Thread(), _ptr(ptr), _done(false)
{}

CRenderingThread::~CRenderingThread()
{
    _done = true;
    if (isRunning())
    {
        cancel();
        join();
    }
}

void CRenderingThread::run()
{
    if (!_ptr)
    {
        _done = true;
        return;
    }

    osgViewer::Viewer* viewer = _ptr->getViewer();
    do
    {
        _ptr->PreFrameUpdate();
        viewer->frame();
        _ptr->PostFrameUpdate();
    } while (!testCancel() && !viewer->done() && !_done);
}



ref_ptr<Node> cOSG::createBase(const Vec3& center, float radius)
{

    int numTilesX = 10;
    int numTilesY = 10;

    float width = 2 * radius;
    float height = 2 * radius;

    Vec3 v000(center - Vec3(width*0.5f, height*0.5f, 0.0f));
    Vec3 dx(Vec3(width / ((float)numTilesX), 0.0, 0.0f));
    Vec3 dy(Vec3(0.0f, height / ((float)numTilesY), 0.0f));

    // fill in vertices for grid, note numTilesX+1 * numTilesY+1...
    Vec3Array* coords = new Vec3Array;
    int iy;
    for (iy = 0; iy <= numTilesY; ++iy)
    {
        for (int ix = 0; ix <= numTilesX; ++ix)
        {
            coords->push_back(v000 + dx*(float)ix + dy*(float)iy);
        }
    }

    //Just two colours - black and white.
    Vec4Array* colors = new Vec4Array;
    colors->push_back(Vec4(1.0f, 1.0f, 1.0f, 1.0f)); // white
    colors->push_back(Vec4(0.0f, 1.0f, 0.0f, 1.0f)); // black

    ref_ptr<DrawElementsUShort> whitePrimitives = new DrawElementsUShort(GL_QUADS);
    ref_ptr<DrawElementsUShort> blackPrimitives = new DrawElementsUShort(GL_QUADS);

    int numIndicesPerRow = numTilesX + 1;
    for (iy = 0; iy < numTilesY; ++iy)
    {
        for (int ix = 0; ix < numTilesX; ++ix)
        {
            DrawElementsUShort* primitives = ((iy + ix) % 2 == 0) ? whitePrimitives.get() : blackPrimitives.get();
            primitives->push_back(ix + (iy + 1)*numIndicesPerRow);
            primitives->push_back(ix + iy*numIndicesPerRow);
            primitives->push_back((ix + 1) + iy*numIndicesPerRow);
            primitives->push_back((ix + 1) + (iy + 1)*numIndicesPerRow);
        }
    }

    // set up a single normal
    Vec3Array* normals = new Vec3Array;
    normals->push_back(Vec3(0.0f, 0.0f, 1.0f));

    Geometry* geom = new Geometry;
    geom->setVertexArray(coords);

    geom->setColorArray(colors, Array::BIND_PER_PRIMITIVE_SET);

    geom->setNormalArray(normals, Array::BIND_OVERALL);

    geom->addPrimitiveSet(whitePrimitives.get());
    geom->addPrimitiveSet(blackPrimitives.get());

    ref_ptr<Geode> geode = new Geode;
    geode->addDrawable(geom);

    return geode;
}
ref_ptr<Geode> cOSG::createAxis()
{
    ref_ptr<Geode> geode(new Geode());
    ref_ptr<Geometry> geometry(new Geometry());

    ref_ptr<Vec3Array> vertices(new Vec3Array());
    vertices->push_back(Vec3(0.0, 0.0, 0.0));
    vertices->push_back(Vec3(1.0, 0.0, 0.0));
    vertices->push_back(Vec3(0.0, 0.0, 0.0));
    vertices->push_back(Vec3(0.0, 2.0, 0.0));
    vertices->push_back(Vec3(0.0, 0.0, 0.0));
    vertices->push_back(Vec3(0.0, 0.0, 3.0));
    geometry->setVertexArray(vertices.get());

    ref_ptr<Vec4Array> colors(new Vec4Array());
    colors->push_back(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    colors->push_back(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    geometry->setColorArray(colors.get(), Array::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(new DrawArrays(PrimitiveSet::LINES, 0, 6));

    geode->addDrawable(geometry.get());
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, false);
    return geode;
}