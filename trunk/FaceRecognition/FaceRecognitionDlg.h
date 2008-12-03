// FaceRecognitionDlg.h : ͷ�ļ�
//

#pragma once

#include "CvApp.h"
#include "afxwin.h"
#include "afxcmn.h"


// CFaceRecognitionDlg �Ի���
class CFaceRecognitionDlg : public CDialog
{
// ����
public:
	CFaceRecognitionDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CFaceRecognitionDlg();

// �Ի�������
	enum { IDD = IDD_FACERECOGNITION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CCvApp* m_pCv1;  //�����ڶԻ�����������ͼƬ, ��Ƶ
	CCvApp* m_pCv2;
	CCvApp* m_pCv3;
	CCvApp* m_pCv4;
	void DrawPicToHDC(IplImage *img,UINT ID);  //��ͼƬ�ؼ�����ʾͼƬ
	

public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSave();
//	CString m_path; //��ǰ������ͼƬ, ��Ƶ�ļ���·��
//	afx_msg void OnBnClickedDetect();
	afx_msg void OnBnClickedCam();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedOpenvedio();
//	afx_msg void OnBnClickedSavevedio();
	afx_msg void OnBnClickedSavecam();
//	afx_msg void OnBnClickedPause();
//	afx_msg void OnBnClickedCease();
	CString m_openPath;
	CString m_savePath;
	CString m_videoPath;
	BOOL m_bDetect;
	afx_msg void OnBnClickedRadiodetect();
	afx_msg void OnBnClickedRadiodetect2();
	afx_msg void OnBnClickedCheckdetect();
	CButton m_checkDetect;
	afx_msg void OnBnClickedCheck2();
	CButton m_checkPause;
	afx_msg void OnBnClickedAbout();
	CButton m_checkProfile;
	afx_msg void OnBnClickedCheckprofile();
	CButton m_checkFrontal;
	afx_msg void OnBnClickedCheckfrontal();
	afx_msg void OnBnClickedOpenTrain();
public:
	CSliderCtrl m_slFrameRate;
public:
	afx_msg void OnNMReleasedcaptureSldframerate(NMHDR *pNMHDR, LRESULT *pResult);
};
