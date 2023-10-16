#ifndef _H_I2CMULTI_DISPATCH_H_
#define _H_I2CMULTI_DISPATCH_H_

#include "I2CSOFTWARE.H"
#include "SYSBIOS.H"
#include "STMSTRING.h"



const unsigned char C_MAXI2CDEV_AMOUNT = 3;
extern bool check_firmware_status ();



// интерфейс I2C обьекта для диспетчера (три метода: CheckComplete, Task, Start)
class TI2CTRDIFACE {
	private:
		//bool F_timer_registered;
		bool f_need_ic_soft_reset;							// флаг означающий что по следующему запуску start - будет произведен soft_reset и инициализация
	
	protected:
		const char *debug_chip_name;
		utimer_t ReleaseTimeout;
		bool F_complete;
		void SetBussError_i2c ();								// если устройство обранужило ошибку и считает ее ошибкой I2C - вызывается этот метод
		void SetBussError_chip ();							// если устройство обранужило ошибку и считает ее ошибкой работы периферийной микросхемы - вызывается этот метод
		void SetBussError_meas ();							// если устройство обранужило ошибку и считает ее ошибкой измерения - вызывается этот метод
	
		unsigned long PermitCount;
		unsigned long BusErrorCount_i2c;
		unsigned long BusErrorCount_chip;
		unsigned long BusErrorCount_measure;
	
		bool f_buss_error;	
		bool f_chip_error;
		bool f_meass_error;
		bool is_soft_reset ();
	
	public:
		TI2CTRDIFACE ();
		//void Init ();
		bool CheckComplete ();
		bool CheckError ();
		void ClearError ();
		unsigned long get_i2c_error_cnt ();
		unsigned long get_chip_error_cnt ();
		unsigned long get_measure_error_cnt ();
	
		const char *get_device_name ();
		virtual void Task (TI2CIFACE *i2cobj) = 0;				// диспетчер постоянно вызывает этот метод для выполнения задачи выбраным устройством
		virtual void Start () = 0;			// диспетчер стартует устройство на чтение данных
};



// сам диспетчер
class TDISPATCHI2C {
	TI2CIFACE *I2C_device;
	
	TI2CTRDIFACE *debug_object;
	utimer_t ReleaseTimeout;
	TI2CTRDIFACE *DevArray[C_MAXI2CDEV_AMOUNT];
	unsigned long DevCount;
	unsigned long CurDevIndx;
	void CircleIndexControl ();
	
	public:
		TDISPATCHI2C (TI2CIFACE *I2C_dv);
		bool Add (TI2CTRDIFACE *dev_iface);				// метод для добавления в диспетчер устройства с TI2CTRDIFACE интерфейсом
		void Task ();															// task диспетчера
};



#endif
