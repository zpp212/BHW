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
	double GetMagneticoffset(int nMA_NUM,int nDebugFlag);//选择指定的磁导航传感器读偏差，变量：传感器标号，是否显示调试信息
	//         --(1)   磁导航传感器布置顺序
	//    --(2)     --(4)
	//		   --(3)
	void GenerateSendBuffer(int nMA_NUM);//生成发送帧
private:
	uint8_t m_wMA_buffer_send[10] = { 0 };//发送给从机的指令
	uint8_t m_wMA_buffer_read[100] = { 0 };//接收的从机数据
	uint16_t m_dwdata[24 + 3] = { 0 };//处理之后的数据
	int m_nMA_send = 0, m_nMA_read = 0;//发送与接收的标识
	double m_dMA_max=0, m_dMA_max1= 0 , m_dMA_max2= 0 , m_dMA_max3= 0 , m_dMA_max4= 0 ;//磁导航的误差，传输给逻辑层
	int m_nMA_maxnum = 0;//磁导航传感器亮灯的数量
	CSerial m_Magnetic_navigation;//CSerial类型对象，用于与不经协议的磁导航传感器通信
};

#endif // !_MAGNETIC_NAVIGATION_H