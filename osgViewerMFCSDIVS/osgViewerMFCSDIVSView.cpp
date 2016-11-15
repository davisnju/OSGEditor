
// osgViewerMFCSDIVSView.cpp : CosgViewerMFCSDIVSView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "osgViewerMFCSDIVS.h"
#endif

#include "osgViewerMFCSDIVSDoc.h"
#include "osgViewerMFCSDIVSView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CosgViewerMFCSDIVSView

IMPLEMENT_DYNCREATE(CosgViewerMFCSDIVSView, CView)

BEGIN_MESSAGE_MAP(CosgViewerMFCSDIVSView, CView)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_CREATE()
    ON_COMMAND(ID_OSG_ADDMD, &CosgViewerMFCSDIVSView::OnOsgAddmd)
    ON_UPDATE_COMMAND_UI(ID_OSG_ADDMD, &CosgViewerMFCSDIVSView::OnUpdateOsgAddmd)
    ON_COMMAND(ID_OSG_ADDEF, &CosgViewerMFCSDIVSView::OnOsgAddef)
    ON_UPDATE_COMMAND_UI(ID_OSG_ADDEF, &CosgViewerMFCSDIVSView::OnUpdateOsgAddef)
    ON_COMMAND(ID_OSG_PAN, &CosgViewerMFCSDIVSView::OnOsgPan)
    ON_UPDATE_COMMAND_UI(ID_OSG_PAN, &CosgViewerMFCSDIVSView::OnUpdateOsgPan)
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
    ON_COMMAND(ID_OSG_ROTATE, &CosgViewerMFCSDIVSView::OnOsgRotate)
    ON_UPDATE_COMMAND_UI(ID_OSG_ROTATE, &CosgViewerMFCSDIVSView::OnUpdateOsgRotate)
    ON_COMMAND(ID_OSG_TRANS, &CosgViewerMFCSDIVSView::OnOsgTrans)
    ON_UPDATE_COMMAND_UI(ID_OSG_TRANS, &CosgViewerMFCSDIVSView::OnUpdateOsgTrans)
END_MESSAGE_MAP()

// CosgViewerMFCSDIVSView 构造/析构

CosgViewerMFCSDIVSView::CosgViewerMFCSDIVSView()
{
    // TODO:  在此处添加构造代码
    mAddModelValid = false;
    mAddEffectValid = false;
    panMode = false;
    naviModeChanged = false;
    mModelRotate = false;
    mModeltrans = false;
}

CosgViewerMFCSDIVSView::~CosgViewerMFCSDIVSView()
{}

BOOL CosgViewerMFCSDIVSView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO:  在此处通过修改
    //  CREATESTRUCT cs 来修改窗口类或样式

    return CView::PreCreateWindow(cs);
}

// CosgViewerMFCSDIVSView 绘制

void CosgViewerMFCSDIVSView::OnDraw(CDC* /*pDC*/)
{
    CosgViewerMFCSDIVSDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO:  在此处为本机数据添加绘制代码
}

void CosgViewerMFCSDIVSView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CosgViewerMFCSDIVSView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    //theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CosgViewerMFCSDIVSView 诊断

#ifdef _DEBUG
void CosgViewerMFCSDIVSView::AssertValid() const
{
    CView::AssertValid();
}

void CosgViewerMFCSDIVSView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CosgViewerMFCSDIVSDoc* CosgViewerMFCSDIVSView::GetDocument() const // 非调试版本是内联的
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CosgViewerMFCSDIVSDoc)));
    return (CosgViewerMFCSDIVSDoc*)m_pDocument;
}
#endif //_DEBUG


// CosgViewerMFCSDIVSView 消息处理程序


int CosgViewerMFCSDIVSView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    mOSG = new cOSG(m_hWnd);

    return 0;
}


void CosgViewerMFCSDIVSView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    mOSG->InitOSG("cow.osg");
    mThreadHandle = (HANDLE)_beginthread(&cOSG::Render, 0, mOSG);

    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    float c1, c2, c3, q1, q2, q3, q4, d;

    Vec3d center = mOSG->m_naviManipulator->getCenter();
    c1 = center.x();
    c2 = center.y();
    c3 = center.z();
    Quat rotate = mOSG->m_naviManipulator->getRotation();
    Vec4d rtasvc4d = rotate.asVec4();
    q1 = rtasvc4d.w();
    q2 = rtasvc4d.x();
    q3 = rtasvc4d.y();
    q4 = rtasvc4d.z();
    d = mOSG->m_naviManipulator->getDistance();
    pFrame->SetManipulatorProperties(c1, c2, c3, q1, q2, q3, q4, d);
}



void CosgViewerMFCSDIVSView::OnOsgAddmd()
{
    mAddModelValid = !mAddModelValid;
    if (mAddModelValid)
    {
        mOSG->addNewModels();
    }
    else
    {
        mOSG->dontAddNewModels();
    }
}


void CosgViewerMFCSDIVSView::OnUpdateOsgAddmd(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(mAddModelValid);
}


void CosgViewerMFCSDIVSView::OnOsgAddef()
{
    mAddEffectValid = !mAddEffectValid;
    if (mAddEffectValid)
    {
        mOSG->addEffects();
    }
    else
    {
        mOSG->dontAddEffects();
    }
}


void CosgViewerMFCSDIVSView::OnUpdateOsgAddef(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(mAddEffectValid);
}


void CosgViewerMFCSDIVSView::OnOsgPan()
{
    panMode = !panMode;
    mOSG->setNaviMode(panMode);
    naviModeChanged = true;
}


void CosgViewerMFCSDIVSView::OnUpdateOsgPan(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(panMode);
}


void CosgViewerMFCSDIVSView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (panMode)
    {
        HCURSOR hCur = LoadCursor(NULL, IDC_HAND);
        ::SetCursor(hCur);
    }
    else
    {
        HCURSOR hCur = LoadCursor(NULL, IDC_ARROW);
        ::SetCursor(hCur);
    }

    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    float c1, c2, c3, q1, q2, q3, q4, d;

    Vec3d center = mOSG->m_naviManipulator->getCenter();
    c1 = center.x();
    c2 = center.y();
    c3 = center.z();
    Quat rotate = mOSG->m_naviManipulator->getRotation();
    Vec4d rtasvc4d = rotate.asVec4();
    q1 = rtasvc4d.w();
    q2 = rtasvc4d.x();
    q3 = rtasvc4d.y();
    q4 = rtasvc4d.z();
    d = mOSG->m_naviManipulator->getDistance();
    pFrame->SetManipulatorProperties(c1, c2, c3, q1, q2, q3, q4, d);

    CView::OnMouseMove(nFlags, point);
}


BOOL CosgViewerMFCSDIVSView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CosgViewerMFCSDIVSView::OnOsgRotate()
{
    mModelRotate = !mModelRotate;
    mOSG->rotateModel(mModelRotate);
}


void CosgViewerMFCSDIVSView::OnUpdateOsgRotate(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(mModelRotate);
}


void CosgViewerMFCSDIVSView::OnOsgTrans()
{
    mModeltrans = !mModeltrans;
    mOSG->transferModel(mModeltrans);
}


void CosgViewerMFCSDIVSView::OnUpdateOsgTrans(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(mModeltrans);
}
