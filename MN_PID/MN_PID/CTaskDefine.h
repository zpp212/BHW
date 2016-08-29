#pragma once
#ifndef _TASKDEFINE_H_
#define _TASKDEFINE_H_
#include <vector>
using std::vector;
enum enJOINT
{
	JOINT_NONE,//�޽ڵ�
	JOINT_J1,//����J1�ڵ�
	JOINT_J2,
	JOINT_J3,
	JOINT_J4,
	JOINT_J5,
	JOINT_J6,
	JOINT_J7,
	JOINT_J8,
	JOINT_J9,
	JOINT_J10
};
enum enJOINTTASK
{
	JOINTTASK_NONE,//������
	JOINTTASK_PARK,//ͣ��
	JOINTTASK_LOADING,//װ��
	JOINTTASK_UNLOAD,//ж��
	JOINTTASK_CHARGE//���
};
enum enFORWARD
{
	FORWARD_NONE,//���н�
	FORWARD_FRONT,//ǰ��
	FORWARD_BACK,//����
	FORWARD_LEFT,//����
	FORWARD_RIGHT//����
};
enum enROTATE
{
	ROTATE_NONE,//����ת
	ROTATE_LEFT90,
	ROTATE_LEFT180,
	ROTATE_LEFT270,
	ROTATE_LEFT360,
	ROTATE_RIGHT90,
	ROTATE_RIGHT180,
	ROTATE_RIGHT270,
	ROTATE_RIGHT360

};
class CTaskDefine
{	
	friend class CTaskDealing;
public:
	CTaskDefine();
	CTaskDefine(int nJointNum);
	~CTaskDefine();
	int m_nrow;
	int** m_ppnTaskTable;//����5���������壺[T1:��ǰ�ڵ㣬T2����һ���ڵ㣬T3����ǰ����T4��ǰ������T5����ת�Ƕ�]
	int** m_ppnTaskEspecial;//����Щ��������(����ȷ��)
private:
	
};

#endif // !_TASKDEFINE_H_