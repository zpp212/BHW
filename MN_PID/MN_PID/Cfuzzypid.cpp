#include "Cfuzzypid_8.17.h"
CFuzzyPid::CFuzzyPid()
{

}

CFuzzyPid::~CFuzzyPid()
{
}
double CFuzzyPid::m_uTrim(const double c_fX1, const double c_fX2, const double c_fX3, const double c_fX)
{
	if (c_fX <= c_fX1)
	{
		return 0.0f;
	}
	if ((c_fX1<c_fX) && (c_fX <= c_fX2))
	{
		return (c_fX - c_fX1) / (c_fX2 - c_fX1);
	}
	if ((c_fX2<c_fX) && (c_fX<c_fX3))
	{
		return (c_fX3 - c_fX) / (c_fX3 - c_fX2);
	}
	if (c_fX >= c_fX3)
	{
		return 0.0f;
	}
	return 0.0f;
}
double CFuzzyPid::m_uZfiml(const double c_fX1, const double c_fX2, const double c_fX)
{
	if (c_fX <= c_fX1)
	{
		return 1.0f;
	}
	if ((c_fX1<c_fX) && (c_fX<c_fX2))
	{
		return (c_fX - c_fX1) / (c_fX2 - c_fX1);
	}
	if (c_fX >= c_fX2)
	{
		return 0.0f;
	}
	return 0.0f;
}
double CFuzzyPid::m_uZfimr(const double c_fX1, const double c_fX2, const double c_fX)
{
	if (c_fX <= c_fX1)
	{
		return 0.0f;
	}
	if ((c_fX1<c_fX) && (c_fX<c_fX2))
	{
		return (c_fX2 - c_fX) / (c_fX2 - c_fX1);
	}
	if (c_fX >= c_fX2)
	{
		return 1.0f;
	}
	return 1.0f;
}
double CFuzzyPid::m_decode_uTrim(const double c_fX1, const double c_fX2, const double c_fX3, const double c_fX)
{
	double fret_val = 0;/*用返回最大的值*/
	if (c_fX == 0)
	{
		return c_fX3;
	}
	if (c_fX == 1.0f)
	{
		return c_fX2;
	}
	fret_val = c_fX3 - c_fX*(c_fX3 - c_fX2);
	return fret_val;
}
double CFuzzyPid::m_decode_uZfiml(const double c_fX1, const double c_fX2, const double c_fX)
{
	if (c_fX >= 1.0f)
	{
		return c_fX1;
	}
	if (c_fX <= 0.0f)
	{
		return c_fX2;
	}
	return c_fX2 - (c_fX2 - c_fX1)*c_fX;
}
double CFuzzyPid::m_decode_uZfimr(const double c_fX1, const double c_fX2, const double c_fX)
{
	if (c_fX >= 1.0f)
	{
		return c_fX2;
	}
	if (c_fX <= 0.0f)
	{
		return c_fX1;
	}
	return c_fX1 + (c_fX2 - c_fX1)*c_fX;
}
double CFuzzyPid::m_Getdelta_kp(const double fe_in, const double fec_in)
{
	double fKp_m_ve = 0;
	double fKp_m_vec = 0;
	double fRet_val = 0;
	char cKp_NZPe = 0;
	char cKp_NZPec = 0;
	char cDelt_kp_ind = 0;
	//计算e在各模糊语言下的隶属度
	if (fe_in <= m_fE_table[NB][0])
	{
		m_fNB_kpe = 1.0f;
	}
	if ((m_fE_table[NB][0]<fe_in) && (fe_in<m_fE_table[NB][1]))
	{
		m_fNB_kpe = m_uZfiml(m_fE_table[NB][0], m_fE_table[NB][1], fe_in);
		m_fNM_kpe = m_uTrim(m_fE_table[NM][0], m_fE_table[NM][1], m_fE_table[NM][2], fe_in);
	}
	if ((m_fE_table[NB][1]<fe_in) && (fe_in<m_fE_table[NM][2]))
	{
		m_fNM_kpe = m_uTrim(m_fE_table[NM][0], m_fE_table[NM][1], m_fE_table[NM][2], fe_in);
		m_fNS_kpe = m_uTrim(m_fE_table[NS][0], m_fE_table[NS][1], m_fE_table[NS][2], fe_in);
	}
	if ((m_fE_table[NM][2]<fe_in) && (fe_in<m_fE_table[NS][2]))
	{
		m_fNS_kpe = m_uTrim(m_fE_table[NS][0], m_fE_table[NS][1], m_fE_table[NS][2], fe_in);
		m_fZO_kpe = m_uTrim(m_fE_table[ZO][0], m_fE_table[ZO][1], m_fE_table[ZO][2], fe_in);
	}
	if ((m_fE_table[NS][2]<fe_in) && (fe_in<m_fE_table[ZO][2]))
	{
		m_fZO_kpe = m_uTrim(m_fE_table[ZO][0], m_fE_table[ZO][1], m_fE_table[ZO][2], fe_in);
		m_fPS_kpe = m_uTrim(m_fE_table[PS][0], m_fE_table[PS][1], m_fE_table[PS][2], fe_in);
	}
	if ((m_fE_table[ZO][2]<fe_in) && (fe_in<m_fE_table[PS][2]))
	{
		m_fPS_kpe = m_uTrim(m_fE_table[PS][0], m_fE_table[PS][1], m_fE_table[PS][2], fe_in);
		m_fPM_kpe = m_uTrim(m_fE_table[PM][0], m_fE_table[PM][1], m_fE_table[PM][2], fe_in);
	}
	if ((m_fE_table[PS][2]<fe_in) && (fe_in<m_fE_table[PM][2]))
	{
		m_fPM_kpe = m_uTrim(m_fE_table[PM][0], m_fE_table[PM][1], m_fE_table[PM][2], fe_in);
		m_fPB_kpe = m_uZfimr(m_fE_table[PB][0], m_fE_table[PB][1], fe_in);
	}
	if ((m_fE_table[PM][2]<fe_in) && (fe_in<m_fE_table[PB][1]))
	{
		m_fPB_kpe = m_uZfimr(m_fE_table[PB][0], m_fE_table[PB][1], fe_in);
	}
	if (fe_in>m_fE_table[PB][1])
	{
		m_fPB_kpe = 1.0f;
	}
	//求取e隶属度最大的
	if (m_fNB_kpe >= fKp_m_ve)
	{
		fKp_m_ve = m_fNB_kpe;
		cKp_NZPe = NB;
	}
	if (m_fNM_kpe >= fKp_m_ve)
	{
		fKp_m_ve = m_fNM_kpe;
		cKp_NZPe = NM;
	}
	if (m_fNS_kpe >= fKp_m_ve)
	{
		fKp_m_ve = m_fNS_kpe;
		cKp_NZPe = NS;
	}
	if (m_fZO_kpe >= fKp_m_ve)
	{
		fKp_m_ve = m_fZO_kpe;
		cKp_NZPe = ZO;
	}
	if (m_fPS_kpe >= fKp_m_ve)
	{
		fKp_m_ve = m_fPS_kpe;
		cKp_NZPe = PS;
	}
	if (m_fPM_kpe >= fKp_m_ve)
	{
		fKp_m_ve = m_fPM_kpe;
		cKp_NZPe = PM;
	}
	if (m_fPB_kpe >= fKp_m_ve)
	{
		fKp_m_ve = m_fPB_kpe;
		cKp_NZPe = PB;
	}
	//计算ec在各模糊语言下的隶属度
	if (fec_in <= m_fEc_table[NB][0])
	{
		m_fNB_kpec = 1.0f;
	}
	if ((m_fEc_table[NB][0]<fec_in) && (fec_in<m_fEc_table[NB][1]))
	{
		m_fNB_kpec = m_uZfiml(m_fEc_table[NB][0], m_fEc_table[NB][1], fec_in);
		m_fNM_kpec = m_uTrim(m_fEc_table[NM][0], m_fEc_table[NM][1], m_fEc_table[NM][2], fec_in);
	}
	if ((m_fEc_table[NB][1]<fec_in) && (fec_in<m_fEc_table[NM][2]))
	{
		m_fNM_kpec = m_uTrim(m_fEc_table[NM][0], m_fEc_table[NM][1], m_fEc_table[NM][2], fec_in);
		m_fNS_kpec = m_uTrim(m_fEc_table[NS][0], m_fEc_table[NS][1], m_fEc_table[NS][2], fec_in);
	}
	if ((m_fEc_table[NM][2]<fec_in) && (fec_in<m_fEc_table[NS][2]))
	{
		m_fNS_kpec = m_uTrim(m_fEc_table[NS][0], m_fEc_table[NS][1], m_fEc_table[NS][2], fec_in);
		m_fZO_kpec = m_uTrim(m_fEc_table[ZO][0], m_fEc_table[ZO][1], m_fEc_table[ZO][2], fec_in);
	}
	if ((m_fEc_table[NS][2]<fec_in) && (fec_in<m_fEc_table[ZO][2]))
	{
		m_fZO_kpec = m_uTrim(m_fEc_table[ZO][0], m_fEc_table[ZO][1], m_fEc_table[ZO][2], fec_in);
		m_fPS_kpec = m_uTrim(m_fEc_table[PS][0], m_fEc_table[PS][1], m_fEc_table[PS][2], fec_in);
	}
	if ((m_fEc_table[ZO][2]<fec_in) && (fec_in<m_fEc_table[PS][2]))
	{
		m_fPS_kpec = m_uTrim(m_fEc_table[PS][0], m_fEc_table[PS][1], m_fEc_table[PS][2], fec_in);
		m_fPM_kpec = m_uTrim(m_fEc_table[PM][0], m_fEc_table[PM][1], m_fEc_table[PM][2], fec_in);
	}
	if ((m_fEc_table[PS][2]<fec_in) && (fec_in<m_fEc_table[PM][2]))
	{
		m_fPM_kpec = m_uTrim(m_fEc_table[PM][0], m_fEc_table[PM][1], m_fEc_table[PM][2], fec_in);
		m_fPB_kpec = m_uZfimr(m_fEc_table[PB][0], m_fEc_table[PB][1], fec_in);
	}
	if ((m_fEc_table[PM][2]<fec_in) && (fec_in<m_fEc_table[PB][1]))
	{
		m_fPB_kpec = m_uZfimr(m_fEc_table[PB][0], m_fEc_table[PB][1], fec_in);
	}
	if (fec_in>m_fEc_table[PB][1])
	{
		m_fPB_kpec = 1.0f;
	}

	//求取ec隶属度最大的
	if (m_fNB_kpec >= fKp_m_vec)
	{
		fKp_m_vec = m_fNB_kpec;
		cKp_NZPec = NB;
	}
	if (m_fNM_kpec >= fKp_m_vec)
	{
		fKp_m_vec = m_fNM_kpec;
		cKp_NZPec = NM;
	}
	if (m_fNS_kpec >= fKp_m_vec)
	{
		fKp_m_vec = m_fNS_kpec;
		cKp_NZPec = NS;
	}
	if (m_fZO_kpec >= fKp_m_vec)
	{
		fKp_m_vec = m_fZO_kpec;
		cKp_NZPec = ZO;
	}
	if (m_fPS_kpec >= fKp_m_vec)
	{
		fKp_m_vec = m_fPS_kpec;
		cKp_NZPec = PS;
	}
	if (m_fPM_kpec >= fKp_m_vec)
	{
		fKp_m_vec = m_fPM_kpec;
		cKp_NZPec = PM;
	}
	if (m_fPB_kpec >= fKp_m_vec)
	{
		fKp_m_vec = m_fPB_kpec;
		cKp_NZPec = PB;
	}
	//求取delt_kp,去模糊化
	cDelt_kp_ind = m_cDelt_kp_index[cKp_NZPe][cKp_NZPec];
	if (cDelt_kp_ind == NB)
	{
		fRet_val = m_decode_uZfiml(m_fDelt_kp_table[NB][0], m_fDelt_kp_table[NB][1], max(cKp_NZPe, cKp_NZPec));
		return fRet_val;
	}
	if (cDelt_kp_ind == PB)
	{
		fRet_val = m_decode_uZfimr(m_fDelt_kp_table[PB][0], m_fDelt_kp_table[PB][1], max(cKp_NZPe, cKp_NZPec));
		return fRet_val;
	}

	fRet_val = m_decode_uTrim(m_fDelt_kp_table[cDelt_kp_ind][0], m_fDelt_kp_table[cDelt_kp_ind][1], m_fDelt_kp_table[cDelt_kp_ind][2], max(cKp_NZPe, cKp_NZPec));
	return fRet_val;
}
double CFuzzyPid::m_Getdelta_ki(const double fe_in, const double fec_in)
{
	double fKi_m_ve = 0;
	double fKi_m_vec = 0;
	double fRet_val = 0;
	char cKi_NZPe = 0;
	char cKi_NZPec = 0;
	char cDelt_ki_ind = 0;
	//计算e在各模糊语言下的隶属度
	if (fe_in <= m_fE_table[NB][0])
	{
		m_fNB_kie = 1.0f;
	}
	if ((m_fE_table[NB][0]<fe_in) && (fe_in<m_fE_table[NB][1]))
	{
		m_fNB_kie = m_uZfiml(m_fE_table[NB][0], m_fE_table[NB][1], fe_in);
		m_fNM_kie = m_uTrim(m_fE_table[NM][0], m_fE_table[NM][1], m_fE_table[NM][2], fe_in);
	}
	if ((m_fE_table[NB][1]<fe_in) && (fe_in<m_fE_table[NM][2]))
	{
		m_fNM_kie = m_uTrim(m_fE_table[NM][0], m_fE_table[NM][1], m_fE_table[NM][2], fe_in);
		m_fNS_kie = m_uTrim(m_fE_table[NS][0], m_fE_table[NS][1], m_fE_table[NS][2], fe_in);
	}
	if ((m_fE_table[NM][2]<fe_in) && (fe_in<m_fE_table[NS][2]))
	{
		m_fNS_kie = m_uTrim(m_fE_table[NS][0], m_fE_table[NS][1], m_fE_table[NS][2], fe_in);
		m_fZO_kie = m_uTrim(m_fE_table[ZO][0], m_fE_table[ZO][1], m_fE_table[ZO][2], fe_in);
	}
	if ((m_fE_table[NS][2]<fe_in) && (fe_in<m_fE_table[ZO][2]))
	{
		m_fZO_kie = m_uTrim(m_fE_table[ZO][0], m_fE_table[ZO][1], m_fE_table[ZO][2], fe_in);
		m_fPS_kie = m_uTrim(m_fE_table[PS][0], m_fE_table[PS][1], m_fE_table[PS][2], fe_in);
	}
	if ((m_fE_table[ZO][2]<fe_in) && (fe_in<m_fE_table[PS][2]))
	{
		m_fPS_kie = m_uTrim(m_fE_table[PS][0], m_fE_table[PS][1], m_fE_table[PS][2], fe_in);
		m_fPM_kie = m_uTrim(m_fE_table[PM][0], m_fE_table[PM][1], m_fE_table[PM][2], fe_in);
	}
	if ((m_fE_table[PS][2]<fe_in) && (fe_in<m_fE_table[PM][2]))
	{
		m_fPM_kie = m_uTrim(m_fE_table[PM][0], m_fE_table[PM][1], m_fE_table[PM][2], fe_in);
		m_fPB_kie = m_uZfimr(m_fE_table[PB][0], m_fE_table[PB][1], fe_in);
	}
	if ((m_fE_table[PM][2]<fe_in) && (fe_in<m_fE_table[PB][1]))
	{
		m_fPB_kie = m_uZfimr(m_fE_table[PB][0], m_fE_table[PB][1], fe_in);
	}
	if (fe_in>m_fE_table[PB][1])
	{
		m_fPB_kie = 1.0f;
	}
	//求取e隶属度最大的
	if (m_fNB_kie >= fKi_m_ve)
	{
		fKi_m_ve = m_fNB_kie;
		cKi_NZPe = NB;
	}
	if (m_fNM_kie >= fKi_m_ve)
	{
		fKi_m_ve = m_fNM_kie;
		cKi_NZPe = NM;
	}
	if (m_fNS_kie >= fKi_m_ve)
	{
		fKi_m_ve = m_fNS_kie;
		cKi_NZPe = NS;
	}
	if (m_fZO_kie >= fKi_m_ve)
	{
		fKi_m_ve = m_fZO_kie;
		cKi_NZPe = ZO;
	}
	if (m_fPS_kie >= fKi_m_ve)
	{
		fKi_m_ve = m_fPS_kie;
		cKi_NZPe = PS;
	}
	if (m_fPM_kie >= fKi_m_ve)
	{
		fKi_m_ve = m_fPM_kie;
		cKi_NZPe = PM;
	}
	if (m_fPB_kie >= fKi_m_ve)
	{
		fKi_m_ve = m_fPB_kie;
		cKi_NZPe = PB;
	}
	//计算ec在各模糊语言下的隶属度
	if (fec_in <= m_fEc_table[NB][0])
	{
		m_fNB_kiec = 1.0f;
	}
	if ((m_fEc_table[NB][0]<fec_in) && (fec_in<m_fEc_table[NB][1]))
	{
		m_fNB_kiec = m_uZfiml(m_fEc_table[NB][0], m_fEc_table[NB][1], fec_in);
		m_fNM_kiec = m_uTrim(m_fEc_table[NM][0], m_fEc_table[NM][1], m_fEc_table[NM][2], fec_in);
	}
	if ((m_fEc_table[NB][1]<fec_in) && (fec_in<m_fEc_table[NM][2]))
	{
		m_fNM_kiec = m_uTrim(m_fEc_table[NM][0], m_fEc_table[NM][1], m_fEc_table[NM][2], fec_in);
		m_fNS_kiec = m_uTrim(m_fEc_table[NS][0], m_fEc_table[NS][1], m_fEc_table[NS][2], fec_in);
	}
	if ((m_fEc_table[NM][2]<fec_in) && (fec_in<m_fEc_table[NS][2]))
	{
		m_fNS_kiec = m_uTrim(m_fEc_table[NS][0], m_fEc_table[NS][1], m_fEc_table[NS][2], fec_in);
		m_fZO_kiec = m_uTrim(m_fEc_table[ZO][0], m_fEc_table[ZO][1], m_fEc_table[ZO][2], fec_in);
	}
	if ((m_fEc_table[NS][2]<fec_in) && (fec_in<m_fEc_table[ZO][2]))
	{
		m_fZO_kiec = m_uTrim(m_fEc_table[ZO][0], m_fEc_table[ZO][1], m_fEc_table[ZO][2], fec_in);
		m_fPS_kiec = m_uTrim(m_fEc_table[PS][0], m_fEc_table[PS][1], m_fEc_table[PS][2], fec_in);
	}
	if ((m_fEc_table[ZO][2]<fec_in) && (fec_in<m_fEc_table[PS][2]))
	{
		m_fPS_kiec = m_uTrim(m_fEc_table[PS][0], m_fEc_table[PS][1], m_fEc_table[PS][2], fec_in);
		m_fPM_kiec = m_uTrim(m_fEc_table[PM][0], m_fEc_table[PM][1], m_fEc_table[PM][2], fec_in);
	}
	if ((m_fEc_table[PS][2]<fec_in) && (fec_in<m_fEc_table[PM][2]))
	{
		m_fPM_kiec = m_uTrim(m_fEc_table[PM][0], m_fEc_table[PM][1], m_fEc_table[PM][2], fec_in);
		m_fPB_kiec = m_uZfimr(m_fEc_table[PB][0], m_fEc_table[PB][1], fec_in);
	}
	if ((m_fEc_table[PM][2]<fec_in) && (fec_in<m_fEc_table[PB][1]))
	{
		m_fPB_kiec = m_uZfimr(m_fEc_table[PB][0], m_fEc_table[PB][1], fec_in);
	}
	if (fec_in>m_fEc_table[PB][1])
	{
		m_fPB_kiec = 1.0f;
	}

	//求取ec隶属度最大的
	if (m_fNB_kiec >= fKi_m_vec)
	{
		fKi_m_vec = m_fNB_kiec;
		cKi_NZPec = NB;
	}
	if (m_fNM_kiec >= fKi_m_vec)
	{
		fKi_m_vec = m_fNM_kiec;
		cKi_NZPec = NM;
	}
	if (m_fNS_kiec >= fKi_m_vec)
	{
		fKi_m_vec = m_fNS_kiec;
		cKi_NZPec = NS;
	}
	if (m_fZO_kiec >= fKi_m_vec)
	{
		fKi_m_vec = m_fZO_kiec;
		cKi_NZPec = ZO;
	}
	if (m_fPS_kiec >= fKi_m_vec)
	{
		fKi_m_vec = m_fPS_kiec;
		cKi_NZPec = PS;
	}
	if (m_fPM_kiec >= fKi_m_vec)
	{
		fKi_m_vec = m_fPM_kiec;
		cKi_NZPec = PM;
	}
	if (m_fPB_kiec >= fKi_m_vec)
	{
		fKi_m_vec = m_fPB_kiec;
		cKi_NZPec = PB;
	}
	//求取delt_ki,去模糊化
	cDelt_ki_ind = m_cDelt_ki_index[cKi_NZPe][cKi_NZPec];
	if (cDelt_ki_ind == NB)
	{
		fRet_val = m_decode_uZfiml(m_fDelt_ki_table[NB][0], m_fDelt_ki_table[NB][1], max(cKi_NZPe, cKi_NZPec));
		return fRet_val;
	}
	if (cDelt_ki_ind == PB)
	{
		fRet_val = m_decode_uZfimr(m_fDelt_ki_table[PB][0], m_fDelt_ki_table[PB][1], max(cKi_NZPe, cKi_NZPec));
		return fRet_val;
	}

	fRet_val = m_decode_uTrim(m_fDelt_ki_table[cDelt_ki_ind][0], m_fDelt_ki_table[cDelt_ki_ind][1], m_fDelt_ki_table[cDelt_ki_ind][2], max(cKi_NZPe, cKi_NZPec));
	return fRet_val;
}
double CFuzzyPid::m_Getdelta_kd(const double fe_in, const double fec_in)
{
	double fKd_m_ve = 0;
	double fKd_m_vec = 0;
	double fRet_val = 0;
	char cKd_NZPe = 0;
	char cKd_NZPec = 0;
	char cDelt_kd_ind = 0;
	//计算e在各模糊语言下的隶属度
	if (fe_in <= m_fE_table[NB][0])
	{
		m_fNB_kde = 1.0f;
	}
	if ((m_fE_table[NB][0]<fe_in) && (fe_in<m_fE_table[NB][1]))
	{
		m_fNB_kde = m_uZfiml(m_fE_table[NB][0], m_fE_table[NB][1], fe_in);
		m_fNM_kde = m_uTrim(m_fE_table[NM][0], m_fE_table[NM][1], m_fE_table[NM][2], fe_in);
	}
	if ((m_fE_table[NB][1]<fe_in) && (fe_in<m_fE_table[NM][2]))
	{
		m_fNM_kde = m_uTrim(m_fE_table[NM][0], m_fE_table[NM][1], m_fE_table[NM][2], fe_in);
		m_fNS_kde = m_uTrim(m_fE_table[NS][0], m_fE_table[NS][1], m_fE_table[NS][2], fe_in);
	}
	if ((m_fE_table[NM][2]<fe_in) && (fe_in<m_fE_table[NS][2]))
	{
		m_fNS_kde = m_uTrim(m_fE_table[NS][0], m_fE_table[NS][1], m_fE_table[NS][2], fe_in);
		m_fZO_kde = m_uTrim(m_fE_table[ZO][0], m_fE_table[ZO][1], m_fE_table[ZO][2], fe_in);
	}
	if ((m_fE_table[NS][2]<fe_in) && (fe_in<m_fE_table[ZO][2]))
	{
		m_fZO_kde = m_uTrim(m_fE_table[ZO][0], m_fE_table[ZO][1], m_fE_table[ZO][2], fe_in);
		m_fPS_kde = m_uTrim(m_fE_table[PS][0], m_fE_table[PS][1], m_fE_table[PS][2], fe_in);
	}
	if ((m_fE_table[ZO][2]<fe_in) && (fe_in<m_fE_table[PS][2]))
	{
		m_fPS_kde = m_uTrim(m_fE_table[PS][0], m_fE_table[PS][1], m_fE_table[PS][2], fe_in);
		m_fPM_kde = m_uTrim(m_fE_table[PM][0], m_fE_table[PM][1], m_fE_table[PM][2], fe_in);
	}
	if ((m_fE_table[PS][2]<fe_in) && (fe_in<m_fE_table[PM][2]))
	{
		m_fPM_kde = m_uTrim(m_fE_table[PM][0], m_fE_table[PM][1], m_fE_table[PM][2], fe_in);
		m_fPB_kde = m_uZfimr(m_fE_table[PB][0], m_fE_table[PB][1], fe_in);
	}
	if ((m_fE_table[PM][2]<fe_in) && (fe_in<m_fE_table[PB][1]))
	{
		m_fPB_kde = m_uZfimr(m_fE_table[PB][0], m_fE_table[PB][1], fe_in);
	}
	if (fe_in>m_fE_table[PB][1])
	{
		m_fPB_kde = 1.0f;
	}
	//求取e隶属度最大的
	if (m_fNB_kde >= fKd_m_ve)
	{
		fKd_m_ve = m_fNB_kde;
		cKd_NZPe = NB;
	}
	if (m_fNM_kde >= fKd_m_ve)
	{
		fKd_m_ve = m_fNM_kde;
		cKd_NZPe = NM;
	}
	if (m_fNS_kde >= fKd_m_ve)
	{
		fKd_m_ve = m_fNS_kde;
		cKd_NZPe = NS;
	}
	if (m_fZO_kde >= fKd_m_ve)
	{
		fKd_m_ve = m_fZO_kde;
		cKd_NZPe = ZO;
	}
	if (m_fPS_kde >= fKd_m_ve)
	{
		fKd_m_ve = m_fPS_kde;
		cKd_NZPe = PS;
	}
	if (m_fPM_kde >= fKd_m_ve)
	{
		fKd_m_ve = m_fPM_kde;
		cKd_NZPe = PM;
	}
	if (m_fPB_kde >= fKd_m_ve)
	{
		fKd_m_ve = m_fPB_kde;
		cKd_NZPe = PB;
	}
	//计算ec在各模糊语言下的隶属度
	if (fec_in <= m_fEc_table[NB][0])
	{
		m_fNB_kdec = 1.0f;
	}
	if ((m_fEc_table[NB][0]<fec_in) && (fec_in<m_fEc_table[NB][1]))
	{
		m_fNB_kdec = m_uZfiml(m_fEc_table[NB][0], m_fEc_table[NB][1], fec_in);
		m_fNM_kdec = m_uTrim(m_fEc_table[NM][0], m_fEc_table[NM][1], m_fEc_table[NM][2], fec_in);
	}
	if ((m_fEc_table[NB][1]<fec_in) && (fec_in<m_fEc_table[NM][2]))
	{
		m_fNM_kdec = m_uTrim(m_fEc_table[NM][0], m_fEc_table[NM][1], m_fEc_table[NM][2], fec_in);
		m_fNS_kdec = m_uTrim(m_fEc_table[NS][0], m_fEc_table[NS][1], m_fEc_table[NS][2], fec_in);
	}
	if ((m_fEc_table[NM][2]<fec_in) && (fec_in<m_fEc_table[NS][2]))
	{
		m_fNS_kdec = m_uTrim(m_fEc_table[NS][0], m_fEc_table[NS][1], m_fEc_table[NS][2], fec_in);
		m_fZO_kdec = m_uTrim(m_fEc_table[ZO][0], m_fEc_table[ZO][1], m_fEc_table[ZO][2], fec_in);
	}
	if ((m_fEc_table[NS][2]<fec_in) && (fec_in<m_fEc_table[ZO][2]))
	{
		m_fZO_kdec = m_uTrim(m_fEc_table[ZO][0], m_fEc_table[ZO][1], m_fEc_table[ZO][2], fec_in);
		m_fPS_kdec = m_uTrim(m_fEc_table[PS][0], m_fEc_table[PS][1], m_fEc_table[PS][2], fec_in);
	}
	if ((m_fEc_table[ZO][2]<fec_in) && (fec_in<m_fEc_table[PS][2]))
	{
		m_fPS_kdec = m_uTrim(m_fEc_table[PS][0], m_fEc_table[PS][1], m_fEc_table[PS][2], fec_in);
		m_fPM_kdec = m_uTrim(m_fEc_table[PM][0], m_fEc_table[PM][1], m_fEc_table[PM][2], fec_in);
	}
	if ((m_fEc_table[PS][2]<fec_in) && (fec_in<m_fEc_table[PM][2]))
	{
		m_fPM_kdec = m_uTrim(m_fEc_table[PM][0], m_fEc_table[PM][1], m_fEc_table[PM][2], fec_in);
		m_fPB_kdec = m_uZfimr(m_fEc_table[PB][0], m_fEc_table[PB][1], fec_in);
	}
	if ((m_fEc_table[PM][2]<fec_in) && (fec_in<m_fEc_table[PB][1]))
	{
		m_fPB_kdec = m_uZfimr(m_fEc_table[PB][0], m_fEc_table[PB][1], fec_in);
	}
	if (fec_in>m_fEc_table[PB][1])
	{
		m_fPB_kdec = 1.0f;
	}

	//求取ec隶属度最大的
	if (m_fNB_kdec >= fKd_m_vec)
	{
		fKd_m_vec = m_fNB_kdec;
		cKd_NZPec = NB;
	}
	if (m_fNM_kdec >= fKd_m_vec)
	{
		fKd_m_vec = m_fNM_kdec;
		cKd_NZPec = NM;
	}
	if (m_fNS_kdec >= fKd_m_vec)
	{
		fKd_m_vec = m_fNS_kdec;
		cKd_NZPec = NS;
	}
	if (m_fZO_kdec >= fKd_m_vec)
	{
		fKd_m_vec = m_fZO_kdec;
		cKd_NZPec = ZO;
	}
	if (m_fPS_kdec >= fKd_m_vec)
	{
		fKd_m_vec = m_fPS_kdec;
		cKd_NZPec = PS;
	}
	if (m_fPM_kdec >= fKd_m_vec)
	{
		fKd_m_vec = m_fPM_kdec;
		cKd_NZPec = PM;
	}
	if (m_fPB_kdec >= fKd_m_vec)
	{
		fKd_m_vec = m_fPB_kdec;
		cKd_NZPec = PB;
	}
	//求取delt_kd,去模糊化
	cDelt_kd_ind = m_cDelt_kd_index[cKd_NZPe][cKd_NZPec];
	if (cDelt_kd_ind == NB)
	{
		fRet_val = m_decode_uZfiml(m_fDelt_kd_table[NB][0], m_fDelt_kd_table[NB][1], max(cKd_NZPe, cKd_NZPec));
		return fRet_val;
	}
	if (cDelt_kd_ind == PB)
	{
		fRet_val = m_decode_uZfimr(m_fDelt_kd_table[PB][0], m_fDelt_kd_table[PB][1], max(cKd_NZPe, cKd_NZPec));
		return fRet_val;
	}

	fRet_val = m_decode_uTrim(m_fDelt_kd_table[cDelt_kd_ind][0], m_fDelt_kd_table[cDelt_kd_ind][1], m_fDelt_kd_table[cDelt_kd_ind][2], max(cKd_NZPe, cKd_NZPec));
	return fRet_val;
}