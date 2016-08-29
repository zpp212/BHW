#include "CPLC-COMMUNICATE.h"
CPLCCommunication::CPLCCommunication()
{
}
CPLCCommunication::CPLCCommunication(const char* c_szCOM, const int c_nBaund, const char c_cCheckDigit, const int c_nSlaveID, const int c_nDebugFlag)
{
	const uint16_t UT_BITS_ADDRESS = 0x130;
	const uint16_t UT_BITS_NB = 0x25;
	const uint8_t UT_BITS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };
	const uint16_t UT_INPUT_BITS_ADDRESS = 0x1C4;
	const uint16_t UT_INPUT_BITS_NB = 0x16;
	const uint8_t UT_INPUT_BITS_TAB[] = { 0xAC, 0xDB, 0x35 };
	const uint16_t UT_REGISTERS_ADDRESS = 0x160;
	const uint16_t UT_REGISTERS_NB = 0x3;
	const uint16_t UT_REGISTERS_NB_MAX = 0x20;
	const uint16_t UT_INPUT_REGISTERS_ADDRESS = 0x108;
	const uint16_t UT_INPUT_REGISTERS_NB = 0x1;
	const uint16_t UT_REGISTERS_NB_SPECIAL = 0x2;
	const uint16_t UT_REGISTERS_ADDRESS_SPECIAL = 0x170;
	const uint16_t UT_REGISTERS_ADDRESS_INVALID_TID_OR_SLAVE = 0x171;
	/* The server will wait for 1 second before replying to test timeout */
	const uint16_t UT_REGISTERS_ADDRESS_SLEEP_500_MS = 0x172;
	/* The server will wait for 5 ms before sending each byte */
	const uint16_t UT_REGISTERS_ADDRESS_BYTE_SLEEP_5_MS = 0x173;
	m_pmb = modbus_new_rtu(c_szCOM, c_nBaund, c_cCheckDigit, 8, 1);//open port，创建串口结构体
	modbus_set_slave(m_pmb, c_nSlaveID);//set slave address
	modbus_set_debug(m_pmb, c_nDebugFlag);
	//设置起始位，数目
	m_pmb_mapping = modbus_mapping_new_start_address(
		UT_BITS_ADDRESS,
		UT_BITS_NB,
		UT_INPUT_BITS_ADDRESS,
		UT_INPUT_BITS_NB,
		UT_REGISTERS_ADDRESS,
		UT_REGISTERS_NB_MAX,
		UT_INPUT_REGISTERS_ADDRESS,
		UT_INPUT_REGISTERS_NB);

	if (m_pmb_mapping == NULL) {
		//fprintf(stderr, "Failed to allocate the mapping: %s\n",
		//	modbus_strerror(errno));
		modbus_free(m_pmb);
	}
	if (modbus_connect(m_pmb) == -1)
	{
		fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		modbus_free(m_pmb);

	}

}

CPLCCommunication::~CPLCCommunication()
{
	modbus_close(m_pmb);
	modbus_free(m_pmb);
}
void CPLCCommunication::SendServoVelocityToPlc(const int c_nDebugFlag)
{
	//电机值的写入
	//m_dwServoVelocity[0] = 1;
	m_pmb_mapping->tab_registers = m_dwServoVelocity;
	int nReturnFlag = 0;
	uint8_t *q_wquery;
	q_wquery = (uint8_t *)malloc(MODBUS_RTU_MAX_ADU_LENGTH);//2.2.分配query内存
	int header_length = modbus_get_header_length(m_pmb);
	nReturnFlag = modbus_receive(m_pmb, q_wquery);//收到的帧存储在query
	nReturnFlag = modbus_reply(m_pmb, q_wquery, nReturnFlag, m_pmb_mapping);//将m_pmd_mapping发出
	if (nReturnFlag == -1) {
		if (c_nDebugFlag)
		{
			printf("%s", modbus_strerror(errno));
		}
	}
}
