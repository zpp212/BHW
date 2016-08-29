//串口通信的实现
#pragma once
#ifndef __SERIAL_H__ 
#define __SERIAL_H__ 

#include <iostream>
#include <cstring>
#include <atlstr.h>

using namespace std;

class CSerial
{
public:
	CSerial();
	~CSerial();

	bool Open(int nPort = 3, int nBaud = 115200, int nflag=0);
	bool Close(void);

	int ReadData(void*, int);
	int SendData(unsigned char*, int);

	bool IsOpened(void){ return (m_bOpened); }

protected:
	HANDLE m_hIDComDev;
	OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
	bool m_bOpened;
};


#endif