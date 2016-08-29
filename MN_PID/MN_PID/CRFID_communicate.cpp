#include "CRFID_communicate.h"
CRfidCommunication::CRfidCommunication()
{
}
CRfidCommunication::CRfidCommunication(const char *cCom, const int nBanud, const char cCheck_digit, const int nDebugflag, const int nSlaveID)
{
	m_pmb = modbus_new_rtu(cCom, nBanud, cCheck_digit, 8, 1);//open port���������ڽṹ��
	modbus_set_slave(m_pmb, nSlaveID);//set slave address
	modbus_set_debug(m_pmb, nDebugflag);//�Ƿ���ʾ������Ϣ
	modbus_set_response_timeout(m_pmb, 1, 0);
	int nMbConnection = modbus_connect(m_pmb);
	if (nDebugflag)
	{
		if (m_pmb&&nMbConnection == -1) {
			printf("\nSorry,fail to connect ttyS0,please check your device's state.\n");
		}
		else {
			printf("Connected to dev!\n");
			//connected = 1;
		}
	}

}
CRfidCommunication::~CRfidCommunication()
{
	modbus_close(m_pmb);
	modbus_free(m_pmb);
}
void CRfidCommunication::GetRfidTagInfo(const int nDebugflag)
{
	int nIsReadRegisterSuccess;
	nIsReadRegisterSuccess = modbus_read_registers(m_pmb, 5, 4, m_dwRfidTagInfo);//���ӻ��Ĵ����������������Σ�mbָ�룬��ʼ��ַ����ȡλ�����洢��ַ
	if (nDebugflag)
	{
		printf("%x\n", m_dwRfidTagInfo);
		for (int i = 0; i < sizeof(m_dwRfidTagInfo) / 2; i++)
		{
			printf("\n%d:%4x", i, m_dwRfidTagInfo[i]);
			//printf("\n");
		}
		printf("\n");
	}

}