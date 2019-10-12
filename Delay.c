#include "delay.h"
#include "sys_time.h"
#if 1

void Timer0_Delay1ms(uint32_t nCount)
{
        uint32_t Get;
				Get = TimeGetTime();
				while((Tim1GetDuration(Get)) < nCount);
				/*while(1)
				{
					FB = Tim1GetDuration(Get);
					if(FB >= nCount)
					{
						nCount = 0;
						Get = 0;
						//FB = 0;
						return;
					}
				}*/
				
}
#endif
