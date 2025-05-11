#include "delay.h"
 
/**
* @brief 
* @param _nus 
* @return 
*/
void delay_us(uint32_t _nus)
{	
	
	uint32_t  fac_us=0;
    uint32_t ticks;
    uint32_t t_old,t_now,t_cnt=0;
    uint32_t reload=SysTick->LOAD;
	
	fac_us=SystemCoreClock/1000000;		
    ticks=_nus*fac_us-fac_us*7/20;
    t_old=SysTick->VAL;
	
    while(1)
    {
		
        t_now=SysTick->VAL;
		
		if(t_now<t_old)
		{
			t_cnt+=t_old-t_now;
		}
		else
		{
			t_cnt+=t_old+(reload-t_now);
		}
		
		if(t_cnt>=ticks)
		{
			break;
		}
		
		t_old=t_now;
    };
}
 
/**
* @brief 
* @param _nms 
* @return 
*/
void delay_ms(uint16_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) 
	{
		delay_us(1000);
	}
}