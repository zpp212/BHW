#define _CRT_SECURE_NO_WARNINGS
#include "CTaskDealing.h"



void main()
{
	CTaskDealing Ctaskdealing;
	CMagnativetoServoSpeed CmagnativetoservoSpeed;
	CMagnetic_Navigation Cmagnetic_navigation(3,115200,1);//COM3串口，波特率115200,调试显示为1
	CPLCCommunication Cplccommunication("COM5", 9600, 'E', 1, 1);
	CRfidCommunication Crfidcommunication("COM4", 9600, 'E', 1, 1);
	CTaskDefine Ctaskdefine(3);
	int nTask[3][5] = {
		{1,2,0,1,0},
		{2,3,0,4,0},
		{3,4,1,0,0}
	};
	for (int i = 0; i < 3; i++)
	{
		for(int j=0;j<5;j++)
			Ctaskdefine.m_ppnTaskTable[i][j] = nTask[i][j];
	}
	for (int i = 0; i < 3; i++)
	{
		uint16_t flag;
		flag=Ctaskdealing.GetSettedSpeed(i, Crfidcommunication, Ctaskdefine, Cplccommunication, Cmagnetic_navigation, CmagnativetoservoSpeed);
		if (flag!=100)
		{
			cout << "Wrong RFID Flag" << endl<<"当前RFID："<<flag<<endl;
			break;
		}
	}
	
	system("pause");
	return ;
}