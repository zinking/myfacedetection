#include "StdAfx.h"
#include "cvapp.h"

// Create a string that contains the exact cascade name
  CString cascade_frontface= ".\\FaceLibrary\\haarcascade_frontalface_alt.xml";

  CString cascade_sideface= ".\\FaceLibrary\\haarcascade_profileface.xml";


CCvApp::CCvApp()
{
	pImg= NULL;
	capture= NULL;
	camcapture=NULL;
	
	detect= FALSE;
	pause= FALSE;
	exit= FALSE;
	profile= FALSE;
	frontal= FALSE;
}

CCvApp::CCvApp(CString fileName, int nType)
{
	detect= FALSE;
	pause= FALSE;
	exit= FALSE;
	profile= FALSE;
	frontal= FALSE;

	switch( nType )
	{
	case 0:
		pImg= cvLoadImage( fileName, 1);   //load  an image
		if(pImg == NULL)
			AfxMessageBox("Image has loaded unsuccessfully!");
		capture= NULL;
		camcapture=NULL;
		break;
	case 1:
        pImg= NULL;
		camcapture=NULL;
		capture= cvCaptureFromFile(fileName); //load capture from a video file
		if(capture == NULL)
			AfxMessageBox("Video File has loaded unsuccessfully!");
		break;
	case 2:
		pImg= NULL;
		capture= NULL;
		camcapture= cvCaptureFromCAM(-1); //load capture from a CAM
		if(camcapture == NULL)
			AfxMessageBox("CAM has loaded unsuccessfully!");
		break;
	default:
		AfxMessageBox("No Image/Capture has Initialized!");
	}

}

CCvApp::~CCvApp(void)
{
	if( pImg)
	{
		cvReleaseImage(&pImg);
		pImg= NULL;
	}
	if( capture)
	{
		cvReleaseCapture( &capture);
		capture= NULL;
	}

}

//retrun cuurent image
IplImage* CCvApp::getImage()
{
	return pImg;
}

//return cuurent capture
CvCapture* CCvApp::getCapture()
{
	return capture;
}

CvCapture* CCvApp::getCamCapture()
{
	return camcapture;
}

//display an image or a vedio file
void CCvApp::display(CString fileName)
{
	if(fileName =="")
		fileName="CAM";

	//显示窗口
	cvvNamedWindow(fileName, 1);
	
	//显示图片
	if(pImg)
	{
	//	cvvNamedWindow(fileName, 1);
		cvvShowImage(fileName, pImg);
	}
		
	//显示视频
	else if( camcapture)
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
			
			if(!pause)
			{
			if(!detect)
				cvShowImage(fileName, frame_copy);

			//draw the face
			else
				cvShowImage(fileName, detectFace(frame_copy));

			cur_frame=cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );

			cvCopy(frame_copy, cur_frame, NULL);
			}
			else
			{
				cvShowImage(fileName, cur_frame);
				cvWaitKey( 30 );
				continue;
			}
 

				
		    // Wait for a while before proceeding to the next frame
			if( cvWaitKey( 30 ) >= 0 )
				break;

	/*		//pause capturing frames 
			if( pause )
				cvShowImage(fileName, frame_copy);
			else
				continue;*/
		}
		cvReleaseImage( &frame_copy );
	}

}

//detect and draw the region of face in an image  
IplImage* CCvApp::detectFace(IplImage* img)
{
	// Create memory for calculations
    static CvMemStorage* storage = 0;
	 static CvMemStorage* storage1 = 0;

    // Create a new Haar classifier
    static CvHaarClassifierCascade* cascade = 0;
	static CvHaarClassifierCascade* cascade1 = 0;

    int scale = 1;

    // Create a new image based on the input image
    IplImage* temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 8, 3 );

    // Create two points to represent the face locations
    CvPoint pt1, pt2;
    int i;

    // Load the HaarClassifierCascade
	char* name = cascade_frontface.GetBuffer();
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_frontface.GetBuffer(), 0, 0, 0 );
	cascade1 = (CvHaarClassifierCascade*)cvLoad( cascade_sideface, 0, 0, 0 );
	
    
    // Check whether the cascade has loaded successfully. Else report and error and quit
    if( !cascade & !cascade1 )
    {
    //    fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
		AfxMessageBox("Could not load classifier cascade .");
        return NULL;
    }
    
    // Allocate the memory storage
    storage = cvCreateMemStorage(0);
	 storage1 = cvCreateMemStorage(0);

    // Create a new named window with title: result
  //  cvNamedWindow( "result", 1 );

    // Clear the memory storage which was used before
    cvClearMemStorage( storage );
	cvClearMemStorage( storage1 );

    CvSeq* faces= NULL;
	CvSeq* faces1=NULL;
    // Find whether the cascade is loaded, to find the faces. If yes, then:
	if( frontal )
	{
		if( cascade )
		{

        // There can be more than one face in an image. So create a growable sequence of faces.
        // Detect the objects and store them in the sequence
			faces = cvHaarDetectObjects( img, cascade, storage,
                                            1.2, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(24, 24) );
        
/*        // Loop the number of faces found.
			for( i = 0; i < (faces ? faces->total : 0); i++ )
			{
           // Create a new rectangle for drawing the face
				CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

            // Find the dimensions of the face,and scale it if necessary
				pt1.x = r->x*scale;
				pt2.x = (r->x+r->width)*scale;
				pt1.y = r->y*scale;
				pt2.y = (r->y+r->height)*scale;

            // Draw the rectangle in the input image
			cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
	
				exit= TRUE;
			}*/
		}
	}

	if( profile)
	{
		if( cascade1 )
        {

        // There can be more than one face in an image. So create a growable sequence of faces.
        // Detect the objects and store them in the sequence
			faces1 = cvHaarDetectObjects( img, cascade1, storage1,
                                            1.2, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(24, 24) );
        
/*        // Loop the number of faces found.
			for( i = 0; i < (faces ? faces->total : 0); i++ )
			{
           // Create a new rectangle for drawing the face
				CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

            // Find the dimensions of the face,and scale it if necessary
				pt1.x = r->x*scale;
				pt2.x = (r->x+r->width)*scale;
				pt1.y = r->y*scale;
				pt2.y = (r->y+r->height)*scale;

            // Draw the rectangle in the input image
				cvRectangle( img, pt1, pt2, CV_RGB(0,0,255), 3, 8, 0 );

				exit= TRUE;
			}*/
         }
	}
      if( faces)
	  {
           // Loop the number of faces found.
			for( i = 0; i < (faces ? faces->total : 0); i++ )
			{
           // Create a new rectangle for drawing the face
				CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

            // Find the dimensions of the face,and scale it if necessary
				pt1.x = r->x*scale;
				pt2.x = (r->x+r->width)*scale;
				pt1.y = r->y*scale;
				pt2.y = (r->y+r->height)*scale;

            // Draw the rectangle in the input image
			cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
	
				exit= TRUE;
			}
	  }

	  if( faces1)
	  {

			       // Loop the number of faces found.
			for( i = 0; i < (faces1 ? faces1->total : 0); i++ )
			{
           // Create a new rectangle for drawing the face
				CvRect* r = (CvRect*)cvGetSeqElem( faces1, i );

            // Find the dimensions of the face,and scale it if necessary
				pt1.x = r->x*scale;
				pt2.x = (r->x+r->width)*scale;
				pt1.y = r->y*scale;
				pt2.y = (r->y+r->height)*scale;

            // Draw the rectangle in the input image
			cvRectangle( img, pt1, pt2, CV_RGB(0,0,255), 3, 8, 0 );
	
				exit= TRUE;
			}
	  }

    // Show the image in the window named "result"
   //  cvShowImage( "result", img );

    // Release the temp image created.
    cvReleaseImage( &temp );

	//return the image with drawing the region of the faces
	return img;


}


int CCvApp::OpenTrainFile(void)
{

	TCHAR		szFilter[]   =   "图像文件，avi视频文件|*.*";   
	CFileDialog	mFileDlg(true,   NULL,   NULL,   OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT,   szFilter);   

	if(mFileDlg.DoModal()   ==   IDOK){   
		cascade_frontface = mFileDlg.GetPathName();
	} 


	return 0;
}
