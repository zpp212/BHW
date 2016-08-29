#include"CMN_PID.h"
CMagnativetoServoSpeed::CMagnativetoServoSpeed()
{
}
CMagnativetoServoSpeed::~CMagnativetoServoSpeed()
{
}
void CMagnativetoServoSpeed::CalculateX_Station()
{// PID Calculation
	m_derrx = m_pd_SetDeltaX - m_pd_PresentDeltaX;
	m_derr_sumx += m_derrx;
	m_dCalculateDeltaX = m_dkpx*m_derrx + m_dkix*(m_derr_sumx)+m_dkdx* (m_derr2x - m_derrx);//位置型
	m_derr2x = m_derrx;
}
void CMagnativetoServoSpeed::CalculateA_Station()
{// PID Calculation
	m_derra = m_pd_SetDeltaAlpha - m_pd_PresentDeltaAlpha;
	m_derr_suma += m_derra;
	m_dCalculateDeltaAlpha = m_dkpa*m_derra + m_dkia*(m_derr_suma)+m_dkda* (m_derr2a - m_derra);//位置型
	m_derr2a = m_derra;
}
void CMagnativetoServoSpeed::CalculateX_Increment()
{
	// PID Calculation
	//m_derrx = (m_pd_SetDeltaX) - m_pd_PresentDeltaX;
	m_dDeltaXIncrement = m_dkpx*(m_derrx - m_derr2x) + m_dkix*m_derrx + m_dkdx* (m_derrx - 2 * m_derr2x + m_derr2_beforex);//增量型
	m_dCalculateDeltaX += m_dDeltaXIncrement;
	m_derr2_beforex = m_derr2x;
	m_derr2x = m_derrx;
}
void CMagnativetoServoSpeed::CalculateA_Increment()
{// PID Calculation
 //m_derra = (m_pd_SetDeltaAlpha) - m_pd_PresentDeltaAlpha;
	m_dDeltaAlphaIncrement = m_dkpa*(m_derra - m_derr2a) + m_dkia*m_derra + m_dkda* (m_derra - 2 * m_derr2a + m_derr2_beforea);//增量型
	m_dCalculateDeltaAlpha += m_dDeltaAlphaIncrement;
	m_derr2_beforea = m_derr2a;
	m_derr2a = m_derra;
}
void CMagnativetoServoSpeed::CalculateDeltaSpeed(const double fMagNav1, const double fMagNav2, int nPidFlag)
{
	//转换为delta_x,delta_alpha（磁传感器背对安装）
	m_pd_SetDeltaX = 12;
	m_pd_SetDeltaAlpha = 0;
	m_pd_PresentDeltaX = (fMagNav1 + fMagNav2) / 2;//反应中心位置的偏差
	m_pd_PresentDeltaAlpha = fMagNav1 - fMagNav2;//差值反应角度
	m_derrx = m_pd_SetDeltaX - m_pd_PresentDeltaX;
	m_derra = m_pd_SetDeltaAlpha - m_pd_PresentDeltaAlpha;
	m_derr_diffx = m_derrx - m_derr2x;
	m_derr_diffa = m_derra - m_derr2a;
	//增量型pid计算下一次的位置
	if (nPidFlag == 1)
	{
		Tuning_Pid(&m_CDlet_PID, m_derrx, m_derrx, m_dCalDltaPidx);
		m_dkpx += m_dCalDltaPidx[0];
		m_dkix += m_dCalDltaPidx[1];
		m_dkdx += m_dCalDltaPidx[2];
	}
	CalculateX_Increment();//计算得delta_Vx
	CalculateA_Increment();

}
void CMagnativetoServoSpeed::Tuning_Pid(CFuzzyPid *pCDlet_PID, const double c_fE, const double c_fEc, double(&dCalDltaPid)[3])
{
	dCalDltaPid[0] = pCDlet_PID->m_Getdelta_kp(c_fE, c_fEc);
	dCalDltaPid[1] = pCDlet_PID->m_Getdelta_ki(c_fE, c_fEc);
	dCalDltaPid[2] = pCDlet_PID->m_Getdelta_kd(c_fE, c_fEc);
}
