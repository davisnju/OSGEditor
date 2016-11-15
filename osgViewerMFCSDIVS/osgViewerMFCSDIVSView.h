
// osgViewerMFCSDIVSView.h : CosgViewerMFCSDIVSView ��Ľӿ�
//

#pragma once

#include "MFC_OSG.h"
#include "osgViewerMFCSDIVSDoc.h"

class CosgViewerMFCSDIVSView : public CView
{
protected: // �������л�����
    CosgViewerMFCSDIVSView();
    DECLARE_DYNCREATE(CosgViewerMFCSDIVSView)

    // ����
public:
    CosgViewerMFCSDIVSDoc* GetDocument() const;

    //����osg����
    cOSG* mOSG;
    //�߳̾��
    HANDLE mThreadHandle;

    // �����
    bool naviModeChanged;
    bool panMode;

    bool mAddModelValid;
    bool mAddEffectValid;

    bool mModelRotate;
    bool mModeltrans;


    // ����
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

    // ��д
public:
    virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

    // ʵ��
public:
    virtual ~CosgViewerMFCSDIVSView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // osgViewerMFCSDIVSView.cpp �еĵ��԰汾
inline CosgViewerMFCSDIVSDoc* CosgViewerMFCSDIVSView::GetDocument() const
{
    return reinterpret_cast<CosgViewerMFCSDIVSDoc*>(m_pDocument);
}
#endif

