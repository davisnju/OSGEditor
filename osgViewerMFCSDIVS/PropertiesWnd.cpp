
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "osgViewerMFCSDIVSView.h"
#include "osgViewerMFCSDIVS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
    m_nComboHeight = 0;
}

CPropertiesWnd::~CPropertiesWnd()
{}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
    ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
    ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
    ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
    ON_COMMAND(ID_PROPERTIES1, OnProperties1)
    ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
    ON_COMMAND(ID_PROPERTIES2, OnProperties2)
    ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
    ON_WM_SETFOCUS()
    ON_WM_SETTINGCHANGE()
    ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CPropertiesWnd::OnPropertyChanged)
    ON_COMMAND(ID_PROPERTIES3, &CPropertiesWnd::OnProperties3)
    ON_UPDATE_COMMAND_UI(ID_PROPERTIES3, &CPropertiesWnd::OnUpdateProperties3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 消息处理程序

void CPropertiesWnd::AdjustLayout()
{
    if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
    {
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // 创建组合: 
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
    {
        TRACE0("未能创建属性组合 \n");
        return -1;      // 未能创建
    }

    m_wndObjectCombo.AddString(_T("应用程序"));
    m_wndObjectCombo.AddString(_T("属性窗口"));
    m_wndObjectCombo.SetCurSel(0);

    CRect rectCombo;
    m_wndObjectCombo.GetClientRect(&rectCombo);

    m_nComboHeight = rectCombo.Height();

    if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
    {
        TRACE0("未能创建属性网格\n");
        return -1;      // 未能创建
    }

    InitPropList();

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
    m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ?
                        IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() &
                              ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    m_wndToolBar.SetOwner(this);

    // 所有命令将通过此控件路由，而不是通过主框架路由: 
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    AdjustLayout();
    return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
    m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{

}

void CPropertiesWnd::OnSortProperties()
{
    m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CosgViewerMFCSDIVSView* pView = (CosgViewerMFCSDIVSView*)pFrame->GetActiveView();
    pView->mAddModelValid = !pView->mAddModelValid;
    pView->mOSG->addNewModels(pView->mAddModelValid);
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* pCmdUI)
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CosgViewerMFCSDIVSView* pView = (CosgViewerMFCSDIVSView*)pFrame->GetActiveView();
    pCmdUI->SetCheck(pView->mAddModelValid);
}

void CPropertiesWnd::OnProperties2()
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CosgViewerMFCSDIVSView* pView = (CosgViewerMFCSDIVSView*)pFrame->GetActiveView();
    pView->mAddEffectValid = !pView->mAddEffectValid;
    pView->mOSG->addEffects(pView->mAddEffectValid);
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* pCmdUI)
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CosgViewerMFCSDIVSView* pView = (CosgViewerMFCSDIVSView*)pFrame->GetActiveView();
    pCmdUI->SetCheck(pView->mAddEffectValid);
}


void CPropertiesWnd::OnProperties3()
{

    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CosgViewerMFCSDIVSView* pView = (CosgViewerMFCSDIVSView*)pFrame->GetActiveView();
    pView->panMode = !pView->panMode;
    pView->mOSG->setNaviMode(pView->panMode);
}

void CPropertiesWnd::OnUpdateProperties3(CCmdUI *pCmdUI)
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CosgViewerMFCSDIVSView* pView = (CosgViewerMFCSDIVSView*)pFrame->GetActiveView();
    pCmdUI->SetCheck(pView->panMode);
}

LRESULT CPropertiesWnd::OnPropertyChanged(__in WPARAM wparam, __in LPARAM lparam)
{
    CMFCPropertyGridProperty* pProp = reinterpret_cast<CMFCPropertyGridProperty*>(lparam);

    int nID = pProp->GetData();

    if (1 == nID) //modelname
    {
        //自定义的操作
        //CString cpStr(pProp->GetValue().bstrVal);
        //view mOSG handler
        CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
        CosgViewerMFCSDIVSView* pView = (CosgViewerMFCSDIVSView*)pFrame->GetActiveView();
        pView->mOSG->setNewModel(getModelName().GetBuffer());

    }
    else if (2 == nID)
    {
        CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
        CosgViewerMFCSDIVSView* pView = (CosgViewerMFCSDIVSView*)pFrame->GetActiveView();
        pView->mOSG->setModelSize(pProp->GetValue().fltVal);
    }
    else if (3 == nID)
    {
        CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
        CosgViewerMFCSDIVSView* pView = (CosgViewerMFCSDIVSView*)pFrame->GetActiveView();
        pView->mOSG->setFireSize(pProp->GetValue().fltVal);
    }

    return 0;
}

void CPropertiesWnd::InitPropList()
{
    SetPropListFont();

    m_wndPropList.EnableHeaderCtrl(FALSE);
    m_wndPropList.EnableDescriptionArea();
    m_wndPropList.SetVSDotNetLook();
    m_wndPropList.MarkModifiedProperties();

    CMFCPropertyGridProperty* pModelGroup = new CMFCPropertyGridProperty(_T("模型"));

    CMFCPropertyGridProperty* pModelNameProp = new CMFCPropertyGridProperty(_T("模型名称"), _T("cow"), _T("指定待添加模型的名称"), 1);
    pModelNameProp->AddOption(_T("avatar"));
    pModelNameProp->AddOption(_T("bignathan"));
    pModelNameProp->AddOption(_T("cessna"));
    pModelNameProp->AddOption(_T("robot"));
    pModelNameProp->AddOption(_T("cow"));
    pModelNameProp->AllowEdit(FALSE);

    pModelGroup->AddSubItem(pModelNameProp);

    CMFCPropertyGridProperty* pModelSizeProp = new CMFCPropertyGridProperty(_T("模型大小"), 1.0f, _T("指定待添加模型的大小"), 2);
    pModelSizeProp->AllowEdit(TRUE);

    pModelGroup->AddSubItem(pModelSizeProp);


    CMFCPropertyGridProperty* pEffectGroup = new CMFCPropertyGridProperty(
        _T("特效"));
    CMFCPropertyGridProperty* pFireSizeProp = new CMFCPropertyGridProperty(
        _T("火焰大小"), 0.0f, _T("指定待添加火焰的大小\n0表示随机大小(0~1)"), 3);
    pFireSizeProp->AllowEdit(TRUE);

    pEffectGroup->AddSubItem(pFireSizeProp);

    CMFCPropertyGridProperty* pManipulatorGroup =
        new CMFCPropertyGridProperty(_T("中心"));
    CMFCPropertyGridProperty* pManipulatorProp = new CMFCPropertyGridProperty(
        _T("x"), 0.0f, _T("_center v1"), 4);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
        _T("y"), 0.0f, _T("_center v2"), 5);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
        _T("z"), 0.0f, _T("_center v4"), 6);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    m_wndPropList.AddProperty(pModelGroup);			//0
    m_wndPropList.AddProperty(pEffectGroup);		//1
    m_wndPropList.AddProperty(pManipulatorGroup);   //2  _center

    pManipulatorGroup =
        new CMFCPropertyGridProperty(_T("旋转"));
    pManipulatorProp = new CMFCPropertyGridProperty(
        _T("q1"), 0.0f, _T("_rotation v1"), 7);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
        _T("q2"), 0.0f, _T("_rotation v2"), 8);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
        _T("q3"), 0.0f, _T("_rotation v3"), 9);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
        _T("q4"), 0.0f, _T("_rotation v4"), 10);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    m_wndPropList.AddProperty(pManipulatorGroup);//3 _rotation


    pManipulatorGroup =
        new CMFCPropertyGridProperty(_T("距离"));

    pManipulatorProp = new CMFCPropertyGridProperty(
        _T("distance"), 0.0f, _T("_distance"), 10);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    m_wndPropList.AddProperty(pManipulatorGroup);//4 _distance

}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);
    m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CDockablePane::OnSettingChange(uFlags, lpszSection);
    SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
    ::DeleteObject(m_fntPropList.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info;
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);

    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight;
    lf.lfItalic = info.lfMenuFont.lfItalic;

    m_fntPropList.CreateFontIndirect(&lf);

    m_wndPropList.SetFont(&m_fntPropList);
    m_wndObjectCombo.SetFont(&m_fntPropList);
}
