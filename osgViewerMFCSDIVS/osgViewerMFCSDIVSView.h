
// osgViewerMFCSDIVSView.h : CosgViewerMFCSDIVSView 类的接口
//

#pragma once

#include "MFC_OSG.h"
#include "osgViewerMFCSDIVSDoc.h"

class CosgViewerMFCSDIVSView : public CView
{
protected: // 仅从序列化创建
    CosgViewerMFCSDIVSView();
    DECLARE_DYNCREATE(CosgViewerMFCSDIVSView)

    // 特性
public:
    CosgViewerMFCSDIVSDoc* GetDocument() const;

    //核心osg对象
    cOSG* mOSG;
    //线程句柄
    HANDLE mThreadHandle;

    // 各项工具
    bool naviModeChanged;
    bool panMode;

    bool mAddModelValid;
    bool mAddEffectValid;

    bool mModelRotate;
    bool mModeltrans;


    // 操作
public:
    CString getProjectionMatrixAsPerspective()
    {
        CString cstr;
        double fovy, aspectRatio, z1, z2;
        mOSG->getViewer()->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);
        //mOSG->getViewer()->getCamera()->setProjectionMatrixAsPerspective(fovy, abs(aspectRatio), z1, z2);
        cstr.Format(_T("%f, %f, %f, %f"), fovy, aspectRatio, z1, z2);
        return cstr;
    }

    void setNewModel(CString modelname)
    {
        mOSG->setNewModel(modelname);
    };

    // 重写
public:
    virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

    // 实现
public:
    virtual ~CosgViewerMFCSDIVSView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // 生成的消息映射函数
protected:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual void OnInitialUpdate();
    afx_msg void OnOsgAddmd();
    afx_msg void OnUpdateOsgAddmd(CCmdUI *pCmdUI);
    afx_msg void OnOsgAddef();
    afx_msg void OnUpdateOsgAddef(CCmdUI *pCmdUI);
    afx_msg void OnOsgPan();
    afx_msg void OnUpdateOsgPan(CCmdUI *pCmdUI);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnOsgRotate();
    afx_msg void OnUpdateOsgRotate(CCmdUI *pCmdUI);
    afx_msg void OnOsgTrans();
    afx_msg void OnUpdateOsgTrans(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // osgViewerMFCSDIVSView.cpp 中的调试版本
inline CosgViewerMFCSDIVSDoc* CosgViewerMFCSDIVSView::GetDocument() const
{
    return reinterpret_cast<CosgViewerMFCSDIVSDoc*>(m_pDocument);
}
#endif

