#include "stdafx.h"  

#include <osgDB/ReadFile>  
#include <osgViewer/Viewer>  
#include <osg/Node>  
#include <osgFX/Scribe>  
#include <osgGA/GUIEventHandler>  
#include <osgUtil/LineSegmentIntersector>  
#include <osg/MatrixTransform>
#include <osgManipulator/TabBoxDragger>  
#include <osgManipulator/Selection>  
#include <osgManipulator/TrackballDragger>  
#include <osgManipulator/CommandManager> 

using namespace osg;

class CEventHandler :public osgGA::GUIEventHandler
{
public:
    bool addFireValid;
    bool addModelValid;

    bool mModelRotate;
    bool mModelRotating;
    bool mModeltrans;
    bool mModeltransfering;

    CString m_modelname;
    float m_modelsize;
    float m_firescale;

    // Êó±ê×ó¼ü×´Ì¬  
    bool m_bLeftButtonDown;

    // Êó±êÎ»ÖÃ  
    float m_fpushX;
    float m_fpushY;

    CEventHandler()
    {
        m_bLeftButtonDown = false;
        addModelValid = false;
        addFireValid = false;
        mModelRotate = false;
        mModeltrans = false;
        mModelRotating = false;
        mModeltransfering = false;
        m_modelname = "cow";
        m_modelsize = 1.0f;
        m_firescale = 0.0f;
    };
    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
protected:
    bool addFire(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea);
    bool addModel(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea);
    bool rotateSelected(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter & ea);
    bool transferSelected(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter & ea);
};