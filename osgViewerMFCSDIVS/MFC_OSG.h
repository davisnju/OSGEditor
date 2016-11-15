#pragma once

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include <osgWidget/WindowManager>

// x64
#include <osgViewer/GraphicsWindow>

#include <string>

#include "EventHandler.h"
#include "NaviManipulator.h"

using namespace osg;

class cOSG
{
public:
    CNaviManipulator* m_naviManipulator;

    cOSG(HWND hWnd);
    ~cOSG();

    void InitOSG(std::string filename);
    void InitManipulators(void);
    void InitSceneGraph(void);
    void InitCameraConfig(void);
    void SetupWindow(void);
    void SetupCamera(void);
    void PreFrameUpdate(void);
    void PostFrameUpdate(void);
    void Done(bool value) { mDone = value; }
    bool Done(void) { return mDone; }
    static void Render(void* ptr);

    // osgWidget
    ref_ptr<osgWidget::Box> _InfoLabelBox;
    void _createLabelBox();

    osgViewer::Viewer* getViewer() { return mViewer; }

    void setNaviMode(bool p)
    {
        m_naviManipulator->naviMode = p;
    }

    void addNewModels();
    void addNewModels(bool a);
    void dontAddNewModels();
    void addEffects();
    void addEffects(bool f);
    void dontAddEffects();

    void rotateModel(bool r) {
        m_eventHandler->mModelRotate = r;
    };
    void transferModel(bool t)
    {
        m_eventHandler->mModeltrans = t;
    };


    void setNewModel(CString modelname);
    void setModelSize(float s)
    {
        if (abs(s) > 1000000.0)
        {
            AfxMessageBox(_T("model size too large"));
            return;
        }
        m_eventHandler->m_modelsize = s;
        //CString cstr;
        //cstr.Format(_T("s = %.1f\n"), m_pickHandler->m_modelsize);
        //AfxMessageBox(cstr.GetBuffer());
    };

    void setFireSize(float s)
    {
        if (abs(s) > 1000000.0)
        {
            AfxMessageBox(_T("fire size too large"));
            return;
        }
        m_eventHandler->m_firescale = s;
    };

    ref_ptr<Node> cOSG::createBase(const Vec3& center, float radius);
    ref_ptr<Geode> cOSG::createAxis();

    osgWidget::Box* createSimpleTabs(float winX, float winY);

private:

    CEventHandler* m_eventHandler;
    bool mDone;
    std::string m_ModelName;
    HWND m_hWnd;
    osgViewer::Viewer* mViewer;
    ref_ptr<Group> mRoot;
    ref_ptr<Node> mModel;
    ref_ptr<osgGA::TrackballManipulator> trackball;
    ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator;
};

class CRenderingThread : public OpenThreads::Thread
{
public:
    CRenderingThread(cOSG* ptr);
    virtual ~CRenderingThread();

    virtual void run();

protected:
    cOSG* _ptr;
    bool _done;
};
