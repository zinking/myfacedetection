// FaceRecognition.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CFaceRecognitionApp:
// �йش����ʵ�֣������ FaceRecognition.cpp
//

class CFaceRecognitionApp : public CWinApp
{
public:
	CFaceRecognitionApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFaceRecognitionApp theApp;
