
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

class CMainFrame : public CFrameWndEx
{

protected: // �������л�����
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

    // ����
public:

    // ����
public:

    // ��д
public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

    void SetManipulatorProperties(const float c1, const float c2, const float c3,
                                  const float q1, const float q2, const float q3, const float q4,
                                  const float d
                                  )
    {
        // _center
        m_wndProperties.m_wndPropList.GetProperty(2)->GetSubItem(0)->SetValue(c1);
        m_wndProperties.m_wndPropList.GetProperty(2)->GetSubItem(1)->SetValue(c2);
        m_wndProperties.m_wndPropList.GetProperty(2)->GetSubItem(2)->SetValue(c3);
        //_rotation
        m_wndProperties.m_wndPropList.GetProperty(3)->GetSubItem(0)->SetValue(q1);
        m_wndProperties.m_wndPropList.GetProperty(3)->GetSubItem(1)->SetValue(q2);
        m_wndProperties.m_wndPropList.GetProperty(3)->GetSubItem(2)->SetValue(q3);
        m_wndProperties.m_wndPropList.GetProperty(3)->GetSubItem(3)->SetValue(q4);
        //_distance
        m_wndProperties.m_wndPropList.GetProperty(4)->GetSubItem(0)->SetValue(d);
    };

    // ʵ��
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
    CMFCMenuBar       m_wndMenuBar;
    CMFCToolBar       m_wndToolBar;
    CMFCStatusBar     m_wndStatusBar;
    CMFCToolBarImages m_UserImages;
    CFileView         m_wndFileView;
    CClassView        m_wndClassView;
    COutputWnd        m_wndOutput;
    CPropertiesWnd    m_wndProperties;

    // ���ɵ���Ϣӳ�亯��
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnViewCustomize();
    afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
    afx_msg void OnApplicationLook(UINT id);
    afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    DECLARE_MESSAGE_MAP()

    BOOL CreateDockingWindows();
    void SetDockingWindowIcons(BOOL bHiColorIcons);
};


