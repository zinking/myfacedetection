#pragma once

#include "cv.h"
#include "highgui.h"

class CCvApp
{
private:
	IplImage* pImg;   //current image
	CvCapture* capture; //current avi file capture
	CvCapture* camcapture; //current cam capture

public:
	CCvApp();
	CCvApp(CString fileName, int nType= 0);
	~CCvApp(void);

    BOOL detect;  //draw the region of face or not
	BOOL exit;    //the current image exit a face
	BOOL pause;   //pause capturing from cam or video file
	BOOL profile;  //detect profile faces
	BOOL frontal;
    

	void display(CString fileName);
	IplImage* getImage();
	CvCapture* getCapture();
	CvCapture* getCamCapture();
    IplImage* detectFace(IplImage* img);
	
	int OpenTrainFile(void);
};



