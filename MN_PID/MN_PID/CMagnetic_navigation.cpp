#include "CMagnetic_navigation.h"
CMagnetic_Navigation::CMagnetic_Navigation()
{

}
CMagnetic_Navigation::CMagnetic_Navigation(int nCom, double dBaund, int nDebugFlag)
{
	m_Magnetic_navigation.Open(nCom, dBaund, 1);//COM4串口，波特率115200,调试显示为1

												//m_wMA_buffer_send[7] = m_wMA_buffer_send[0] + m_wMA_buffer_send[1] + m_wMA_buffer_send[2] + m_wMA_buffer_send[3] + m_wMA_buffer_send[4] + m_wMA_buffer_send[5] + m_wMA_buffer_send[6];
}
void CMagnetic_Navigation::GenerateSendBuffer(int nMA_NUM)
{
	m_wMA_buffer_send[0] = 0xFF;
	m_wMA_buffer_send[1] = 0xFE;
	m_wMA_buffer_send[2] = nMA_NUM;
	m_wMA_buffer_send[3] = 0x02;
	m_wMA_buffer_send[4] = 0x02;
	m_wMA_buffer_send[5] = 0x30;
	m_wMA_buffer_send[6] = 0x62;
	for (int i = 0; i < 7; i++)
	{
		m_wMA_buffer_send[7] += m_wMA_buffer_send[i];
	}
}
CMagnetic_Navigation::~CMagnetic_Navigation()
{
	m_Magnetic_navigation.Close();
}
double CMagnetic_Navigation::GetMagneticoffset(int nMA_NUM, int nDebugFlag)
{
	switch (nMA_NUM)
	{
	case 1:
	{
		GenerateSendBuffer(1);
		break;
	}
	case 2:
	{
		GenerateSendBuffer(2);
		break;
	}
	case 3:
	{
		GenerateSendBuffer(3);
		break;
	}
	case 4:
	{
		GenerateSendBuffer(4);
		break;
	}
	default:
		break;
	}
	m_dMA_max = 0;
	m_nMA_maxnum = 0;
	m_nMA_send = m_Magnetic_navigation.SendData(m_wMA_buffer_send, 8);
	m_nMA_read = m_Magnetic_navigation.ReadData(m_wMA_buffer_read, 59);
	for (int i = 0; i < 24; i++)
	{
		m_dwdata[i] = m_wMA_buffer_read[i * 2 + 7] * pow(2, 8) + m_wMA_buffer_read[i * 2 + 8];
	}
	m_dwdata[24] = m_wMA_buffer_read[55];
	m_dwdata[25] = m_wMA_buffer_read[56];
	m_dwdata[26] = m_wMA_buffer_read[57];
	for (int i = 0; i < 8; i++)
	{
		if ((m_dwdata[24] & (1 << i)) != 0)
		{
			m_dMA_max += (i + 1);
			m_nMA_maxnum++;
		}
		if ((m_dwdata[25] & (1 << i)) != 0)
		{
			m_dMA_max += (i + 8 + 1);
			m_nMA_maxnum++;
		}
		if ((m_dwdata[26] & (1 << i)) != 0)
		{
			m_dMA_max += (i + 16 + 1);
			m_nMA_maxnum++;
		}
	}
	if (m_nMA_maxnum != 0)
	{
		m_dMA_max /= m_nMA_maxnum;
	}
	if (nDebugFlag)
	{
		cout << bitset<8>(m_dwdata[24]) << " ";
		cout << bitset<8>(m_dwdata[25]) << " ";
		cout << bitset<8>(m_dwdata[26]) << " ";
		cout << m_dMA_max << endl;
	}
	switch (nMA_NUM)
	{
	case 1:
	{
		m_dMA_max1 = m_dMA_max;
		break;
	}
	case 2:
	{
		m_dMA_max2 = m_dMA_max;
		break;
	}
	case 3:
	{
		m_dMA_max3 = m_dMA_max;
		break;
	}
	case 4:
	{
		m_dMA_max4 = m_dMA_max;
		break;
	}
	default:
		break;
	}
	return m_dMA_max;
}
