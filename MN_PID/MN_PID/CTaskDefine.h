#pragma once
#ifndef _TASKDEFINE_H_
#define _TASKDEFINE_H_
#include <vector>
using std::vector;
enum enJOINT
{
	JOINT_NONE,//无节点
	JOINT_J1,//代表J1节点
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
	JOINTTASK_NONE,//无任务
	JOINTTASK_PARK,//停车
	JOINTTASK_LOADING,//装货
	JOINTTASK_UNLOAD,//卸货
	JOINTTASK_CHARGE//充电
};
enum enFORWARD
{
	FORWARD_NONE,//无行进
	FORWARD_FRONT,//前进
	FORWARD_BACK,//后退
	FORWARD_LEFT,//左行
	FORWARD_RIGHT//右行
};
enum enROTATE
{
	ROTATE_NONE,//无旋转
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
	int** m_ppnTaskTable;//单行5个参数含义：[T1:当前节点，T2：下一个节点，T3：当前任务，T4：前进方向，T5：旋转角度]
	int** m_ppnTaskEspecial;//定义些特殊任务(后期确定)
private:
	
};

#endif // !_TASKDEFINE_H_