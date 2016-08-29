#pragma once
#ifndef _PLC_COMMUNICATE_H
#define _PLC_COMMUNICATE_H
#include <iostream>
#include "libmodbus-3.1.4\modbus.h"
#define INVALID_SERVER_ID 18
#ifndef MSG_NOSIGNAL
# define MSG_NOSIGNAL 0
#endif

class CPLCCommunication
{
	friend class CTaskDealing;
public:
	CPLCCommunication();
	CPLCCommunication(const char* c_szCOM, const int c_nBaund, const char c_cCheckDigit, const int c_nSlaveID, const int c_nDebugFlag);
	~CPLCCommunication();
	void SendServoVelocityToPlc(const int c_nDebugFlag);
private:
	modbus_t *m_pmb;
	modbus_mapping_t* m_pmb_mapping;
	int32_t m_dwServoVelocity[5] = { 0,0,0,0,0};//前四位为电机速度，最后一位为旋转时的时间
};


#endif // !_PLC_COMMUNICATE_H