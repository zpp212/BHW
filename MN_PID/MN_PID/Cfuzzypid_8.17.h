#pragma once
#ifndef _FUZZYPID_817_H
#define _FUZZYPID_817_H
#define max(x,y) (x>=y?x:y)
#define min(x,y) (x>=y?y:x)
class CFuzzyPid
{
	friend class CMagnativetoServoSpeed;
public:
	CFuzzyPid();
	~CFuzzyPid();
	
private:
	enum m_enFUZZY
	{
		NB = 0, NM, NS, ZO, PS, PM, PB

	};
	//e,ec,kp,ki,kd����ģ������ֵ�����������Ĳ���
	double m_fE_table[7][3] = {
		{ -8.00f,-6.22f,-6.22f },/*NB--Zl*/
		{ -7.11f,-5.33f,-3.56f },/*NM-����������*/
		{ -4.44f,-2.67f,-0.89f },/*NS-����������*/
		{ -1.78f,0.0f,1.78f },/*zero--����������*/
		{ 0.89f,2.67f,4.44f },/*PS--����������*/
		{ 3.56f,5.33f,7.11f },/*PM--����������*/
		{ 6.22f,8.00f,8.00f },/*PB--Zr*/
	};
	//���ݲ����������Ŵ�e_table,���������Ϊ0.1�����e_table�Ŵ�10�����ec_table
	double m_fEc_table[7][3] = { { -80.0f,-62.2f,-62.2f },/*NB--Zl*/
	{ -71.1f,-53.3f,-35.6f },/*NM-����������*/
	{ -44.4f,-26.7f,-8.90f },/*NS-����������*/
	{ -17.8f,0.00f,17.8f },/*zero--����������*/
	{ 8.9f,26.7f,44.4f },/*PS--����������*/
	{ 35.6f,53.3f,71.1f },/*PM--����������*/
	{ 62.2f,80.0f,80.0f },/*PB--Zr*/
	};
	//kp
	double m_fDelt_kp_table[7][3] = { { -5.00f,-3.89f,-3.89f },/*NB--Zl*/
	{ -4.44f,-3.33f,-2.22f },/*NM-����������*/
	{ -2.78f,-1.67f,-0.56f },/*NS-����������*/
	{ -1.11f,0.0f,1.11f },/*zero--����������*/
	{ 0.56f,1.67f,2.78f },/*PS--����������*/
	{ 2.22f,3.33f,4.44f },/*PM--����������*/
	{ 3.89f,5.00f,5.00f },/*PB--Zr*/
	};
	//ki
	double m_fDelt_ki_table[7][3] = {
		{ -2.00f, -1.56f, -1.56f },/*NB--Zl*/
		{ -1.77f, -1.33f, -0.89f },/*NM-����������*/
		{ -1.11f, -0.67f, -0.22f },/*NS-����������*/
		{ -0.44f,  0.00f,   0.44f },/*zero--����������*/
		{ 0.22f,   0.67f,   1.11f },/*PS--����������*/
		{ 0.89f,   1.33f,   1.78f },/*PM--����������*/
		{ 1.56f,   2.0f,    2.0f },/*PB--Zr*/
	};
	//kd
	double m_fDelt_kd_table[7][3] = {
		{ -2.00f, -1.56f, -1.56f },/*NB--Zl*/
		{ -1.77f, -1.33f, -0.89f },/*NM-����������*/
		{ -1.11f, -0.67f, -0.22f },/*NS-����������*/
		{ -0.44f,  0.00f,   0.44f },/*zero--����������*/
		{ 0.22f,   0.67f,   1.11f },/*PS--����������*/
		{ 0.89f,   1.33f,   1.78f },/*PM--����������*/
		{ 1.56f,   2.0f,    2.0f },/*PB--Zr*/
	};
	//ģ��pid�����
	char m_cDelt_kp_index[7][7] = 
	  { 6,6,6,6,4,3,2,
		6,6,5,5,3,2,1,
		6,5,5,4,2,1,0,
		3,3,3,3,3,3,3,
		0,1,2,4,5,5,6,
		1,2,3,5,6,6,6,
		2,3,4,6,6,6,6 };
	char m_cDelt_ki_index[7][7] = 
	  { 6,6,6,6,2,1,0,
		6,6,5,5,1,0,0,
		6,5,4,4,0,0,0,
		3,3,3,3,3,3,3,
		0,0,0,4,4,5,6,
		0,0,1,5,5,6,6,
		0,1,2,6,6,6,6 };
	char m_cDelt_kd_index[7][7] =
	  { 6,6,6,0,0,1,2,
		6,6,5,5,3,4,5,
		6,5,5,2,5,6,6,
		3,3,3,3,3,3,3,
		6,6,5,2,5,5,6,
		5,4,3,1,5,6,6,
		2,1,0,0,6,6,6 };
	//kp��e,ec��ʼ��
	double m_fNB_kpe = 0.0f;
	double m_fNM_kpe = 0.0f;
	double m_fNS_kpe = 0.0f;
	double m_fZO_kpe = 0.0f;
	double m_fPS_kpe = 0.0f;
	double m_fPM_kpe = 0.0f;
	double m_fPB_kpe = 0.0f;
	double m_fNB_kpec = 0.0f;
	double m_fNM_kpec = 0.0f;
	double m_fNS_kpec = 0.0f;
	double m_fZO_kpec = 0.0f;
	double m_fPS_kpec = 0.0f;
	double m_fPM_kpec = 0.0f;
	double m_fPB_kpec = 0.0f;
	//ki��e,ec��ʼ��
	double m_fNB_kie = 0.0f;
	double m_fNM_kie = 0.0f;
	double m_fNS_kie = 0.0f;
	double m_fZO_kie = 0.0f;
	double m_fPS_kie = 0.0f;
	double m_fPM_kie = 0.0f;
	double m_fPB_kie = 0.0f;
	double m_fNB_kiec = 0.0f;
	double m_fNM_kiec = 0.0f;
	double m_fNS_kiec = 0.0f;
	double m_fZO_kiec = 0.0f;
	double m_fPS_kiec = 0.0f;
	double m_fPM_kiec = 0.0f;
	double m_fPB_kiec = 0.0f;
	//kd��e,ec��ʼ��
	double m_fNB_kde = 0.0f;
	double m_fNM_kde = 0.0f;
	double m_fNS_kde = 0.0f;
	double m_fZO_kde = 0.0f;
	double m_fPS_kde = 0.0f;
	double m_fPM_kde = 0.0f;
	double m_fPB_kde = 0.0f;
	double m_fNB_kdec = 0.0f;
	double m_fNM_kdec = 0.0f;
	double m_fNS_kdec = 0.0f;
	double m_fZO_kdec = 0.0f;
	double m_fPS_kdec = 0.0f;
	double m_fPM_kdec = 0.0f;
	double m_fPB_kdec = 0.0f;

	//��������
	double m_uTrim(const double c_fX1, const double c_fX2, const double c_fX3, const double c_fX);//(���������Ⱥ���)
	double m_uZfiml(const double c_fX1, const double c_fX2, const double c_fX);//(��Z�������Ⱥ���)
	double m_uZfimr(const double c_fX1, const double c_fX2, const double c_fX);//(��Z�������Ⱥ���)
	double m_decode_uTrim(const double c_fX1, const double c_fX2, const double c_fX3, const double c_fX);//(���������Ⱥ���ȥģ����)
	double m_decode_uZfiml(const double c_fX1, const double c_fX2, const double c_fX);//(��Z�������Ⱥ���ȥģ����)
	double m_decode_uZfimr(const double c_fX1, const double c_fX2, const double c_fX);//(��Z�������Ⱥ���ȥģ����)
    //��ȡdelta_kp,delta_ki,delta_kd
	double m_Getdelta_kd(const double fe_in, const double fec_in);//��ȡȥģ����Kd����ֵ
	double m_Getdelta_kp(const double fe_in, const double fec_in);// ��ȡȥģ����Kp����ֵ
	double m_Getdelta_ki(const double fe_in, const double fec_in);//��ȡȥģ����Ki����ֵ
};


#endif // !_FUZZYPID_817_H