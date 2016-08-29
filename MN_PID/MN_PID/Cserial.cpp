#include "Cserial.h"
CSerial::CSerial()
{
	memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
	memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));
	m_hIDComDev = NULL;
	m_bOpened = false;
}

CSerial::~CSerial()
{
	Close();
}

bool CSerial::Open(int nPort, int nBaud, int nflag)
{
	if (m_bOpened) return true;//串口已经打开，返回true，表示打开成功

	TCHAR szPort[20];
	DCB dcb;

	wsprintf(szPort, _T("COM%d"), nPort);
	//用CreateFile打开串口
	m_hIDComDev = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//szPort为要打开的串口，第二个变量为访问的模式（写/读），第三个变量0表示不共享,OPEN_EXISTING表示打开而不是创建，FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED表示异步
	if (m_hIDComDev == INVALID_HANDLE_VALUE)
	{

		MessageBox(NULL, _T("打开COM失败!"), _T("警告"), MB_OK);
		return false;
	}

	memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
	memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));

	COMMTIMEOUTS CommTimeOuts;//设置和查询通讯设备的超时参数
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	//ReadIntervalTimeout设置为MAXDWORD,ReadTotalTimeoutConstant和ReadTotalTimeoutMultiplier设为零，代表读取操作立即返回那些已接收的数据。
	CommTimeOuts.WriteTotalTimeoutMultiplier = 500;
	CommTimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(m_hIDComDev, &CommTimeOuts);//设置已打开的串行口的超时参数

	m_OverlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_OverlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hIDComDev, &dcb);
	dcb.BaudRate = nBaud;//设置波特率的值
	dcb.ByteSize = 8;//每个字节有8位
	dcb.fParity = 1;//奇偶校验使能
	dcb.Parity = EVENPARITY;//偶校验
	dcb.StopBits = ONESTOPBIT;//一个停止位

	if (!SetCommState(m_hIDComDev, &dcb) || !SetupComm(m_hIDComDev, 10000, 10000) || m_OverlappedRead.hEvent == NULL || m_OverlappedWrite.hEvent == NULL)
	{
		//COM口的控制块设置失败、设置缓冲区失败、无读写标志
		if (nflag)
		{
			MessageBox(NULL, _T("打开串口失败。错误号!"), _T("警告"), MB_OK);
			cout << "打开串口失败。错误号" << GetLastError() << endl;
		}
		DWORD   dwError = GetLastError();
		if (m_OverlappedRead.hEvent != NULL) CloseHandle(m_OverlappedRead.hEvent);
		if (m_OverlappedWrite.hEvent != NULL) CloseHandle(m_OverlappedWrite.hEvent);
		CloseHandle(m_hIDComDev);
		return(false);
	}
	PurgeComm(m_hIDComDev, PURGE_TXCLEAR | PURGE_RXCLEAR);//在读写串口之前清空缓冲区
	if (nflag)
	{
		cout << "打开串口成功" << endl;
	}
	m_bOpened = true;

	return(m_bOpened);
}

bool CSerial::Close(void)
{
	if (!m_bOpened || m_hIDComDev == NULL) return true;//若本来就没开或者串口是空的，返回true，表示关闭成功

	if (m_OverlappedRead.hEvent != NULL) CloseHandle(m_OverlappedRead.hEvent);
	if (m_OverlappedWrite.hEvent != NULL) CloseHandle(m_OverlappedWrite.hEvent);
	CloseHandle(m_hIDComDev);
	m_bOpened = false;
	m_hIDComDev = NULL;

	return true;
}

int CSerial::SendData(unsigned char *buffer, int size)
{

	if (!m_bOpened || m_hIDComDev == NULL)   return 0;//若串口没有被打开或者串口句柄为空，无法发送数据

	BOOL bWriteStat;
	DWORD dwBytesWritten = size;

	bWriteStat = WriteFile(m_hIDComDev, buffer, dwBytesWritten, &dwBytesWritten, &m_OverlappedWrite);
	if (!bWriteStat)//如果WriteFile函数返回false
	{
		if (GetLastError() == ERROR_IO_PENDING)//GetLastError()函数返回ERROR_IO_PENDING，表明串口正在进行写操作
		{
			WaitForSingleObject(m_OverlappedRead.hEvent, 1000);
			//使用WaitForSingleObject函数等待，直到读操作完成或延时已达到1秒钟；当串口读操作完毕后m_OverlappedRead的hEvent事件会变为有信号
			return dwBytesWritten;
		}
		return 0;
	}

	return dwBytesWritten;//返回发送信息的位数，即size

}

int CSerial::ReadData(void *buffer, int limit)
{

	if (!m_bOpened || m_hIDComDev == NULL)   return(0);//若串口没有打开或者串口句柄为空

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError(m_hIDComDev, &dwErrorFlags, &ComStat);//用ClearCommError函数清除错误
	if (!ComStat.cbInQue) return 0;//cbInQue成员变量的值表示输入缓冲区的字节数

	dwBytesRead = (DWORD)ComStat.cbInQue;
	if (limit < (int)dwBytesRead)   dwBytesRead = (DWORD)limit;

	bReadStatus = ReadFile(m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead);
	if (!bReadStatus)//如果ReadFile函数返回false
	{
		if (GetLastError() == ERROR_IO_PENDING)//GetLastError()函数返回ERROR_IO_PENDING，表明串口正在进行读操作  
		{
			//使用WaitForSingleObject函数等待，直到读操作完成或延时已达到2秒钟；当串口读操作完毕后m_OverlappedRead的hEvent事件会变为有信号
			WaitForSingleObject(m_OverlappedRead.hEvent, 2000);
			PurgeComm(m_hIDComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//用PurgeComm函数清空串口的输入输出缓冲区
			return ((int)dwBytesRead);
		}
		return (0);
	}

	PurgeComm(m_hIDComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	return ((int)dwBytesRead);
}
