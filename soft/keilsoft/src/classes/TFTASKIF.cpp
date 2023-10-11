#include "TFTASKIF.h"



TFFC *TFFC::TaskArray[C_MAXFRIEND_TASK];
unsigned short TFFC::task_counter = 0;



TFFC::TFFC ()
{
	AddObjectToExecuteManager ();
}



void TFFC::Task ()
{

}



void TFFC::AddObjectToExecuteManager ()
{
	if (task_counter < C_MAXFRIEND_TASK)
		{
		TaskArray[task_counter] = this;
		CurIX = task_counter;
		task_counter++;
		}
}



unsigned long TFFC::GetTaskIX ()
{
	return CurIX;
}



void TFCC_MANAGER::Execute_Tasks ()
{
	unsigned short Ix = 0;
	while (Ix < task_counter)
		{
		//TBATRAM::Set_CurentTask (TaskArray[Ix]->GetTaskIX() + 1);
		TaskArray[Ix]->Task ();
		Ix++;
		}
}


