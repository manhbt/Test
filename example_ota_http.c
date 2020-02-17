#if defined(CONFIG_PLATFORM_8711B)
#include "rtl8710b_ota.h"
#include <FreeRTOS.h>
#include <task.h>
#elif defined(CONFIG_PLATFORM_8195A)
#include <ota_8195a.h>
#elif defined(CONFIG_PLATFORM_8195BHP)
#include <ota_8195b.h>
#elif defined(CONFIG_PLATFORM_8710C)
#include <ota_8710c.h>
#endif
#include <osdep_service.h>

#define PORT	8060
#define HOST	"27.71.226.68"  //"m-apps.oss-cn-shenzhen.aliyuncs.com"
#define RESOURCE "/rest/v1/softwaremodules/64/artifacts/122/download"     //"051103061600.bin"
  
#ifdef HTTP_OTA_UPDATE
void http_update_ota_task(void *param){

#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1)
	rtw_create_secure_context(configMINIMAL_SECURE_STACK_SIZE);
#endif
	
	int ret = -1;
	
    vTaskDelay(5000);
#if 1
    extern int http_update_ota_clone(char *host, int port, char *resource);
    ret = http_update_ota_clone(HOST, PORT, RESOURCE); 
#else
	ret = http_update_ota(HOST, PORT, RESOURCE);        // origin
#endif
    
	printf("\n\r[%s] Update task exit", __FUNCTION__);
	if(!ret)
    {
		printf("\n\r[%s] Ready to reboot", __FUNCTION__);	
        vTaskDelay(100);
		ota_platform_reset();
	}
	vTaskDelete(NULL);	
}


void example_ota_http(void){
		if(xTaskCreate(http_update_ota_task, (char const *)"http_update_ota_task", 4096, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS){
		printf("\n\r[%s] Create update task failed", __FUNCTION__);
	}
}
#endif

