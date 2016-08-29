#pragma once

#ifndef _MAGNETIC_NAVIGATION_H
#define _MAGNETIC_NAVIGATION_H
#include "Cserial.h"
#include <bitset>
using namespace std;
class CMagnetic_Navigation
{
public:
	CMagnetic_Navigation();
	CMagnetic_Navigation(int nCom, double dBaund, int nDebugFlag);
	~CMagnetic_Navigation();
	double GetMagneticoffset(int nMA_NUM,int nDebugFlag);//ѡ��ָ���Ĵŵ�����������ƫ���������������ţ��Ƿ���ʾ������Ϣ
	//         --(1)   �ŵ�������������˳��
	//    --(2)     --(4)
	//		   --(3)
	void GenerateSendBuffer(int nMA_NUM);//���ɷ���֡
private:
	uint8_t m_wMA_buffer_send[10] = { 0 };//���͸��ӻ���ָ��
	uint8_t m_wMA_buffer_read[100] = { 0 };//���յĴӻ�����
	uint16_t m_dwdata[24 + 3] = { 0 };//����֮�������
	int m_nMA_send = 0, m_nMA_read = 0;//��������յı�ʶ
	double m_dMA_max=0, m_dMA_max1= 0 , m_dMA_max2= 0 , m_dMA_max3= 0 , m_dMA_max4= 0 ;//�ŵ�������������߼���
	int m_nMA_maxnum = 0;//�ŵ������������Ƶ�����
	CSerial m_Magnetic_navigation;//CSerial���Ͷ��������벻��Э��Ĵŵ���������ͨ��
};

#endif // !_MAGNETIC_NAVIGATION_H