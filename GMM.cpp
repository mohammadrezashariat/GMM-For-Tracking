#include "stdafx.h"
#include "GMM.h"

//���캯��
GMM::GMM(int gmm_max_compont1,float gmm_learn_alpha1,float gmm_threshold_sumw1,float gmm_testT1,int end_frame1)
{
	gmm_max_compont = gmm_max_compont1;  //GMM���ģ�͵���������5
	gmm_learn_alpha = gmm_learn_alpha1; //ѧϰ��0.005
	gmm_threshold_sumw = gmm_threshold_sumw1;  //������0.7
	end_frame = /*(int)(1/gmm_learn_alpha1)*/end_frame1;  //ѵ��֡��
	default_sigma = 15; //Ĭ�ϵĳ�ʼ����Ϊ15
	default_weight = gmm_learn_alpha1;//Ĭ�ϵ�ȨֵΪ0.005
	default_var_threshold = 2.5*2.5;//Ĭ�ϵķ�����ֵΪ2.5*2.5

	gmm_testT = gmm_testT1*gmm_testT1;//ǰ�������ֵĬ���������2.5*2.5
}

//��������
GMM::~GMM()
{

}

//gmm�����ʼ������
void GMM::gmm_init(IplImage* img)
{
	for (int i=0;i<gmm_max_compont;i++)
	{
		w[i] = Mat(img->height,img->width,CV_32FC1,0.0f);
		u[i] = Mat(img->height,img->width,CV_32FC1,0.0f);
		sigma[i] = Mat(img->height,img->width,CV_32FC1,0.0f);
	}
	fit_num = Mat(img->height,img->width,CV_8UC1,-1);
	FGMask = cvCreateImage(cvGetSize(img),8,1);
}

 //gmm��һ֡��ʼ������
void GMM::gmm_first_frame(IplImage *img)
{
	for (int y=0;y<img->height;y++)
	{
		for (int x=0;x<img->width;x++)
		{
			w[0].at<float>(y,x) = 1.0;
			u[0].at<float>(y,x) = (float)cvGetReal2D(img,y,x);
			sigma[0].at<float>(y,x) = default_sigma;

			for(int k=1;k<gmm_max_compont;k++)    
			{
				/****when assigment this,we must be very carefully****/
				w[k].at<float>(y,x)=0.0;
				u[k].at<float>(y,x)=0.0;
				sigma[k].at<float>(y,x)=default_sigma;//��ֹ��������ʱ�з�ĸΪ0�����
			}
		}
	}
}

//gmm���¹���
void GMM::gmm_update(IplImage* img)
{
	for (int y=0;y<img->height;y++)
	{
		for (int x=0;x<img->width;x++)
		{
			int k = 0;
			int nfit = 0;
			for (;k<gmm_max_compont;k++)
			{
				float delam = abs((float)cvGetReal2D(img,y,x)-u[k].at<float>(y,x));
				float dis = delam*delam;
				if (dis<default_var_threshold*sigma[k].at<float>(y,x))//the present pixpel is fit the component
				{
					/****update the weight***/
					w[k].at<float>(y,x) = (1-gmm_learn_alpha)*w[k].at<float>(y,x) + gmm_learn_alpha*1;

					/****update the average***/
					double pgsm = (1/sqrt(2*PI*sigma[k].at<float>(y,x)))*exp(-0.5*dis/sigma[k].at<float>(y,x));
					double lou = gmm_learn_alpha*pgsm;
					u[k].at<float>(y,x)= (1-lou)*u[k].at<float>(y,x) + lou*cvGetReal2D(img,y,x);

					/****update the variance****/
					sigma[k].at<float>(y,x) = (1-lou)*sigma[k].at<float>(y,x) + lou*dis;

				}
				else
				{
					/***update the weight***/
					w[k].at<float>(y,x) = (1-gmm_learn_alpha)*w[k].at<float>(y,x);
					nfit++;

					/*****u,sigma remain the same****/
				}
			}

			//��gmm������˹�������򣬴Ӵ�С����������Ϊw/sigma
			for(int kk=0;kk<gmm_max_compont;kk++)
			{
				for(int rr=kk;rr<gmm_max_compont;rr++)
				{
					if(w[rr].at<float>(y,x)/sigma[rr].at<float>(y,x)>w[kk].at<float>(y,x)/sigma[kk].at<float>(y,x))
					{
						float temp_w,temp_u,temp_sigma;
						//Ȩֵ����
						temp_w=w[rr].at<float>(y,x);
						w[rr].at<float>(y,x)=w[kk].at<float>(y,x);
						w[kk].at<float>(y,x)=temp_w;

						//��ֵ����
						temp_u=u[rr].at<float>(y,x);
						u[rr].at<float>(y,x)=u[kk].at<float>(y,x);
						u[kk].at<float>(y,x)=temp_u;

						//�����
						temp_sigma=sigma[rr].at<float>(y,x);
						sigma[rr].at<float>(y,x)=sigma[kk].at<float>(y,x);
						sigma[kk].at<float>(y,x)=temp_sigma;
					}
				}
			}

			/*******���û�����������ĸ�˹�������¿�ʼ����һ����˹�ֲ�****/
			if (nfit==gmm_max_compont&&w[gmm_max_compont-1].at<float>(y,x)==0)//if there is no exit component fit,then start a new componen
			{
				for (int h=0;h<gmm_max_compont;h++)
				{
					if (w[h].at<float>(y,x) == 0)
					{
						w[h].at<float>(y,x) = default_weight;
						u[h].at<float>(y,x) = cvGetReal2D(img,y,x);
						sigma[h].at<float>(y,x) = default_sigma;
						for (int q=0;q<gmm_max_compont&&q!=h;q++)
						{
							/****update the other unfit's weight,u and sigma remain unchanged****/
							w[q].at<float>(y,x) = (1-gmm_learn_alpha)*w[q].at<float>(y,x);
						}
						break;////�ҵ���һ��Ȩֵ��Ϊ0�ļ���
					}
				}
			}
			//���GMM_MAX_COMPONT��������ֵ�������������ĸ�˹����Ȩֵ�����ĸ�˹��Ȩֵ���䣬ֻ���¾�ֵ�ͷ���
			else if (nfit==gmm_max_compont&&w[gmm_max_compont-1].at<float>(y,x)!=0)
			{
				u[gmm_max_compont-1].at<float>(y,x) = cvGetReal2D(img,y,x);
				sigma[gmm_max_compont-1].at<float>(y,x) = default_sigma;
			}
		}
	}
}//end the train phase

//������ͼ��ÿ������gmmѡ����ʵĸ���
void GMM::gmm_fit_num(IplImage* img)
{
	for (int y=0;y<img->height;y++)
	{
		for (int x=0;x<img->width;x++)
		{
			float sum_w = 0.0;//���¸�ֵΪ0������һ���������ۻ�
			//chose the fittest number fit_num
			for (int b=0;b<gmm_max_compont;b++)
			{
				sum_w += w[b].at<float>(y,x);
				if (sum_w>gmm_threshold_sumw)
				{
					fit_num.at<unsigned char>(y,x) = b+1;
					break;
				}
			}
		}
	}
}

//gmm���Ժ�����ʵ��
void GMM::gmm_test(IplImage* img)
{
	for (int y=0;y<img->height;y++)
	{
		for (int x=0;x<img->width;x++)
		{
			unsigned char a=0;
			for (;a<fit_num.at<unsigned char>(y,x);a++)
			{
				double delam = abs(cvGetReal2D(img,y,x)-u[a].at<float>(y,x));
				double dis = delam*delam;
				if (dis<gmm_testT*sigma[a].at<float>(y,x))
				{
					cvSetReal2D(FGMask,y,x,0);//����
					break;
				}
			}
			if (a==fit_num.at<unsigned char>(y,x))
			{
				cvSetReal2D(FGMask,y,x,255);//ǰ��
			}
		}
	}
}