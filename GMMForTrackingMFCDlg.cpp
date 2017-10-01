
// GMMForTrackingMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GMMForTrackingMFC.h"
#include "GMMForTrackingMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGMMForTrackingMFCDlg �Ի���



CGMMForTrackingMFCDlg::CGMMForTrackingMFCDlg(CWnd* pParent /*=NULL*/)//���캯��
	: CDialogEx(CGMMForTrackingMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//����������ʼ��
	gmm_max_compont = 5;//GMM���ģ�͵�������Ĭ��Ϊ5
	gmm_learn_alpha = 0.005;//ѧϰ��Ĭ��Ϊ0.005
	gmm_threshold_sumw = 0.7;//������Ĭ��Ϊ0.7
	gmm_test_T = 2.5;//ǰ�������ֵĬ��Ϊ2.5
	gmm_end_frame = 200;//GMMģ�ͳ�ʼѵ��֡��Ĭ��Ϊ200

	strRunState = _T("");
}

void CGMMForTrackingMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RunState, m_editRunState_View);
}

BEGIN_MESSAGE_MAP(CGMMForTrackingMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Frame, &CGMMForTrackingMFCDlg::OnBnClickedButtonFrame)
	ON_BN_CLICKED(IDC_BUTTON_Run, &CGMMForTrackingMFCDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_AirPlane, &CGMMForTrackingMFCDlg::OnBnClickedButtonAirplane)
	ON_BN_CLICKED(IDC_BUTTON_Video, &CGMMForTrackingMFCDlg::OnBnClickedButtonVideo)
END_MESSAGE_MAP()


// CGMMForTrackingMFCDlg ��Ϣ�������

BOOL CGMMForTrackingMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	ShowWindow(SW_MAXIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ�������ı���
	SetDlgItemInt(IDC_EDIT_MaxCompont, gmm_max_compont); 

	char s[128];
	sprintf(s,"%f",gmm_learn_alpha); 
	CString str = CString(s); 
	SetDlgItemText(IDC_EDIT_LearnAlpha, str);

	char s1[128];
	sprintf(s1,"%f",gmm_threshold_sumw); 
	CString str1 = CString(s1); 
	SetDlgItemText(IDC_EDIT_T, str1);

	char s2[128];
	sprintf(s2,"%f",gmm_test_T); 
	CString str2 = CString(s2); 
	SetDlgItemText(IDC_EDIT_TestT, str2);

	SetDlgItemInt(IDC_EDIT_TrainFrameNumber, gmm_end_frame); 

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGMMForTrackingMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGMMForTrackingMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGMMForTrackingMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGMMForTrackingMFCDlg::OnBnClickedButtonFrame()//��ȡ��Ƶ֡����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ѡ���ļ���
	TCHAR           szFolderPath[MAX_PATH] = {0};  
	/*CString         */strFolderPath = TEXT("");  

	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot   = 0;  
	sInfo.lpszTitle   = _T("��ѡ��һ���ļ��У�");  
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;  
	sInfo.lpfn     = NULL;  

	// ��ʾ�ļ���ѡ��Ի���  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// ȡ���ļ�����  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
		}  
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	}  
	strFolderPath += "\\*.*"; 

	strRunState += _T("/*******��������*****/\r\n") + strFolderPath;
	m_editRunState_View.SetWindowText(strRunState); 
}


void CGMMForTrackingMFCDlg::OnBnClickedButtonRun()//�����¼�--�����㷨����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�������ı����е�����
	CString strMaxCompont;
	CString strLearnAlpha;
	CString strT;
	CString strTestT;
	CString strEndFrame;
	GetDlgItem(IDC_EDIT_MaxCompont)->GetWindowText(strMaxCompont);
	GetDlgItem(IDC_EDIT_LearnAlpha)->GetWindowText(strLearnAlpha);
	GetDlgItem(IDC_EDIT_T)->GetWindowText(strT);
	GetDlgItem(IDC_EDIT_TestT)->GetWindowText(strTestT);
	GetDlgItem(IDC_EDIT_TrainFrameNumber)->GetWindowText(strEndFrame);

	if(strFolderPath == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("�����ȶ�ȡ���ݣ�"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}

	if(strMaxCompont == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("GMM������������ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strLearnAlpha == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("ѧϰ�ʱ���ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strT == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("�����ʱ���ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strTestT == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("ǰ�������ֵ����ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strEndFrame == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("ѵ��֡������ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	gmm_max_compont = _tstoi(strMaxCompont);
	gmm_learn_alpha = _tstof(strLearnAlpha);
	gmm_threshold_sumw = _tstof(strT);
	gmm_test_T = _tstof(strTestT);
	gmm_end_frame = _tstoi(strEndFrame);

	GMM gmm(gmm_max_compont,gmm_learn_alpha,gmm_threshold_sumw,gmm_test_T,gmm_end_frame);

	int num = -3;

	CFileFind finder;
	CString strPath;
	CString strDir = strFolderPath;
	BOOL bWorking = finder.FindFile(strDir);

	while (bWorking)
	{
		num++;
		bWorking = finder.FindNextFile();
		strPath=finder.GetFilePath();
		if (num>=0)
		{
			//strPath������Ҫ��ȡTestĿ¼�µ��ļ��к��ļ�������·����
			if (num <= gmm.end_frame)
			{
				strRunState = _T("/*******ѵ��*****/\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
				char s[128];
				sprintf(s,"%d",num); 
				CString str = CString(s); 
				strRunState += str + _T(" frame processing!\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
			}
			else
			{
				strRunState = _T("/*******����*****/\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
				char s[128];
				sprintf(s,"%d",num); 
				CString str = CString(s); 
				strRunState += str + _T(" frame processing!\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
			}


			//CString -> string
			CStringA stra(strPath.GetBuffer(0));
			strPath.ReleaseBuffer();
			std::string imgpath=stra.GetBuffer(0);
			stra.ReleaseBuffer();

			IplImage* image = NULL;
			IplImage* image_gray = NULL;
			IplImage* foreground = NULL;

			image = cvLoadImage(imgpath.c_str());
			if(!image)
			{
				cout<<"something is wrong!!"<<endl;
				cvWaitKey(0);
				//release images
				cvDestroyWindow("src");
				cvDestroyWindow("foreground");
				return ;
			}
			cvShowImage("src",image);
			image_gray = cvCreateImage(cvGetSize(image),8,1);
			foreground = cvCreateImage(cvGetSize(image),8,1);
			cvCvtColor(image,image_gray,CV_BGR2GRAY);
			/****when it is the first frame,set the default parameter****/
			if (num == 0)
			{
				gmm.gmm_init(image_gray);
				gmm.gmm_first_frame(image_gray);
			}
			//the train of gmm phase
			if (num<gmm.end_frame)
			{
				gmm.gmm_update(image_gray);
			}
			/****chose the fitting number of component in gmm****/
			if (num == gmm.end_frame)
			{
				gmm.gmm_fit_num(image_gray);
			}
			/****start the test phase****/
			if (num>gmm.end_frame)
			{
				gmm.gmm_test(image_gray);
				gmm.gmm_update(image_gray);
				gmm.gmm_fit_num(image_gray);
				foreground = cvCloneImage(gmm.FGMask);
				cvShowImage("foreground",foreground);
			}

			//release images
			cvReleaseImage(&image);
			cvReleaseImage(&image_gray);
			cvReleaseImage(&foreground);

			char c=(char)waitKey(1);
			if(c==27)//if press the ESC key,the exit the proggram
				break;
		}
		

	}
	finder.Close();


	cvDestroyWindow("src");
	cvDestroyWindow("foreground");
}


void CGMMForTrackingMFCDlg::OnBnClickedButtonAirplane()//Ӧ��--�ɻ�����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�������ı����е�����
	CString strMaxCompont;
	CString strLearnAlpha;
	CString strT;
	CString strTestT;
	CString strEndFrame;
	GetDlgItem(IDC_EDIT_MaxCompont)->GetWindowText(strMaxCompont);
	GetDlgItem(IDC_EDIT_LearnAlpha)->GetWindowText(strLearnAlpha);
	GetDlgItem(IDC_EDIT_T)->GetWindowText(strT);
	GetDlgItem(IDC_EDIT_TestT)->GetWindowText(strTestT);
	GetDlgItem(IDC_EDIT_TrainFrameNumber)->GetWindowText(strEndFrame);

	if(strFolderPath == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("�����ȶ�ȡ���ݣ�"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}

	if(strMaxCompont == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("GMM������������ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strLearnAlpha == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("ѧϰ�ʱ���ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strT == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("�����ʱ���ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strTestT == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("ǰ�������ֵ����ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strEndFrame == "")
	{
		// ��ʾ��Ϣ�Ի���   
		MessageBox(_T("ѵ��֡������ָ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);   
		return;
	}
	gmm_max_compont = _tstoi(strMaxCompont);
	gmm_learn_alpha = _tstof(strLearnAlpha);
	gmm_threshold_sumw = _tstof(strT);
	gmm_test_T = _tstof(strTestT);
	gmm_end_frame = _tstoi(strEndFrame);

	GMM gmm(gmm_max_compont,gmm_learn_alpha,gmm_threshold_sumw,gmm_test_T,gmm_end_frame);

	int num = -3;

	CFileFind finder;
	CString strPath;
	CString strDir = strFolderPath;
	BOOL bWorking = finder.FindFile(strDir);

	while (bWorking)
	{
		num++;
		bWorking = finder.FindNextFile();
		strPath=finder.GetFilePath();
		if (num>=0)
		{
			//strPath������Ҫ��ȡTestĿ¼�µ��ļ��к��ļ�������·����
			if (num <= gmm.end_frame)
			{
				strRunState = _T("/*******ѵ��*****/\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
				char s[128];
				sprintf(s,"%d",num); 
				CString str = CString(s); 
				strRunState += str + _T(" frame processing!\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
			}
			else
			{
				strRunState = _T("/*******����*****/\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
				char s[128];
				sprintf(s,"%d",num); 
				CString str = CString(s); 
				strRunState += str + _T(" frame processing!\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
			}


			//CString -> string
			CStringA stra(strPath.GetBuffer(0));
			strPath.ReleaseBuffer();
			std::string imgpath=stra.GetBuffer(0);
			stra.ReleaseBuffer();

			IplImage* image = NULL;
			IplImage* image_gray = NULL;
			IplImage* foreground = NULL;
			IplImage* foregroundRefine = NULL;

			image = cvLoadImage(imgpath.c_str());
			if(!image)
			{
				cout<<"something is wrong!!"<<endl;
				cvWaitKey(0);
				//release images
				cvDestroyWindow("src");
				cvDestroyWindow("foreground");
				cvDestroyWindow("refine");
				return ;
			}

			IplImage* imageScale1 = NULL;
			IplImage* imageScale2 = NULL;

			imageScale1 = cvCreateImage(cvSize(image->width/2,image->height/2),8,3);
			cvPyrDown(image,imageScale1);//��С�ı�

			imageScale2 = cvCreateImage(cvSize(imageScale1->width/2,imageScale1->height/2),8,3);
			cvPyrDown(imageScale1,imageScale2);//��С�ı�

			cvShowImage("src",imageScale2);
			image_gray = cvCreateImage(cvGetSize(imageScale2),8,1);
			foreground = cvCreateImage(cvGetSize(imageScale2),8,1);
			cvZero(foreground);
			cvCvtColor(imageScale2,image_gray,CV_BGR2GRAY);

			//��ȡ�ܵ�ROI
			cvSetImageROI(image_gray,cvRect(0,cvRound(image_gray->height*0.44),image_gray->width,cvRound(image_gray->height*0.2)));
			IplImage* image_grayROI = cvCreateImage(cvGetSize(image_gray),8,1);
			cvCopy(image_gray,image_grayROI);

			/****when it is the first frame,set the default parameter****/
			if (num == 0)
			{
				gmm.gmm_init(image_grayROI);
				gmm.gmm_first_frame(image_grayROI);
			}
			//the train of gmm phase
			if (num<gmm.end_frame)
			{
				gmm.gmm_update(image_grayROI);
			}
			/****chose the fitting number of component in gmm****/
			if (num == gmm.end_frame)
			{
				gmm.gmm_fit_num(image_grayROI);
			}
			/****start the test phase****/
			if (num>gmm.end_frame)
			{
				gmm.gmm_test(image_grayROI);
				gmm.gmm_update(image_grayROI);
				gmm.gmm_fit_num(image_grayROI);
				/*foreground = cvCloneImage(gmm.FGMask);*/
				for (int y=0;y<gmm.FGMask->height;y++)
				{
					for (int x=0;x<gmm.FGMask->width;x++)
					{
						int pixel = cvRound(cvGetReal2D(gmm.FGMask,y,x));
						cvSetReal2D(foreground,y+cvRound(foreground->height*0.44),x,pixel);
					}
				}
				foregroundRefine = cvCloneImage(foreground);
				cvMorphologyEx(foreground,foregroundRefine,NULL,NULL,CV_MOP_CLOSE,1);
				cvShowImage("foreground",foreground);
				cvShowImage("refine",foregroundRefine);

				//��Ƶ�л����ɻ�����
				CvRect airPlaneRect = AirPlaneDetection(foregroundRefine);
				cvRectangle(imageScale2,cvPoint(airPlaneRect.x,airPlaneRect.y),cvPoint(airPlaneRect.x+airPlaneRect.width,airPlaneRect.y+airPlaneRect.height),CV_RGB(255,0,0),2);
				cvShowImage("src",imageScale2);
			}
			//release images
			cvReleaseImage(&image);
			cvReleaseImage(&image_gray);
			cvReleaseImage(&foreground);
			cvReleaseImage(&foregroundRefine);
			cvReleaseImage(&imageScale1);
			cvReleaseImage(&imageScale2);
			cvReleaseImage(&image_grayROI);
			char c=(char)waitKey(1);
			if(c==27)//if press the ESC key,the exit the proggram
				break;
		}


	}
	finder.Close();

	cvDestroyWindow("src");
	cvDestroyWindow("foreground");
	cvDestroyWindow("refine");
}


CvRect CGMMForTrackingMFCDlg::AirPlaneDetection(IplImage *imageBi)//���ɻ�
{
	CvSeq *pContour = NULL; 
	CvMemStorage *pStorage = cvCreateMemStorage(0); 
	int n=cvFindContours(imageBi,pStorage,&pContour,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	CvRect planeRect = cvRect(0,0,0,0);
	int maxarea = 0;
	for(;pContour!=NULL;pContour=pContour->h_next)
	{
		int area=(int)cvContourArea(pContour);
		CvRect rect=cvBoundingRect(pContour);//����㼯�������棨up-right�����α߽� 
		if (area>maxarea)
		{
			maxarea = area;
			planeRect = rect;
		}
	}
	//�ͷ��ڴ�
	cvReleaseMemStorage(&pStorage);
	return planeRect;
}

void CGMMForTrackingMFCDlg::OnBnClickedButtonVideo()//������Ƶ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//������
}
