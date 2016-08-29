#pragma once
#ifndef _MN_PID_H
#define _MN_PID_H
#include "Cfuzzypid_8.17.h"
class CMagnativetoServoSpeed
{
	friend class CTaskDealing;
public:
	CMagnativetoServoSpeed();
	~CMagnativetoServoSpeed();
	//�ɵ�ǰx���򼰽Ƕ�ƫ�����delta_x��delta_alpha
	void CalculateX_Station();//Xλ����pid
	void CalculateX_Increment();//X������pid
	void CalculateA_Station();//alphaλ����pid
	void CalculateA_Increment();//alpha������pid
	//����delta_vx��delta_vw
	void CalculateDeltaSpeed(const double c_fMagNav1,const double c_fMagNav2,int nPidFlag);//nPidFlag 1) 0:������ͨpid  2) 1:Fuzzy PID
	//����deltaPIDϵ��
	void Tuning_Pid(CFuzzyPid *pCDlet_PID, const double c_fE, const double c_fEc,double(&dCalDeltaPid)[3]);
	
private:
	double m_pd_SetDeltaX;	//input
	double m_pd_SetDeltaAlpha;	//input
	double m_pd_PresentDeltaX;	//input
	double m_pd_PresentDeltaAlpha;	//input
	double m_dCalculateDeltaX, m_dCalculateDeltaAlpha, m_dDeltaXIncrement,m_dDeltaAlphaIncrement;	//output
	double m_dkpx, m_dkdx, m_dkix, m_dkpa, m_dkda, m_dkia;
	double m_derrx, m_derr2x, m_derr2_beforex, m_derr_sumx, m_derr_diffx;
	double m_derra, m_derr2a, m_derr2_beforea, m_derr_suma, m_derr_diffa;
	CFuzzyPid m_CDlet_PID;
	double m_dCalDltaPidx[3] = {0,0,0};
	double m_dCalDltaPida[3] = { 0,0,0 };
	
};


#endif // !_MN_PID_H