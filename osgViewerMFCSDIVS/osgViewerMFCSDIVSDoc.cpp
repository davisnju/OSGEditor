
// osgViewerMFCSDIVSDoc.cpp : CosgViewerMFCSDIVSDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "osgViewerMFCSDIVS.h"
#endif

#include "osgViewerMFCSDIVSDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CosgViewerMFCSDIVSDoc

IMPLEMENT_DYNCREATE(CosgViewerMFCSDIVSDoc, CDocument)

BEGIN_MESSAGE_MAP(CosgViewerMFCSDIVSDoc, CDocument)
END_MESSAGE_MAP()


// CosgViewerMFCSDIVSDoc ����/����

CosgViewerMFCSDIVSDoc::CosgViewerMFCSDIVSDoc()
{
    // TODO:  �ڴ����һ���Թ������

}

CosgViewerMFCSDIVSDoc::~CosgViewerMFCSDIVSDoc()
{}

BOOL CosgViewerMFCSDIVSDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO:  �ڴ�������³�ʼ������
    // (SDI �ĵ������ø��ĵ�)

    return TRUE;
}




// CosgViewerMFCSDIVSDoc ���л�

void CosgViewerMFCSDIVSDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO:  �ڴ���Ӵ洢����
    }
    else
    {
        // TODO:  �ڴ���Ӽ��ش���
    }
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CosgViewerMFCSDIVSDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
    // �޸Ĵ˴����Ի����ĵ�����
    dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

    CString strText = _T("TODO: implement thumbnail drawing here");
    LOGFONT lf;

    CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
    pDefaultGUIFont->GetLogFont(&lf);
    lf.lfHeight = 36;

    CFont fontDraw;
    fontDraw.CreateFontIndirect(&lf);

    CFont* pOldFont = dc.SelectObject(&fontDraw);
    dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
    dc.SelectObject(pOldFont);
}

// ������������֧��
void CosgViewerMFCSDIVSDoc::InitializeSearchContent()
{
    CString strSearchContent;
    // ���ĵ����������������ݡ�
    // ���ݲ���Ӧ�ɡ�;���ָ�

    // ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
    SetSearchContent(strSearchContent);
}

void CosgViewerMFCSDIVSDoc::SetSearchContent(const CString& value)
{
    if (value.IsEmpty())
    {
        RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
    }
    else
    {
        CMFCFilterChunkValueImpl *pChunk = NULL;
        ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
        if (pChunk != NULL)
        {
            pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
            SetChunkValue(pChunk);
        }
    }
}

#endif // SHARED_HANDLERS

// CosgViewerMFCSDIVSDoc ���

#ifdef _DEBUG
void CosgViewerMFCSDIVSDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CosgViewerMFCSDIVSDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG


// CosgViewerMFCSDIVSDoc ����
