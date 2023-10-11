#include "stm32f4xx_hal.h"
#include "TAIN.h"
#include "TcontRect.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"
#include "ST7565R_SPI.H"
#include "LCD132x64.h"


void SystemClockHSE_Config (void);
static S_GPIOPIN pinphase_in_a = {};
static S_GPIOPIN pinphase_out_a = {};
static const S_GPIOPIN rawpins[EST7565P_ENDENUM] = {/*EST7565P_CS*/{GPIOA,GPIO_PIN_11}, /*EST7565P_REST*/{GPIOA, GPIO_PIN_10}, /*EST7565P_RS*/{GPIOA, GPIO_PIN_9},\
/*EST7565P_SCL*/ {GPIOA, GPIO_PIN_8}, /*EST7565P_SI*/{GPIOB, GPIO_PIN_15}};
static TCONTRECT *rectifier;
static TST7565RSPI *lcd;
static TLCDCANVABW *canva;
	
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



int main ()
{
	SystemClockHSE_Config ();
	canva = new TLCDCANVABW ();
	lcd = new TST7565RSPI (const_cast<S_GPIOPIN*>(rawpins));
	rectifier = new TCONTRECT (&pinphase_in_a, &pinphase_out_a, ESYSTIM_TIM2, 1000000, 10);
	while (true)
		{
		frendly_task ();
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
