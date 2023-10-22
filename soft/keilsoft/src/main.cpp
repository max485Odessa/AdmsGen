#include "stm32f4xx_hal.h"
#include "TAIN.h"
#include "TcontRect.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"
#include "ST7565R_SPI.H"
#include "LCD132x64.h"
#include "TEASYKEYS.h"
#include "TCORE.h"
#include "TM24Cxxx.h"
#include "I2CSOFTWARE.H"



void SystemClockHSE_Config (void);
static S_GPIOPIN pinphase_in_a = {};
static S_GPIOPIN pinphase_out_a = {};
	
static S_GPIOPIN pinsi2c_a[EARRI2CPIN_ENDENUM] = {{GPIOB,GPIO_PIN_3}/*scl*/, {GPIOB,GPIO_PIN_4}/*sda*/};
static const S_GPIOPIN rawpins_keys[EJSTCPINS_ENDENUM] = {{GPIOB,GPIO_PIN_5}, {GPIOB,GPIO_PIN_6}, {GPIOB,GPIO_PIN_7}, {GPIOB,GPIO_PIN_8}, {GPIOB,GPIO_PIN_9}};
static const S_GPIOPIN rawpins_lcd[EST7565P_ENDENUM] = {/*EST7565P_CS*/{GPIOA,GPIO_PIN_11}, /*EST7565P_REST*/{GPIOA, GPIO_PIN_10}, /*EST7565P_RS*/{GPIOA, GPIO_PIN_9},\
/*EST7565P_SCL*/ {GPIOA, GPIO_PIN_8}, /*EST7565P_SI*/{GPIOB, GPIO_PIN_15}};
static TCONTRECT *rectifier;
static TST7565RSPI *lcd;
static TLCDCANVABW *canva;
static TEASYKEYS *keys;
static TCORERCT *core;
static TI2CIFACE *busi2c;
static TM24C16 *memi2c;
	
#ifdef __cplusplus
 extern "C" {
#endif 



void SysTick_Handler(void)
{
  HAL_IncTick();
	SYSBIOS::EXECUTE_PERIODIC_ISR (1);
}



#ifdef __cplusplus
}
#endif



static void frendly_task ()
{
	SYSBIOS::EXECUTE_PERIODIC_SYS ();
	TFCC_MANAGER::Execute_Tasks ();
	
}



//static uint8_t testbufwr[512];
//static uint8_t testbufrd[512];



int main ()
{
	static uint8_t updline_cnt = 0;
	SystemClockHSE_Config ();
	keys = new TEASYKEYS (const_cast<S_GPIOPIN*>(rawpins_keys), EJSTCPINS_ENDENUM);
	// pinsi2c_a
	busi2c = new TI2CIFACE (pinsi2c_a, 50);
	memi2c = new TM24C16 (busi2c, 0);
	
	
	//memi2c->write (0, testbufwr, 512); 
	//memi2c->read (0, testbufrd, 512);

	canva = new TLCDCANVABW ();
	canva->Init ();
	lcd = new TST7565RSPI (const_cast<S_GPIOPIN*>(rawpins_lcd));
	lcd->LCD_init ();
	rectifier = new TCONTRECT (&pinphase_in_a, &pinphase_out_a, ESYSTIM_TIM2, 1000000, 10);
	
	canva->DrawCircle (20, 20, 10);
	
	core = new TCORERCT (rectifier, canva, keys, memi2c);
	updline_cnt = C_LCD_PAGE_AMOUNT;
	
	while (true)
		{
		frendly_task ();
			
		if (core->is_lcd_update ()) updline_cnt = C_LCD_PAGE_AMOUNT;
		if (updline_cnt)
			{
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
