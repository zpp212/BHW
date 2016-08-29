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
	//由当前x方向及角度偏差，计算delta_x和delta_alpha
	void CalculateX_Station();//X位置型pid
	void CalculateX_Increment();//X增量型pid
	void CalculateA_Station();//alpha位置型pid
	void CalculateA_Increment();//alpha增量型pid
	//计算delta_vx和delta_vw
	void CalculateDeltaSpeed(const double c_fMagNav1,const double c_fMagNav2,int nPidFlag);//nPidFlag 1) 0:采用普通pid  2) 1:Fuzzy PID
	//计算deltaPID系数
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