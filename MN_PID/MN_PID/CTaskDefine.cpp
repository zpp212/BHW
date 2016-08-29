#ifndef _TASKDEFINE_H_
#include "CTaskDefine.h"
CTaskDefine::CTaskDefine()
{}
CTaskDefine::CTaskDefine(int nJointNum)
{
	m_nrow = nJointNum;
	m_ppnTaskTable = new int*[nJointNum];
	for (int i = 0; i < nJointNum; i++)
	{
		m_ppnTaskTable[i] = new int[5];
	}

}

CTaskDefine::~CTaskDefine()
{
	for (int i = 0; i < m_nrow; i++)
	{
		delete m_ppnTaskTable[i];
		m_ppnTaskTable[i] = nullptr;
	}
	delete[m_nrow]m_ppnTaskTable;
	m_ppnTaskTable = nullptr;
}
#endif // !_TASKDEFINE_H_