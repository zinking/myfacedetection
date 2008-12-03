// FaceRecognitionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FaceRecognition.h"
#include "FaceRecognitionDlg.h"
#include "facerecognitiondlg.h"

#include "CvApp.h"
#include "cv.h"
#include "highgui.h"

#include <stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int FRAMEPERSECOND= 15;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFaceRecognitionDlg �Ի���



CFaceRecognitionDlg::CFaceRecognitionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFaceRecognitionDlg::IDD, pParent)
//	, m_path(_T(""))
	, m_openPath(_T(""))
	, m_savePath(_T(""))
	, m_videoPath(_T(""))
	, m_bDetect(FALSE)
{
	m_pCv1= NULL;
	m_pCv2= NULL;
	m_pCv3= NULL;
	m_pCv4= NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CFaceRecognitionDlg::~CFaceRecognitionDlg()
{
	if(m_pCv1 != NULL)
		delete m_pCv1;
	if(m_pCv2 != NULL)
	    delete m_pCv2;
	if(m_pCv3 != NULL)
	    delete m_pCv3;
	if(m_pCv4 != NULL)
	    delete m_pCv4;

}

void CFaceRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_OPENPATH, m_openPath);
	DDX_Text(pDX, IDC_SAVEPATH, m_savePath);
	DDX_Text(pDX, IDC_VIDEOPATH, m_videoPath);
	//	DDX_Radio(pDX, IDC_RADIODETECT, m_bDetect);
	DDX_Control(pDX, IDC_CHECKDETECT, m_checkDetect);
	DDX_Control(pDX, IDC_CHECK2, m_checkPause);
	DDX_Control(pDX, IDC_CHECKPROFILE, m_checkProfile);
	DDX_Control(pDX, IDC_CHECKFRONTAL, m_checkFrontal);
	DDX_Control(pDX, IDC_SLIDER1, m_slFrameRate);
}

BEGIN_MESSAGE_MAP(CFaceRecognitionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE, OnBnClickedSave)
//	ON_BN_CLICKED(IDC_DETECT, OnBnClickedDetect)
	ON_BN_CLICKED(IDC_CAM, OnBnClickedCam)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPENVEDIO, OnBnClickedOpenvedio)
//	ON_BN_CLICKED(IDC_SAVEVEDIO, OnBnClickedSavevedio)
	ON_BN_CLICKED(IDC_SAVECAM, OnBnClickedSavecam)
//	ON_BN_CLICKED(IDC_PAUSE, OnBnClickedPause)
//	ON_BN_CLICKED(IDC_CEASE, OnBnClickedCease)
	ON_BN_CLICKED(IDC_RADIODETECT, OnBnClickedRadiodetect)
	ON_BN_CLICKED(IDC_RADIODETECT2, OnBnClickedRadiodetect2)
	ON_BN_CLICKED(IDC_CHECKDETECT, OnBnClickedCheckdetect)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_About, OnBnClickedAbout)
	ON_BN_CLICKED(IDC_CHECKPROFILE, OnBnClickedCheckprofile)
	ON_BN_CLICKED(IDC_CHECKFRONTAL, OnBnClickedCheckfrontal)
	ON_BN_CLICKED(IDC_OPEN_TRAIN, &CFaceRecognitionDlg::OnBnClickedOpenTrain)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLDFRAMERATE, &CFaceRecognitionDlg::OnNMReleasedcaptureSldframerate)
END_MESSAGE_MAP()


// CFaceRecognitionDlg ��Ϣ�������

BOOL CFaceRecognitionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
//	m_checkDetect.SetCheck(0); //��ѡ������δѡ��
//	m_checkPause.SetCheck(0);
	m_checkDetect.EnableWindow(FALSE);
	m_checkPause.EnableWindow(FALSE);
	m_checkProfile.EnableWindow(FALSE);
	m_checkFrontal.EnableWindow(FALSE);
	
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CFaceRecognitionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFaceRecognitionDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CFaceRecognitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ͼƬ�ļ�
void CFaceRecognitionDlg::OnBnClickedOpen()
{
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE, _T("*.bmp"), "",
		OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY, 
		"Image Files (*.jpg; *.bmp)|*.jpg; *.bmp|All Files (*.*)|*.*||", NULL);

	//���ô򿪶Ի���ı���
	char title[]= {"��ͼƬ"};
	fileDlg.m_pOFN->lpstrTitle= title;

	if(fileDlg.DoModal()==IDOK)
	{
		CString pathName= fileDlg.GetPathName();
		m_openPath= pathName;
		UpdateData(FALSE);
	    m_pCv1=new CCvApp(pathName, 0);
	
	//	m_pCv1->display(pathName);
		cvvNamedWindow(pathName, 1);
	
	   //��ʾͼƬ
	   IplImage* pImg= m_pCv1->getImage();
	   if(pImg)
	   {
		   if(m_bDetect)
			   cvvShowImage(pathName, m_pCv1->detectFace(pImg));
		   else
	   		  cvvShowImage(pathName, pImg);
		}
	//	m_path= pathName;
		

		//display picture in picture control
	//	DrawPicToHDC(m_pCv->getImage(), IDC_PICTURE);

	}
	

}

//����Ƶ�ļ�
void CFaceRecognitionDlg::OnBnClickedOpenvedio()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_slFrameRate.SetRange(1, 24 );
    UpdateData(TRUE);

	CFileDialog fileDlg(TRUE, _T("*.avi"), "",
		OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY, 
		"Vedio Files (*.avi)|*.avi|All Files (*.*)|*.*||", NULL);

	//���ô򿪶Ի���ı���
	char title[]= {"����Ƶ"};
	fileDlg.m_pOFN->lpstrTitle= title;

	if(fileDlg.DoModal()==IDOK)
	{
	//	m_checkProfile.EnableWindow(TRUE);

		CString pathName= fileDlg.GetPathName();
		m_videoPath= pathName;
		UpdateData(FALSE);
	    m_pCv2=new CCvApp(m_videoPath, 1);
		if(!m_pCv2)
		{
			MessageBox("Memeory Allocated Failure !");
			return;
		}
		if(!m_pCv2->getCapture())
			return;

		cvNamedWindow(m_videoPath, 1);
	
		CvCapture* capture= m_pCv2->getCapture();
		if( capture)
		{
			// Images to capture the frame from vedio file 
			IplImage* frame = NULL;
			IplImage* frame_copy = NULL;
			IplImage* detect_frame= NULL;
		
			int numFrame= 0;
			int index= 0;
		    // Capture from the vedio file.
			for(;;)
			{
				if( !cvGrabFrame( capture )) 	break;
				frame = cvRetrieveFrame( capture );
				if( !frame ) break;
           
				if( !frame_copy ) 
					frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );
				// Check the origin of image. If top left, copy the image frame to frame_copy. 
				if( frame->origin == IPL_ORIGIN_TL ) cvCopy( frame, frame_copy, 0 );
				// Else flip and copy the image
				else cvFlip( frame, frame_copy, 0 );
				
				//suppose there is no face in the current frame
				m_pCv2->exit= FALSE;
				
				//do not draw the face
				cvShowImage(m_videoPath, frame_copy);
				if(!m_bDetect) 	cvShowImage(m_videoPath, frame_copy);

				//draw the face
				else
				{
					//capture only one frame from the continuous 6 frames
				    numFrame++;
					UpdateData(TRUE);
					FRAMEPERSECOND = m_slFrameRate.GetPos();
				    if( numFrame <  FRAMEPERSECOND )	continue;
				    else 	numFrame= 0;

                    detect_frame= m_pCv2->detectFace(frame_copy);
					cvShowImage(m_videoPath, detect_frame);

					IplImage* save_frame=NULL;
				
					if(m_pCv2->exit)
					{
						index++;
	                    char* str=new char[50];
	                    itoa(index,str,10);
						CString type= ".jpg";
						CString picName=(CString)str+type;
					    
						save_frame = cvCreateImage( cvSize(detect_frame->width,detect_frame->height),
							IPL_DEPTH_8U, detect_frame->nChannels );
						cvCopy(detect_frame, save_frame, NULL);
	                    //cvSaveImage(picName,save_frame);
					
					}
				//	delete str;
				}
				
			    // Wait for a while before proceeding to the next frame
				if( cvWaitKey( 30 ) >= 0 )
				{
				//	m_checkProfile.EnableWindow(FALSE);
					break;
				}

			}
			cvReleaseImage( &frame_copy );
			cvReleaseCapture(&capture);
		}
	}
	
}

//����ͼƬ�ļ�
void CFaceRecognitionDlg::OnBnClickedSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//ע�������ļ���ʱҪ���Ϻ�׺��, ���������쳣

	CFileDialog fileDlg(FALSE, _T("*.jpg"), 0, 
		     OFN_HIDEREADONLY|OFN_EXPLORER|OFN_OVERWRITEPROMPT, 
		     "Image Files (*.jpg; *.bmp)|*.jpg;*.bmp||", NULL);

	char title[]= {"����ͼƬ"};
	fileDlg.m_pOFN->lpstrTitle=title;

	if(!m_pCv1)
	{
		MessageBox("Current Image is NULL!");
		return;
	}

	IplImage* pImg= m_pCv1->getImage();

    if(!pImg)
	{
		MessageBox("Current Image is NULL!");
		return;
	}
	else
	{
		if(fileDlg.DoModal()==IDOK)
		{
			CString pathName=fileDlg.GetPathName();
			m_savePath= pathName;
            if(m_bDetect)
				cvSaveImage(m_savePath, m_pCv1->detectFace(pImg));
			else
				cvSaveImage(m_savePath,pImg);
		}
	}
	UpdateData(FALSE);
	
}

//������Ƶ�ļ�
/*void CFaceRecognitionDlg::OnBnClickedSavevedio()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(FALSE, _T("*.avi"), 0, 
		     OFN_HIDEREADONLY|OFN_EXPLORER|OFN_OVERWRITEPROMPT, 
		     "Video Files (*.avi)|*.avi||", NULL);

	char title[]= {"��Ƶ���Ϊ"};
	fileDlg.m_pOFN->lpstrTitle=title;

	if(fileDlg.DoModal()==IDOK)
    {
		CString pathName=fileDlg.GetPathName();

		if(m_pCv->getCapture()!= NULL)
		{
			CvCapture* capture=m_pCv->getCapture();
			IplImage* frame,*frame_copy = 0;
			frame= cvQueryFrame(capture);

			CvVideoWriter* videoWriter;
			videoWriter= cvCreateVideoWriter(pathName, -1, 2.50, cvSize(frame->width, frame->height), 1);

			// Capture from the video file.
			for(;;)
			{
				// Capture the frame and load it in IplImage
				if( !cvGrabFrame( capture ))
					break;
				frame = cvRetrieveFrame( capture );

				// If the frame does not exist, quit the loop
				if( !frame )
					break;
            
				// Allocate framecopy as the same size of the frame
				if( !frame_copy )
					frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );

				// Check the origin of image. If top left, copy the image frame to frame_copy. 
				if( frame->origin == IPL_ORIGIN_TL )
					cvCopy( frame, frame_copy, 0 );
				// Else flip and copy the image
				else
					cvFlip( frame, frame_copy, 0 );

				//д��һ֡����Ƶ�ļ�
				cvWriteFrame(videoWriter, m_pCv->detectFace(frame_copy));
				
			    // Wait for a while before proceeding to the next frame
				if( cvWaitKey( 10 ) >= 0 )
					break;
			}
			cvReleaseImage( &frame_copy );
			cvReleaseVideoWriter(&videoWriter);
		}
			
		}
	
}*/


//�˳�����
void CFaceRecognitionDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	if(m_pCv != NULL)
//		delete m_pCv;
	cvDestroyAllWindows();
	OnOK();
}



/*//�������
void CFaceRecognitionDlg::OnBnClickedDetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pCv != NULL )
	{
		IplImage* pImg;
		IplImage* pDrawImg;
		pImg= m_pCv->getImage();
		pDrawImg= m_pCv->detectFace( pImg );
		if( pImg != NULL)
			cvShowImage(m_openPath, &pDrawImg);

		CvCapture* capture;
		capture= m_pCv->getCapture();
		if( capture != NULL)
			m_pCv->detect= TRUE;
	}
	else
		MessageBox("Image/CAM Loading Failure!");

   
	
}*/


//������ͷ
void CFaceRecognitionDlg::OnBnClickedCam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	m_pCv3= new CCvApp("",2);
	if(m_pCv3 && m_pCv3->getCamCapture())
	{
		m_checkDetect.EnableWindow(TRUE);
	    m_checkPause.EnableWindow(TRUE);
		m_pCv3->display("");
		
		CString fileName="CAM";
		CvCapture* camcapture= m_pCv3->getCamCapture();

	//��ʾ����
	cvvNamedWindow(fileName, 1);
	
	//��ʾ��Ƶ
	if( camcapture)
	{
		// Images to capture the frame from vedio file 
		IplImage* frame = NULL;
		IplImage* frame_copy = NULL;
		IplImage* cur_frame= NULL;

		
		// Capture from the vedio file.
		for(;;)
		{
			// Capture the frame and load it in IplImage
			if( !cvGrabFrame( camcapture ))
				break;
			frame = cvRetrieveFrame( camcapture );

			// If the frame does not exist, quit the loop
			if( !frame )
				break;

			// Allocate framecopy as the same size of the frame
			if( !frame_copy )
				frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );
			// Check the origin of image. If top left, copy the image frame to frame_copy. 
			if( frame->origin == IPL_ORIGIN_TL )
				cvCopy( frame, frame_copy, 0 );
			// Else flip and copy the image
			else
				cvFlip( frame, frame_copy, 0 );

			//display 
			//cvNamedWindow(fileName, 1);
			//do not draw the face
			
			if(!m_pCv3->pause)
			{
			if(!m_pCv3->detect)
				cvShowImage(fileName, frame_copy);

			//draw the face
			else
				cvShowImage(fileName, m_pCv3->detectFace(frame_copy));

			cur_frame=cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );

			cvCopy(frame_copy, cur_frame, NULL);
			}
			else
			{
				cvShowImage(fileName, cur_frame);
				cvWaitKey( 10 );
				continue;
			}
 

				
		    // Wait for a while before proceeding to the next frame
			if( cvWaitKey( 10 ) >= 0 )
			{
				delete m_pCv3;
	            m_pCv3= NULL;
				m_checkDetect.EnableWindow(FALSE);
	            m_checkPause.EnableWindow(FALSE);
				break; 
			}

	/*		//pause capturing frames 
			if( pause )
				cvShowImage(fileName, frame_copy);
			else
				continue;*/
		}
		cvReleaseImage( &frame_copy );
		cvReleaseCapture(&camcapture);
	}

	   
	}

}

//������ͷ������Ƶ(��������)
void CFaceRecognitionDlg::OnBnClickedSavecam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	CFileDialog fileDlg(FALSE, _T("*.avi"), 0, 
		     OFN_HIDEREADONLY|OFN_EXPLORER|OFN_OVERWRITEPROMPT, 
		     "Video Files (*.avi)|*.avi||", NULL);

	m_pCv4= new CCvApp("", 2);
	if(!m_pCv4)
	{
		MessageBox("Memeory Allocated Failure!");
		return;
	}
	//capturing cam failure
	if(m_pCv4->getCamCapture() == NULL)
		return;
	
	m_checkDetect.EnableWindow(TRUE);
	m_checkPause.EnableWindow(TRUE);

	char title[]= {"Save Video as"};
	fileDlg.m_pOFN->lpstrTitle=title;

	if(fileDlg.DoModal()==IDOK)
    {
		CString pathName=fileDlg.GetPathName();

		if(m_pCv4->getCamCapture()!= NULL)
		{
			CvCapture* capture=m_pCv4->getCamCapture();
			IplImage* frame,*frame_copy = 0;
			IplImage* cur_frame= NULL;
			frame= cvQueryFrame(capture);

			CvVideoWriter* videoWriter;
			videoWriter= cvCreateVideoWriter(pathName, -1, 2.50, cvSize(frame->width, frame->height), 1);

			// Capture from the video file.
			for(;;)
			{
				// Capture the frame and load it in IplImage
				if( !cvGrabFrame( capture ))
					break;
				frame = cvRetrieveFrame( capture );

				// If the frame does not exist, quit the loop
				if( !frame )
					break;
            
				// Allocate framecopy as the same size of the frame
				if( !frame_copy )
					frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );

				// Check the origin of image. If top left, copy the image frame to frame_copy. 
				if( frame->origin == IPL_ORIGIN_TL )
					cvCopy( frame, frame_copy, 0 );
				// Else flip and copy the image
				else
					cvFlip( frame, frame_copy, 0 );

				//д��һ֡����Ƶ�ļ�
				if(! m_pCv4->pause )
				{
				cvNamedWindow("Capturing...", 1);
				
				if(m_pCv4->detect)
				{
					IplImage* pImg=m_pCv4->detectFace(frame_copy);
					cvShowImage("Capturing...",pImg);
					cvWriteFrame(videoWriter, pImg);
				}
				else
				{
					cvShowImage("Capturing...",frame_copy);
                    cvWriteFrame(videoWriter, frame_copy);
				}

				cur_frame=cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );

			    cvCopy(frame_copy, cur_frame, NULL);
				}
				else
				{
					cvShowImage("Capturing...", cur_frame);
				    cvWaitKey( 10 );
				    continue;
				}
            
				
			    // Wait for a while before proceeding to the next frame
				if( cvWaitKey( 10 ) >= 0 )
				{
				//	MessageBox("Vedio has been done!");
					cvDestroyWindow("Capturing...");
					cvNamedWindow("Capturing Done", 1);
				    cvShowImage("Capturing Done", frame_copy);

					delete m_pCv4;
					m_pCv4= NULL;

					m_checkDetect.EnableWindow(FALSE);
	                m_checkPause.EnableWindow(FALSE);

					break;
					
				}

			//	cvWaitKey(40);

				
			}
			cvReleaseImage( &frame_copy );
			cvReleaseCapture(&capture);
			cvReleaseVideoWriter(&videoWriter);
		}

	}
//	delete m_pCv;

	
}


void CFaceRecognitionDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDialog::OnTimer(nIDEvent);
}

//��ͼƬ�ؼ�����ʾͼƬ
void CFaceRecognitionDlg::DrawPicToHDC(IplImage *img,UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC(); 
	HDC hDC=  pDC->GetSafeHdc(); 
	CRect rect; 
	GetDlgItem(ID)->GetClientRect(&rect); 
	CvvImage cimg; 
  
	cimg.CopyOf(img);    
	cimg.DrawToHDC(hDC,&rect);

/* ����ͼ����ʾ
	CDC *pDC = GetDC(); 
HDC hDC= pDC->GetSafeHdc(); 
CRect rect; 
GetClientRect(&rect); 
CvvImage cimg; 
cimg.CopyOf(img); 
cimg.DrawToHDC(hDC,&rect); 
*/
}


/*
void CFaceRecognitionDlg::OnBnClickedPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	if(!m_pCv->pause)
//		m_pCv->pause= TRUE;
//	else
//		m_pCv->pause= FALSE;
	
}

void CFaceRecognitionDlg::OnBnClickedCease()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pCv3)
		m_pCv3->cease= TRUE;
	if(m_pCv4)
		m_pCv4->cease= TRUE;
}
*/
void CFaceRecognitionDlg::OnBnClickedRadiodetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bDetect= FALSE;

	m_checkFrontal.EnableWindow(FALSE);
	m_checkProfile.EnableWindow(FALSE);

	if(m_pCv1 && m_pCv1->getImage())
		cvvShowImage(m_openPath, m_pCv1->getImage());
	
}

void CFaceRecognitionDlg::OnBnClickedRadiodetect2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bDetect= TRUE;

	m_checkFrontal.EnableWindow(TRUE);
	m_checkProfile.EnableWindow(TRUE);

	if(m_pCv1 && m_pCv1->getImage())
	{
		IplImage* img=m_pCv1->getImage();
		IplImage* copy = cvCreateImage( cvSize(img->width,img->height),
                                            IPL_DEPTH_8U, img->nChannels );
		cvCopy(img, copy, NULL);
		cvvShowImage(m_openPath, m_pCv1->detectFace(copy));
	}
	
}

void CFaceRecognitionDlg::OnBnClickedCheckdetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pCv3 && m_pCv3->getCamCapture())
	{
		m_pCv3->frontal= TRUE;
		if( m_pCv3->detect )
	    {
		//	MessageBox("draw the face"); 
			m_pCv3->detect= FALSE;
		   
		}
		else
		{
		// MessageBox("do not draw the face");
		   m_pCv3->detect= TRUE;
		}
	}
	if(m_pCv4 && m_pCv4->getCamCapture())
	{
		m_pCv4->frontal= TRUE;
		if( m_pCv4->detect )
	    {
		//	MessageBox("draw the face"); 
			m_pCv4->detect= FALSE;
		   
		}
		else
		{
		// MessageBox("do not draw the face");
		   m_pCv4->detect= TRUE;
		}
	}
//	else
//		MessageBox("CAM has not be Loaded!");

}

void CFaceRecognitionDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pCv3 && m_pCv3->getCamCapture())
	{
		if( m_pCv3->pause )
	    {
		//	MessageBox("draw the face"); 
			m_pCv3->pause= FALSE;
		   
		}
		else
		{
		// MessageBox("do not draw the face");
		   m_pCv3->pause= TRUE;
		}
	}
	if(m_pCv4 && m_pCv4->getCamCapture())
	{
		if( m_pCv4->pause )
	    {
		//	MessageBox("draw the face"); 
			m_pCv4->pause= FALSE;
		   
		}
		else
		{
		// MessageBox("do not draw the face");
		   m_pCv4->pause= TRUE;
		}
	}
}

void CFaceRecognitionDlg::OnBnClickedAbout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg dlg;
	dlg.DoModal();

}

void CFaceRecognitionDlg::OnBnClickedCheckprofile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pCv1 && m_pCv1->getImage())
	{
		if( m_pCv1->profile )
	    {
		//	MessageBox("draw the face"); 
			m_pCv1->profile= FALSE;
		   
		}
		else
		{
		// MessageBox("do not draw the face");
		   m_pCv1->profile= TRUE;
		}

		IplImage* img=m_pCv1->getImage();
		IplImage* copy = cvCreateImage( cvSize(img->width,img->height),
                                            IPL_DEPTH_8U, img->nChannels );
		cvCopy(img, copy, NULL);
		cvvShowImage(m_openPath, m_pCv1->detectFace(copy));
	}

	if(m_pCv2 && m_pCv2->getCapture())
	{
		if( m_pCv2->profile )
	    {
		//	MessageBox("draw the face"); 
			m_pCv2->profile= FALSE;
		   
		}
		else
		{
		// MessageBox("do not draw the face");
		   m_pCv2->profile= TRUE;
		}
	}
}

void CFaceRecognitionDlg::OnBnClickedCheckfrontal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pCv1 && m_pCv1->getImage())
	{
		if( m_pCv1->frontal )
	    {
		//	MessageBox("draw the face"); 
			m_pCv1->frontal= FALSE;
		   
		}
		else
		{
		// MessageBox("do not draw the face");
		   m_pCv1->frontal= TRUE;
		}

		IplImage* img=m_pCv1->getImage();
		IplImage* copy = cvCreateImage( cvSize(img->width,img->height),
                                            IPL_DEPTH_8U, img->nChannels );
		cvCopy(img, copy, NULL);
		cvvShowImage(m_openPath, m_pCv1->detectFace(copy));
	}

	if(m_pCv2 && m_pCv2->getCapture())
	{
		if( m_pCv2->frontal )
	    {
		//	MessageBox("draw the face"); 
			m_pCv2->frontal= FALSE;
		   
		}
		else
		{
		// MessageBox("do not draw the face");
		   m_pCv2->frontal= TRUE;
		}
	}

}

void CFaceRecognitionDlg::OnBnClickedOpenTrain()
{
	// TODO: Add your control notification handler code here
	m_pCv1->OpenTrainFile();
}

void CFaceRecognitionDlg::OnNMReleasedcaptureSldframerate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
