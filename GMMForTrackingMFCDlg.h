
// GMMForTrackingMFCDlg.h : ͷ�ļ�
//

#pragma once
#include "GMM.h"
#include "afxwin.h"

// CGMMForTrackingMFCDlg �Ի���
class CGMMForTrackingMFCDlg : public CDialogEx
{
// ����
public:
	CGMMForTrackingMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GMMFORTRACKINGMFC_DIALOG };

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
	CString         strFolderPath; //�洢�ļ��е�·��
	CString			strRunState;//�洢���������״̬
	int gmm_max_compont;//GMM���ģ�͵�������
	float gmm_learn_alpha;//ѧϰ��
	float gmm_threshold_sumw;//������
	float gmm_test_T;//ǰ�������ֵ
	int gmm_end_frame;//GMMģ�ͳ�ʼѵ��֡��

public:
	CvRect AirPlaneDetection(IplImage *imageBi);//���ɻ�

public:
	afx_msg void OnBnClickedButtonFrame();//��ȡ��Ƶ֡����
	afx_msg void OnBnClickedButtonRun();//�����¼�
	CEdit m_editRunState_View;//����״̬�����
	afx_msg void OnBnClickedButtonAirplane();//Ӧ��--�ɻ�����
	afx_msg void OnBnClickedButtonVideo();//������Ƶ����
};
