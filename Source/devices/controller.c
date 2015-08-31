#include "controller.h"
#include "protocol.h"
#include "frequency.h"
#include "ADS1220.h"
#include "rtc.h"
#include "fram_i2c.h"
#include "log.h"
#include "watchdog.h"

xSemaphoreHandle xMeasureDataMutex;
xSemaphoreHandle xSettingsMutex;

stControllerSettings stSettings;
const stControllerSettings stSettingsDefault={TCXO_FREQ_DEFAULT};
stControllerMeasureData stMeasureData;

uint8_t Controller_RestoreSettings(void);

void ControllerInit(void)
{
	xMeasureDataMutex=xSemaphoreCreateMutex() ;
	xSettingsMutex=xSemaphoreCreateMutex() ;
	//Watchdog_Init();
	FRAM_I2C_Init();
	Controller_RestoreSettings();
	RTC_Clock_Init();
	Protocol_Init();
	FrequencyMeasureInit();
//	ADS1220_init();
	//Log_Init();
}

uint8_t Controller_RestoreSettings(void)
{
	FRAM_Read_Settings(&stSettings);
	if((stSettings.TCXO_frequency<TCXO_FREQ_MIN) || (stSettings.TCXO_frequency>TCXO_FREQ_MAX))
	{
		stSettings.TCXO_frequency=TCXO_FREQ_DEFAULT;
	}

	return 0;
}
