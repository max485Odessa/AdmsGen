#include "stm32f4xx_hal.h"
#include "TAIN.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"
#include "ST7565R_SPI.H"
#include "LCD132x64.h"
#include "TEASYKEYS.h"
#include "TCORE.h"
#include "TM24Cxxx.h"
#include "I2CSOFTWARE.H"
#include "IEXTINT.h"
#include "THALLDIG.h"
#include "ITIMINT.h"
#include "TGlobalISR.h"
#include "TINA226.h"
#include "TPWM.h"
#include "TPIDMOTOR.h"



/*
USED RESOURCES:
	
*/

static S_PWM_INIT_LIST_T pin_mot_pwm = {{GPIOB,GPIO_PIN_6},ETIMCH_1, E_GPIO_AF2};

void SystemClockHSE_Config (void);
static S_GPIOPIN pinphase_in_a = {GPIOB,GPIO_PIN_12};	// pb12, pb13, pb14
static S_GPIOPIN pinphase_out_a = {GPIOA,GPIO_PIN_12};		// pb0, pb1, pb2
	
static S_GPIOPIN pinsi2c_a[EARRI2CPIN_ENDENUM] = {{GPIOB,GPIO_PIN_3}/*scl*/, {GPIOB,GPIO_PIN_4}/*sda*/};
static const S_GPIOPIN rawpins_keys[EKEYSID_ENDENUM] = {{GPIOB,GPIO_PIN_2/*right*/}, {GPIOB,GPIO_PIN_1/*left*/}, {GPIOB,GPIO_PIN_10/*select*/}, {GPIOB,GPIO_PIN_0/*menu*/}, \
{GPIOB,GPIO_PIN_9/*onoff*/}};
static const S_GPIOPIN rawpins_lcd[EST7565P_ENDENUM] = {/*EST7565P_CS*/{GPIOA,GPIO_PIN_11}, /*EST7565P_REST*/{GPIOA, GPIO_PIN_10}, /*EST7565P_RS*/{GPIOA, GPIO_PIN_9},\
/*EST7565P_SCL*/ {GPIOA, GPIO_PIN_8}, /*EST7565P_SI*/{GPIOB, GPIO_PIN_15}};
//static TCONTRECT *rectifier;
static TST7565RSPI *lcd;
static TLCDCANVABW *canva;
static TEASYKEYS *keys;
static TCORERCT *core;
static TI2CIFACE *busi2c;
static TM24C16 *memi2c;
//static TLINEARHALL *linear_hall;
static TAIN *ain;
static TEXTINT_ISR *extint_obj;
static TTIM_MKS_ISR *timisr_obj;
static TTIM_MKS_ISR *timsystem_obj;
static THALLDIG *dighall;
static TTINA226 *cur_motor;
static TTINA226 *cur_src24;
static TPWM *pwm;
static TPIDPWM *motor_pid;
//static TPWMIMPL *pwm;
	
#ifdef __cplusplus
 extern "C" {
#endif 



void SysTick_Handler(void)
{
//  HAL_IncTick();
//	SYSBIOS::EXECUTE_PERIODIC_ISR (1);
}


#ifdef __cplusplus
}
#endif



static void frendly_task ()
{
	SYSBIOS::EXECUTE_PERIODIC_SYS ();
	TFCC_MANAGER::Execute_Tasks ();
}

/*
TIM_HandleTypeDef    TimHandle;
uint32_t uwPrescalerValue = 0;

static void init_base_timer ()
{
  __HAL_RCC_TIM3_CLK_ENABLE();
  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
  uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / 10000) - 1;
  TimHandle.Instance = TIM3;
  TimHandle.Init.Period            = 10000 - 1;
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;

  HAL_TIM_Base_Init(&TimHandle);

  HAL_TIM_Base_Start_IT(&TimHandle);
}
*/


class TMain: public ITIMCB {
	public:
		virtual void tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch) override;
};


//static uint32_t cnt = 0;


void TMain::tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch)
{
if (ch == EPWMCHNL_UPDATE)
	{
	//cnt++;
  HAL_IncTick();
	SYSBIOS::EXECUTE_PERIODIC_ISR (1);
	}
}



static TMain *mainobj;




// tim 2/5 - 32 bits
int main ()
{
	mainobj = new TMain ();
	
	
	static uint8_t updline_cnt = 0;
	SystemClockHSE_Config ();
	timsystem_obj = new TTIM_MKS_ISR (ESYSTIM_TIM3, 1000, 1000000);
	timsystem_obj->set_tim_cb (EPWMCHNL_UPDATE, mainobj);
	timsystem_obj->enable_timer_oc (EPWMCHNL_UPDATE, true);
	timsystem_obj->enable_timer_isr (true);
	//TGLOBISR::enable ();

	keys = new TEASYKEYS (const_cast<S_GPIOPIN*>(rawpins_keys), EKEYSID_ENDENUM);
	// pinsi2c_a
	busi2c = new TI2CIFACE (pinsi2c_a, 50);
	memi2c = new TM24C16 (busi2c, 0);
	
	cur_motor = new TTINA226 (0, busi2c, 0.02F);	// (A1)gnd, (A0)gnd
	cur_src24 = new TTINA226 (1, busi2c, 0.02F);	// (A1)gnd, (A0)vs

	
	canva = new TLCDCANVABW ();
	canva->Init ();
	lcd = new TST7565RSPI (const_cast<S_GPIOPIN*>(rawpins_lcd));
	lcd->LCD_init ();
	
	pwm = new TPWM (ESYSTIM_TIM4, 1000, 1000000, &pin_mot_pwm, 1);
	motor_pid = new TPIDPWM (pwm->getChanel (pin_mot_pwm.ch));
	
	extint_obj = new TEXTINT_ISR (&pinphase_in_a, EGPINTMOD_RISING);
	timisr_obj = new TTIM_MKS_ISR (ESYSTIM_TIM2, 0xFFFFFFFF, 1000000);

	dighall = new THALLDIG (timisr_obj, extint_obj, EPWMCHNL_PWM2, 4, 5);
	dighall->enable (true);
	dighall->set_sync_cb (motor_pid);

	
	extint_obj->set_cb (dighall);

	ain = new TAIN ();
	
	core = new TCORERCT (dighall, &pinphase_out_a, canva, keys, memi2c, ain, cur_motor, cur_src24, motor_pid);
	
	updline_cnt = C_LCD_PAGE_AMOUNT;
	//static float freq;
	while (true)
		{
		frendly_task ();
		//freq = dighall->get_freq ();
		if (core->is_lcd_update ()) 
			{
			updline_cnt = C_LCD_PAGE_AMOUNT;
			canva->fixed_scr ();
			}
		if (updline_cnt) {
			lcd->RefreshAllDisplay_L (canva);
			updline_cnt--;
			}
		}
}




/**
  * @brief  Switch the PLL source from HSI to HSE, and select the PLL as SYSCLK
  *         source.
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 84000000
  *            HCLK(Hz)                       = 84000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClockHSE_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
  /* -1- Select HSI as system clock source to allow modification of the PLL configuration */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  
  /* -2- Enable HSE Oscillator, select it as PLL source and finally activate the PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  
  /* -3- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  
  /* -4- Optional: Disable HSI Oscillator (if the HSI is no more needed by the application)*/
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

}
