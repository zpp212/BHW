#pragma once
#ifndef  _RFID_COMMUNICATE_H
#define _RFID_COMMUNICATE_H
#include "libmodbus-3.1.4\modbus.h"
#include <iostream>
class CRfidCommunication
{
	friend class CTaskDealing;
public:
	CRfidCommunication();
	//cCom:com口；nBaund:波特率；cCheck_digit:校验位；nDebugflag:调试使能；nSlaveID:从机地址
	CRfidCommunication(const char *cCom, const int nBanud, const char cCheck_digit, const int nDebugflag, const int nSlaveID);
	~CRfidCommunication();
	//功能：获得标签信息，存储在m_dwRfidTagInfo[4]中；
	void GetRfidTagInfo(const int nDebugflag);
private:
	modbus_t * m_pmb;
	uint16_t m_dwRfidTagInfo[4] = { 0 };//存放rfid标签信息
};






#endif // ! _RFID_COMMUNICATE_H