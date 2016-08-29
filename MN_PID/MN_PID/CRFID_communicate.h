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
	//cCom:com�ڣ�nBaund:�����ʣ�cCheck_digit:У��λ��nDebugflag:����ʹ�ܣ�nSlaveID:�ӻ���ַ
	CRfidCommunication(const char *cCom, const int nBanud, const char cCheck_digit, const int nDebugflag, const int nSlaveID);
	~CRfidCommunication();
	//���ܣ���ñ�ǩ��Ϣ���洢��m_dwRfidTagInfo[4]�У�
	void GetRfidTagInfo(const int nDebugflag);
private:
	modbus_t * m_pmb;
	uint16_t m_dwRfidTagInfo[4] = { 0 };//���rfid��ǩ��Ϣ
};






#endif // ! _RFID_COMMUNICATE_H