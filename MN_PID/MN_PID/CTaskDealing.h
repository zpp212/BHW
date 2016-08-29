#pragma once
#ifndef _TASKDEALING_H_
#define _TASKDEALING_H_
#include "CTaskDefine.h"
#include "CRFID_communicate.h"
#include <Windows.h>
#include "CPLC-COMMUNICATE.h"
#include "CMagnetic_navigation.h"
#include "CMN_PID.h"
#define PI 3.1415926
class CTaskDealing
{
	friend class CMagnativetoServoSpeed;
public:
	CTaskDealing();
	~CTaskDealing();
	void SpeedUpperBound();//�����ٶ�����
	uint16_t GetSettedSpeed(int&,CRfidCommunication&,CTaskDefine&,CPLCCommunication&, CMagnetic_Navigation&, CMagnativetoServoSpeed&);
	//�����ĸ�����ٶ�,ִ�е�ǰ�ڵ�����
	void CalculateFourServoSpeed(const double c_dVx, const double c_dVy, const double c_dw);//����Ϊ���õĵ���ٶ�
	void RotateControl(int, CPLCCommunication&);
	void TransformToPlc(CPLCCommunication&);//�����ݴ�����plcͨѶ�ļĴ�������λ���ٶ�rad/min,ʱ�䵥λ������
private:
	double m_nMaxSpeed,m_nMaxW;//�����ٶ�
	int m_nDirection;//˳ʱ�룺0,1,2,3
	uint16_t m_dwStartRfidFlag;
    double m_dVxSet, m_dVySet, m_dWSet;//��С������ϵΪ��׼��VyΪС��ֱ�з���W�ǽ��ٶ�
	double m_dR, m_dW, m_dL;//R���ְ뾶��W����L����
	double m_dW1, m_dW2, m_dW3, m_dW4;//�ĸ����ӵĽ��ٶ�
	double m_dRotateTime;//��תʱ��,��λ��
};

CTaskDealing::CTaskDealing()
{
	//���������ʼ��
	m_dR = 0.375 / 2;
	m_dW = 1.45;
	m_dL = 1.436;
	m_nMaxSpeed = 14;
	m_nMaxW = 15.3;
}
CTaskDealing::~CTaskDealing()
{
}
void CTaskDealing::SpeedUpperBound()
{
	if (abs(m_dVxSet) > m_nMaxSpeed)
		if(m_dVxSet*m_nMaxSpeed>0)
			m_dVxSet = m_nMaxSpeed;
		else
			m_dVxSet = -m_nMaxSpeed;
	if (abs(m_dVySet) > m_nMaxSpeed)
		if(m_dVySet*m_nMaxSpeed>0)
			m_dVySet = m_nMaxSpeed;
		else
			m_dVySet = -m_nMaxSpeed;
		
	if (abs(m_dWSet) > m_nMaxW)
		if (m_dWSet*m_nMaxW > 0)
			m_dWSet = m_nMaxW;
		else
		m_dWSet = -m_nMaxW;
}
void CTaskDealing::TransformToPlc(CPLCCommunication&Cplccommunication)
{
	Cplccommunication.m_dwServoVelocity[0] = m_dW1;
	Cplccommunication.m_dwServoVelocity[1] = m_dW2;
	Cplccommunication.m_dwServoVelocity[2] = m_dW3;
	Cplccommunication.m_dwServoVelocity[3] = m_dW4;
	Cplccommunication.m_dwServoVelocity[4] = m_dRotateTime*1000;
}
void CTaskDealing::RotateControl(int nAngleLable, CPLCCommunication&Cplccommunication)//��1Ϊ��ת90��
{
	uint16_t dw;//��ת������ٶ�
	dw = 36 / (m_dL + m_dW);//w=2Rw1/(W+L)������Rw1=m_nMaxSpeedm/min
	m_dRotateTime = abs(nAngleLable)*(PI / (2 * dw)) * 60;//��λ����
	if (nAngleLable)
	{
		m_dW1=m_dW2=m_nMaxSpeed/m_dR;
		m_dW3=m_dW4=-m_nMaxSpeed / m_dR;
	}
	else if (nAngleLable < 0)
	{
		m_dW1 = m_dW2 = -m_nMaxSpeed / m_dR;
		m_dW3 = m_dW4 = m_nMaxSpeed / m_dR;
	}
	TransformToPlc(Cplccommunication);
	for (double i = 10*m_dRotateTime; i < 0; --i)
	{
		Cplccommunication.SendServoVelocityToPlc(0);
		Sleep(100);
	}//��תʱ���ڣ�ÿ0.1s��plc������ͬ����
	
}
uint16_t CTaskDealing::GetSettedSpeed(int&nTasknum, CRfidCommunication&Crfidcommunication, CTaskDefine&Ctaskdefine,CPLCCommunication&Cplccommunication, CMagnetic_Navigation&CMagnetic_navigation, CMagnativetoServoSpeed&CMagnativetoservospeed)
{
		Crfidcommunication.GetRfidTagInfo(1);
		m_dwStartRfidFlag = Crfidcommunication.m_dwRfidTagInfo[3];//һ��һ��16λ�����ݾ͹�����
		if ((m_dwStartRfidFlag == Ctaskdefine.m_ppnTaskTable[nTasknum][0]&&nTasknum &&m_dwStartRfidFlag== Ctaskdefine.m_ppnTaskTable[nTasknum-1][1])|| (!nTasknum && (m_dwStartRfidFlag == Ctaskdefine.m_ppnTaskTable[nTasknum][0])))
			//�����������׽ڵ㣬��ǰ�ڵ��Ǻ�����ڵ㣻���׽ڵ㣬��ǰ�ڵ��Ǻϵ�ǰ�ڵ㼰ǰһ�����Ŀ��ڵ�
		{
			//�������ȼ�����ǰ�ڵ����ת4����ǰ�ڵ�����2��ǰ������3
			switch (Ctaskdefine.m_ppnTaskTable[nTasknum][4])
			{//ִ����ת��������ǳ������λ��
			case ROTATE_NONE://����ת
			{
				break;
			}
			case ROTATE_LEFT90:
			{
				m_nDirection = (m_nDirection + 3) % 4;
				RotateControl(-1,Cplccommunication);
				break;
			}
			case ROTATE_LEFT180:
			{
				m_nDirection = (m_nDirection + 2) % 4;
				RotateControl(-2, Cplccommunication);
				break;
			}
			case ROTATE_LEFT270:
			{
				m_nDirection = (m_nDirection + 1) % 4;
				RotateControl(-3, Cplccommunication);
				break;
			}
			case ROTATE_LEFT360:
			{
				RotateControl(-4, Cplccommunication);
				break;
			}
			case ROTATE_RIGHT90:
			{
				RotateControl(1, Cplccommunication);
				m_nDirection = (m_nDirection + 1) % 4;
				break;
			}
			case ROTATE_RIGHT180:
			{
				RotateControl(2, Cplccommunication);
				m_nDirection = (m_nDirection + 2) % 4;
				break;
			}
			case ROTATE_RIGHT270:
			{
				RotateControl(3, Cplccommunication);
				m_nDirection = (m_nDirection + 3) % 4;
				break;
			}
			case ROTATE_RIGHT360:
			{
				RotateControl(4, Cplccommunication);
				break;
			}
			default:
				break;
			}
			switch(Ctaskdefine.m_ppnTaskTable[nTasknum][2])
			{	case JOINTTASK_NONE://������
				{
					break;
				}
				case JOINTTASK_PARK://ͣ��
				{
					m_dVxSet = m_dVySet = m_dWSet = 0;
					
					break;
				}
				case JOINTTASK_LOADING://װ��
				{
					break;
				};
				case JOINTTASK_UNLOAD ://ж��
				{
					break;
				};
				case JOINTTASK_CHARGE://���
				{
					break;
				}
				default:
					break;
			}
			switch (Ctaskdefine.m_ppnTaskTable[nTasknum][3])
			{
			case FORWARD_NONE://���н�
			{
				m_dVxSet = 0;
				m_dVySet = 0;
				m_dWSet = 0;
				CalculateFourServoSpeed(m_dVxSet, m_dVySet, m_dWSet);
				TransformToPlc(Cplccommunication);
				for (double i = 10; i < 0; --i)
				{
					Cplccommunication.SendServoVelocityToPlc(0);
					Sleep(100);
				}//���ݾ���ͣ��ʱ�����
				break;
			}
			case FORWARD_FRONT://ǰ��
			{
				m_dVxSet = 0;
				m_dVySet = m_nMaxSpeed;
				m_dWSet = 0;
				uint16_t dMN1, dMN2;
				while (1)
				{
					dMN1 = CMagnetic_navigation.GetMagneticoffset(1, 0);
					//dMN2 = CMagnetic_navigation.GetMagneticoffset(3, 0);//��ʱû�ӵڶ���
					dMN2 = 0;
					CMagnativetoservospeed.CalculateDeltaSpeed(dMN1, dMN2, 0);//������ͨpid
					if ((m_dVxSet == m_nMaxSpeed)&&(m_dWSet == m_nMaxW))//���ֿ�����
					{
						if(m_dVxSet*(CMagnativetoservospeed.m_dCalculateDeltaX)<0)
							m_dVxSet += CMagnativetoservospeed.m_dCalculateDeltaX;
						if (m_dWSet* (CMagnativetoservospeed.m_dCalculateDeltaAlpha)<0)
							m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;

					}
					else if ((m_dVxSet != m_nMaxSpeed) && (m_dWSet == m_nMaxW))
					{
						m_dVxSet += CMagnativetoservospeed.m_dCalculateDeltaX;
						if(m_dWSet* (CMagnativetoservospeed.m_dCalculateDeltaAlpha)<0)
							m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
					}
					else if ((m_dVxSet == m_nMaxSpeed) && (m_dWSet != m_nMaxW))
					{
						m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
						if (m_dVxSet*(CMagnativetoservospeed.m_dCalculateDeltaX)<0)
							m_dVxSet += CMagnativetoservospeed.m_dCalculateDeltaX;
					}
					else
					{
						m_dVxSet += CMagnativetoservospeed.m_dCalculateDeltaX;
						m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
					}
					//�����ٶ�����
					SpeedUpperBound();
					
					CalculateFourServoSpeed(m_dVxSet, m_dVySet, m_dWSet);//�����ĸ�����ٶ�

					TransformToPlc(Cplccommunication);//����plc��reply�Ĵ���
					Cplccommunication.SendServoVelocityToPlc(0);//����plc
					Crfidcommunication.GetRfidTagInfo(1);
					if (Crfidcommunication.m_dwRfidTagInfo[3] == Ctaskdefine.m_ppnTaskTable[nTasknum][1])//������һ�ڵ�ʱ����
						break;
				}
				break;
			}
			case FORWARD_BACK://����
			{
				m_dVxSet = 0;
				m_dVySet = -m_nMaxSpeed;
				m_dWSet = 0;
				double dMN1, dMN2;
				while (1)
				{
					dMN1 = CMagnetic_navigation.GetMagneticoffset(1, 0);
					//dMN2 = CMagnetic_navigation.GetMagneticoffset(3, 0);
					dMN2 = 0;
					CMagnativetoservospeed.CalculateDeltaSpeed(dMN1, dMN2, 0);//������ͨpid
					if ((m_dVxSet == m_nMaxSpeed) && (m_dWSet == m_nMaxW))//���ֿ�����
					{
						if (m_dVxSet*(CMagnativetoservospeed.m_dCalculateDeltaX)<0)
							m_dVxSet += CMagnativetoservospeed.m_dCalculateDeltaX;
						if (m_dWSet* (CMagnativetoservospeed.m_dCalculateDeltaAlpha)<0)
							m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;

					}
					else if ((m_dVxSet != m_nMaxSpeed) && (m_dWSet == m_nMaxW))
					{
						m_dVxSet += CMagnativetoservospeed.m_dCalculateDeltaX;
						if (m_dWSet* (CMagnativetoservospeed.m_dCalculateDeltaAlpha)<0)
							m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
					}
					else if ((m_dVxSet == m_nMaxSpeed) && (m_dWSet != m_nMaxW))
					{
						m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
						if (m_dVxSet*(CMagnativetoservospeed.m_dCalculateDeltaX)<0)
							m_dVxSet += CMagnativetoservospeed.m_dCalculateDeltaX;
					}
					else
					{
						m_dVxSet += CMagnativetoservospeed.m_dCalculateDeltaX;
						m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
					}
					//�����ٶ�����
					SpeedUpperBound();
					CalculateFourServoSpeed(m_dVxSet, m_dVySet, m_dWSet);//�����ĸ�����ٶ�
					TransformToPlc(Cplccommunication);//����plc��reply�Ĵ���
					Cplccommunication.SendServoVelocityToPlc(0);//����plc
					Crfidcommunication.GetRfidTagInfo(1);
					if (Crfidcommunication.m_dwRfidTagInfo[3] == Ctaskdefine.m_ppnTaskTable[nTasknum][1])//������һ�ڵ�ʱ����
						break;
				}
				break;
			}
			case FORWARD_LEFT://����
			{
				m_dVxSet = -m_nMaxSpeed;
				m_dVySet = 0;
				m_dWSet = 0;
				double dMN1, dMN2;
				while (1)
				{
					dMN1 = CMagnetic_navigation.GetMagneticoffset(2, 0);
					//dMN2 = CMagnetic_navigation.GetMagneticoffset(4, 0);
					dMN2 = 0;
					CMagnativetoservospeed.CalculateDeltaSpeed(dMN1, dMN2, 0);//������ͨpid
					if ((m_dVySet == m_nMaxSpeed) && (m_dWSet == m_nMaxW))//���ֿ�����
					{
						if (m_dVySet*(CMagnativetoservospeed.m_dCalculateDeltaX)<0)
							m_dVySet += CMagnativetoservospeed.m_dCalculateDeltaX;
						if (m_dWSet* (CMagnativetoservospeed.m_dCalculateDeltaAlpha)<0)
							m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;

					}
					else if ((m_dVySet != m_nMaxSpeed) && (m_dWSet == m_nMaxW))
					{
						m_dVySet += CMagnativetoservospeed.m_dCalculateDeltaX;
						if (m_dWSet* (CMagnativetoservospeed.m_dCalculateDeltaAlpha)<0)
							m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
					}
					else if ((m_dVySet == m_nMaxSpeed) && (m_dWSet != m_nMaxW))
					{
						m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
						if (m_dVySet*(CMagnativetoservospeed.m_dCalculateDeltaX)<0)
							m_dVySet += CMagnativetoservospeed.m_dCalculateDeltaX;
					}
					else
					{
						m_dVySet += CMagnativetoservospeed.m_dCalculateDeltaX;
						m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
					}
					//�����ٶ�����
					SpeedUpperBound();
					CalculateFourServoSpeed(m_dVxSet, m_dVySet, m_dWSet);//�����ĸ�����ٶ�
					TransformToPlc(Cplccommunication);//����plc��reply�Ĵ���
					Cplccommunication.SendServoVelocityToPlc(0);//����plc
					Crfidcommunication.GetRfidTagInfo(1);
					if (Crfidcommunication.m_dwRfidTagInfo[3] == Ctaskdefine.m_ppnTaskTable[nTasknum][1])//������һ�ڵ�ʱ����
						break;
				}
				break;
			}
			case FORWARD_RIGHT://����
			{
				m_dVxSet = m_nMaxSpeed;
				m_dVySet = 0;
				m_dWSet = 0;
				double dMN1, dMN2;
				while (1)
				{
					dMN1 = CMagnetic_navigation.GetMagneticoffset(2, 0);
					//dMN2 = CMagnetic_navigation.GetMagneticoffset(4, 0);
					dMN2 = 0;
					CMagnativetoservospeed.CalculateDeltaSpeed(dMN1, dMN2, 0);//������ͨpid
					if ((m_dVySet == m_nMaxSpeed) && (m_dWSet == m_nMaxW))//���ֿ�����
					{
						if (m_dVySet*(CMagnativetoservospeed.m_dCalculateDeltaX)<0)
							m_dVySet += CMagnativetoservospeed.m_dCalculateDeltaX;
						if (m_dWSet* (CMagnativetoservospeed.m_dCalculateDeltaAlpha)<0)
							m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;

					}
					else if ((m_dVySet != m_nMaxSpeed) && (m_dWSet == m_nMaxW))
					{
						m_dVySet += CMagnativetoservospeed.m_dCalculateDeltaX;
						if (m_dWSet* (CMagnativetoservospeed.m_dCalculateDeltaAlpha)<0)
							m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
					}
					else if ((m_dVySet == m_nMaxSpeed) && (m_dWSet != m_nMaxW))
					{
						m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
						if (m_dVySet*(CMagnativetoservospeed.m_dCalculateDeltaX)<0)
							m_dVySet += CMagnativetoservospeed.m_dCalculateDeltaX;
					}
					else
					{
						m_dVySet += CMagnativetoservospeed.m_dCalculateDeltaX;
						m_dWSet += CMagnativetoservospeed.m_dCalculateDeltaAlpha;
					}
					//�����ٶ�����
					SpeedUpperBound();
					CalculateFourServoSpeed(m_dVxSet, m_dVySet, m_dWSet);//�����ĸ�����ٶ�
					TransformToPlc(Cplccommunication);//����plc��reply�Ĵ���
					Cplccommunication.SendServoVelocityToPlc(0);//����plc
					Crfidcommunication.GetRfidTagInfo(1);
					if (Crfidcommunication.m_dwRfidTagInfo[3] == Ctaskdefine.m_ppnTaskTable[nTasknum][1])//������һ�ڵ�ʱ����
						break;
				}
				break;
			}
			default:
				break;
			}
			return 100;
	}
	return m_dwStartRfidFlag;
}
void CTaskDealing::CalculateFourServoSpeed(const double c_dVx, const double c_dVy, const double c_dw)
{
	//������ʽ//1  4\\   ��˳ʱ��Ϊ������yΪ��ǰ����xΪ����
	//        \\2  3//
	//ת��Ϊ���ֵ�ת��,��λrad/min
	m_dW1 = (1 / m_dR)*(c_dVx + c_dVy) + (m_dW + m_dL)*c_dw / (m_dR);
	m_dW2 = (1 / m_dR)*(-c_dVx + c_dVy) + (m_dW + m_dL)*c_dw / (m_dR);
	m_dW3 = (1 / m_dR)*(c_dVx + c_dVy) - (m_dW + m_dL)*c_dw / (m_dR);
	m_dW4 = (1 / m_dR)*(-c_dVx + c_dVy) - (m_dW + m_dL)*c_dw / (m_dR);
	m_dRotateTime = 0;
}

#endif // !_TASKDEALING_H_