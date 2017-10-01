/******************************************************************************************
*
*	The class implement the paper"Adaptive background mixture models for real-time tracking"
*		by Yangyang Hu at 2014/11/19
*
*******************************************************************************************/
#include "Global.h"

class GMM
{
public:
	//����gmmģ���õ��Ĳ�������
	int gmm_max_compont;  //GMM���ģ�͵���������5
	float gmm_learn_alpha; //ѧϰ��0.005
	float gmm_threshold_sumw;  //������0.7
	int end_frame;  //ѵ��֡��
	float default_sigma; //Ĭ�ϵĳ�ʼ����Ϊ15
	float default_weight;//Ĭ�ϵ�ȨֵΪ0.005
	float default_var_threshold;//Ĭ�ϵķ�����ֵΪ2.5*2.5

	float gmm_testT;//ǰ�������ֵ

	//gmm��Ȩֵw����ֵu���������sigma
	Mat w[GMM_MAX_NUM];
	Mat u[GMM_MAX_NUM];
	Mat sigma[GMM_MAX_NUM];

	//����ͼ��ÿ������gmm���ʵĸ���
	Mat fit_num;

	//����ǰ����Ĥ
	IplImage* FGMask;

public:
	//��Ա����
	GMM(int gmm_max_compont1,float gmm_learn_alpha1,float gmm_threshold_sumw1,float gmm_testT1,int end_frame1);
	~GMM();
	void gmm_init(IplImage *img);//gmm�����ʼ������

	void gmm_first_frame(IplImage *img);  //gmm��һ֡��ʼ������

	void gmm_update(IplImage* img);//gmm���¹���

	void gmm_fit_num(IplImage* img);//������ͼ��ÿ������gmmѡ����ʵĸ���

	void gmm_test(IplImage* img);//gmm�Ĳ��Ժ���

};