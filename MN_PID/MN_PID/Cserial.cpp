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
	if (m_bOpened) return true;//�����Ѿ��򿪣�����true����ʾ�򿪳ɹ�

	TCHAR szPort[20];
	DCB dcb;

	wsprintf(szPort, _T("COM%d"), nPort);
	//��CreateFile�򿪴���
	m_hIDComDev = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//szPortΪҪ�򿪵Ĵ��ڣ��ڶ�������Ϊ���ʵ�ģʽ��д/����������������0��ʾ������,OPEN_EXISTING��ʾ�򿪶����Ǵ�����FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED��ʾ�첽
	if (m_hIDComDev == INVALID_HANDLE_VALUE)
	{

		MessageBox(NULL, _T("��COMʧ��!"), _T("����"), MB_OK);
		return false;
	}

	memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
	memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));

	COMMTIMEOUTS CommTimeOuts;//���úͲ�ѯͨѶ�豸�ĳ�ʱ����
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	//ReadIntervalTimeout����ΪMAXDWORD,ReadTotalTimeoutConstant��ReadTotalTimeoutMultiplier��Ϊ�㣬�����ȡ��������������Щ�ѽ��յ����ݡ�
	CommTimeOuts.WriteTotalTimeoutMultiplier = 500;
	CommTimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(m_hIDComDev, &CommTimeOuts);//�����Ѵ򿪵Ĵ��пڵĳ�ʱ����

	m_OverlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_OverlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hIDComDev, &dcb);
	dcb.BaudRate = nBaud;//���ò����ʵ�ֵ
	dcb.ByteSize = 8;//ÿ���ֽ���8λ
	dcb.fParity = 1;//��żУ��ʹ��
	dcb.Parity = EVENPARITY;//żУ��
	dcb.StopBits = ONESTOPBIT;//һ��ֹͣλ

	if (!SetCommState(m_hIDComDev, &dcb) || !SetupComm(m_hIDComDev, 10000, 10000) || m_OverlappedRead.hEvent == NULL || m_OverlappedWrite.hEvent == NULL)
	{
		//COM�ڵĿ��ƿ�����ʧ�ܡ����û�����ʧ�ܡ��޶�д��־
		if (nflag)
		{
			MessageBox(NULL, _T("�򿪴���ʧ�ܡ������!"), _T("����"), MB_OK);
			cout << "�򿪴���ʧ�ܡ������" << GetLastError() << endl;
		}
		DWORD   dwError = GetLastError();
		if (m_OverlappedRead.hEvent != NULL) CloseHandle(m_OverlappedRead.hEvent);
		if (m_OverlappedWrite.hEvent != NULL) CloseHandle(m_OverlappedWrite.hEvent);
		CloseHandle(m_hIDComDev);
		return(false);
	}
	PurgeComm(m_hIDComDev, PURGE_TXCLEAR | PURGE_RXCLEAR);//�ڶ�д����֮ǰ��ջ�����
	if (nflag)
	{
		cout << "�򿪴��ڳɹ�" << endl;
	}
	m_bOpened = true;

	return(m_bOpened);
}

bool CSerial::Close(void)
{
	if (!m_bOpened || m_hIDComDev == NULL) return true;//��������û�����ߴ����ǿյģ�����true����ʾ�رճɹ�

	if (m_OverlappedRead.hEvent != NULL) CloseHandle(m_OverlappedRead.hEvent);
	if (m_OverlappedWrite.hEvent != NULL) CloseHandle(m_OverlappedWrite.hEvent);
	CloseHandle(m_hIDComDev);
	m_bOpened = false;
	m_hIDComDev = NULL;

	return true;
}

int CSerial::SendData(unsigned char *buffer, int size)
{

	if (!m_bOpened || m_hIDComDev == NULL)   return 0;//������û�б��򿪻��ߴ��ھ��Ϊ�գ��޷���������

	BOOL bWriteStat;
	DWORD dwBytesWritten = size;

	bWriteStat = WriteFile(m_hIDComDev, buffer, dwBytesWritten, &dwBytesWritten, &m_OverlappedWrite);
	if (!bWriteStat)//���WriteFile��������false
	{
		if (GetLastError() == ERROR_IO_PENDING)//GetLastError()��������ERROR_IO_PENDING�������������ڽ���д����
		{
			WaitForSingleObject(m_OverlappedRead.hEvent, 1000);
			//ʹ��WaitForSingleObject�����ȴ���ֱ����������ɻ���ʱ�Ѵﵽ1���ӣ������ڶ�������Ϻ�m_OverlappedRead��hEvent�¼����Ϊ���ź�
			return dwBytesWritten;
		}
		return 0;
	}

	return dwBytesWritten;//���ط�����Ϣ��λ������size

}

int CSerial::ReadData(void *buffer, int limit)
{

	if (!m_bOpened || m_hIDComDev == NULL)   return(0);//������û�д򿪻��ߴ��ھ��Ϊ��

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError(m_hIDComDev, &dwErrorFlags, &ComStat);//��ClearCommError�����������
	if (!ComStat.cbInQue) return 0;//cbInQue��Ա������ֵ��ʾ���뻺�������ֽ���

	dwBytesRead = (DWORD)ComStat.cbInQue;
	if (limit < (int)dwBytesRead)   dwBytesRead = (DWORD)limit;

	bReadStatus = ReadFile(m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead);
	if (!bReadStatus)//���ReadFile��������false
	{
		if (GetLastError() == ERROR_IO_PENDING)//GetLastError()��������ERROR_IO_PENDING�������������ڽ��ж�����  
		{
			//ʹ��WaitForSingleObject�����ȴ���ֱ����������ɻ���ʱ�Ѵﵽ2���ӣ������ڶ�������Ϻ�m_OverlappedRead��hEvent�¼����Ϊ���ź�
			WaitForSingleObject(m_OverlappedRead.hEvent, 2000);
			PurgeComm(m_hIDComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//��PurgeComm������մ��ڵ��������������
			return ((int)dwBytesRead);
		}
		return (0);
	}

	PurgeComm(m_hIDComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	return ((int)dwBytesRead);
}
