
// osgViewerMFCSDIVS.h : osgViewerMFCSDIVS Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CosgViewerMFCSDIVSApp:
// �йش����ʵ�֣������ osgViewerMFCSDIVS.cpp
//

class CosgViewerMFCSDIVSApp : public CWinAppEx
{
public:
    CosgViewerMFCSDIVSApp();


    // ��д
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // ʵ��
    UINT  m_nAppLook;
    BOOL  m_bHiColorIcons;

    virtual void PreLoadState();
    virtual void LoadCustomState();
    virtual void SaveCustomState();

    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};

extern CosgViewerMFCSDIVSApp theApp;
