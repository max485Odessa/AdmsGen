#ifndef _H_FRIENDTASK_INTERFACE_H_
#define _H_FRIENDTASK_INTERFACE_H_



#define C_MAXFRIEND_TASK 25


class TFFC {
	protected:
		static TFFC *TaskArray[C_MAXFRIEND_TASK];
		static unsigned short task_counter;
		unsigned long CurIX;
		void AddObjectToExecuteManager ();
	
	public:
		TFFC ();
		virtual void Task ();
		unsigned long GetTaskIX ();
};



class TFCC_MANAGER : public TFFC {
	
	public:
		static void Execute_Tasks ();
};



#endif
