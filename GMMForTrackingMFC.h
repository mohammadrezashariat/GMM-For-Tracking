
// GMMForTrackingMFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGMMForTrackingMFCApp:
// �йش����ʵ�֣������ GMMForTrackingMFC.cpp
//

class CGMMForTrackingMFCApp : public CWinApp
{
public:
	CGMMForTrackingMFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGMMForTrackingMFCApp theApp;