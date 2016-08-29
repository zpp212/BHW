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
	void SpeedUpperBound();//设置速度上限
	uint16_t GetSettedSpeed(int&,CRfidCommunication&,CTaskDefine&,CPLCCommunication&, CMagnetic_Navigation&, CMagnativetoServoSpeed&);
	//计算四个电机速度,执行当前节点任务
	void CalculateFourServoSpeed(const double c_dVx, const double c_dVy, const double c_dw);//参数为设置的电机速度
	void RotateControl(int, CPLCCommunication&);
	void TransformToPlc(CPLCCommunication&);//将数据传给与plc通讯的寄存器。单位角速度rad/min,时间单位：毫秒
private:
	double m_nMaxSpeed,m_nMaxW;//额定最大速度
	int m_nDirection;//顺时针：0,1,2,3
	uint16_t m_dwStartRfidFlag;
    double m_dVxSet, m_dVySet, m_dWSet;//以小车坐标系为基准，Vy为小车直行方向，W是角速度
	double m_dR, m_dW, m_dL;//R车轮半径，W车宽，L车长
	double m_dW1, m_dW2, m_dW3, m_dW4;//四个轮子的角速度
	double m_dRotateTime;//旋转时间,单位秒
};

CTaskDealing::CTaskDealing()
{
	//车身参数初始化
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
void CTaskDealing::RotateControl(int nAngleLable, CPLCCommunication&Cplccommunication)//正1为右转90度
{
	uint16_t dw;//旋转车体角速度
	dw = 36 / (m_dL + m_dW);//w=2Rw1/(W+L)，其中Rw1=m_nMaxSpeedm/min
	m_dRotateTime = abs(nAngleLable)*(PI / (2 * dw)) * 60;//单位：秒
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
	}//旋转时间内，每0.1s给plc发送相同数据
	
}
uint16_t CTaskDealing::GetSettedSpeed(int&nTasknum, CRfidCommunication&Crfidcommunication, CTaskDefine&Ctaskdefine,CPLCCommunication&Cplccommunication, CMagnetic_Navigation&CMagnetic_navigation, CMagnativetoServoSpeed&CMagnativetoservospeed)
{
		Crfidcommunication.GetRfidTagInfo(1);
		m_dwStartRfidFlag = Crfidcommunication.m_dwRfidTagInfo[3];//一般一个16位的数据就够用了
		if ((m_dwStartRfidFlag == Ctaskdefine.m_ppnTaskTable[nTasknum][0]&&nTasknum &&m_dwStartRfidFlag== Ctaskdefine.m_ppnTaskTable[nTasknum-1][1])|| (!nTasknum && (m_dwStartRfidFlag == Ctaskdefine.m_ppnTaskTable[nTasknum][0])))
			//满足条件：首节点，当前节点吻合任务节点；非首节点，当前节点吻合当前节点及前一任务的目标节点
		{
			//任务优先级：当前节点的旋转4、当前节点任务2、前进方向3
			switch (Ctaskdefine.m_ppnTaskTable[nTasknum][4])
			{//执行旋转动作，标记车体相对位置
			case ROTATE_NONE://无旋转
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
			{	case JOINTTASK_NONE://无任务
				{
					break;
				}
				case JOINTTASK_PARK://停车
				{
					m_dVxSet = m_dVySet = m_dWSet = 0;
					
					break;
				}
				case JOINTTASK_LOADING://装货
				{
					break;
				};
				case JOINTTASK_UNLOAD ://卸货
				{
					break;
				};
				case JOINTTASK_CHARGE://充电
				{
					break;
				}
				default:
					break;
			}
			switch (Ctaskdefine.m_ppnTaskTable[nTasknum][3])
			{
			case FORWARD_NONE://无行进
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
				}//根据具体停车时间决定
				break;
			}
			case FORWARD_FRONT://前进
			{
				m_dVxSet = 0;
				m_dVySet = m_nMaxSpeed;
				m_dWSet = 0;
				uint16_t dMN1, dMN2;
				while (1)
				{
					dMN1 = CMagnetic_navigation.GetMagneticoffset(1, 0);
					//dMN2 = CMagnetic_navigation.GetMagneticoffset(3, 0);//暂时没接第二个
					dMN2 = 0;
					CMagnativetoservospeed.CalculateDeltaSpeed(dMN1, dMN2, 0);//先用普通pid
					if ((m_dVxSet == m_nMaxSpeed)&&(m_dWSet == m_nMaxW))//积分抗饱和
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
					//设置速度上限
					SpeedUpperBound();
					
					CalculateFourServoSpeed(m_dVxSet, m_dVySet, m_dWSet);//计算四个电机速度

					TransformToPlc(Cplccommunication);//发到plc的reply寄存器
					Cplccommunication.SendServoVelocityToPlc(0);//发给plc
					Crfidcommunication.GetRfidTagInfo(1);
					if (Crfidcommunication.m_dwRfidTagInfo[3] == Ctaskdefine.m_ppnTaskTable[nTasknum][1])//到达下一节点时跳出
						break;
				}
				break;
			}
			case FORWARD_BACK://后退
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
					CMagnativetoservospeed.CalculateDeltaSpeed(dMN1, dMN2, 0);//先用普通pid
					if ((m_dVxSet == m_nMaxSpeed) && (m_dWSet == m_nMaxW))//积分抗饱和
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
					//设置速度上限
					SpeedUpperBound();
					CalculateFourServoSpeed(m_dVxSet, m_dVySet, m_dWSet);//计算四个电机速度
					TransformToPlc(Cplccommunication);//发到plc的reply寄存器
					Cplccommunication.SendServoVelocityToPlc(0);//发给plc
					Crfidcommunication.GetRfidTagInfo(1);
					if (Crfidcommunication.m_dwRfidTagInfo[3] == Ctaskdefine.m_ppnTaskTable[nTasknum][1])//到达下一节点时跳出
						break;
				}
				break;
			}
			case FORWARD_LEFT://左行
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
					CMagnativetoservospeed.CalculateDeltaSpeed(dMN1, dMN2, 0);//先用普通pid
					if ((m_dVySet == m_nMaxSpeed) && (m_dWSet == m_nMaxW))//积分抗饱和
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
					//设置速度上限
					SpeedUpperBound();
					CalculateFourServoSpeed(m_dVxSet, m_dVySet, m_dWSet);//计算四个电机速度
					TransformToPlc(Cplccommunication);//发到plc的reply寄存器
					Cplccommunication.SendServoVelocityToPlc(0);//发给plc
					Crfidcommunication.GetRfidTagInfo(1);
					if (Crfidcommunication.m_dwRfidTagInfo[3] == Ctaskdefine.m_ppnTaskTable[nTasknum][1])//到达下一节点时跳出
						break;
				}
				break;
			}
			case FORWARD_RIGHT://右行
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
					CMagnativetoservospeed.CalculateDeltaSpeed(dMN1, dMN2, 0);//先用普通pid
					if ((m_dVySet == m_nMaxSpeed) && (m_dWSet == m_nMaxW))//积分抗饱和
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
					//设置速度上限
					SpeedUpperBound();
					CalculateFourServoSpeed(m_dVxSet, m_dVySet, m_dWSet);//计算四个电机速度
					TransformToPlc(Cplccommunication);//发到plc的reply寄存器
					Cplccommunication.SendServoVelocityToPlc(0);//发给plc
					Crfidcommunication.GetRfidTagInfo(1);
					if (Crfidcommunication.m_dwRfidTagInfo[3] == Ctaskdefine.m_ppnTaskTable[nTasknum][1])//到达下一节点时跳出
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
	//轮子样式//1  4\\   以顺时针为正方向，y为正前方，x为右向
	//        \\2  3//
	//转换为四轮的转速,单位rad/min
	m_dW1 = (1 / m_dR)*(c_dVx + c_dVy) + (m_dW + m_dL)*c_dw / (m_dR);
	m_dW2 = (1 / m_dR)*(-c_dVx + c_dVy) + (m_dW + m_dL)*c_dw / (m_dR);
	m_dW3 = (1 / m_dR)*(c_dVx + c_dVy) - (m_dW + m_dL)*c_dw / (m_dR);
	m_dW4 = (1 / m_dR)*(-c_dVx + c_dVy) - (m_dW + m_dL)*c_dw / (m_dR);
	m_dRotateTime = 0;
}

#endif // !_TASKDEALING_H_