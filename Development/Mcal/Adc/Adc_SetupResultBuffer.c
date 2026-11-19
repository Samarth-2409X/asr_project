/* [SWS_Adc_91000] AUTOSAR Classic Platform R25-11 ADC Driver - Adc_SetupResultBuffer Implementation (TC39x EVADC CPU0) */

#include "Adc.h"

#ifndef ADC_DEV_ERROR_DETECT
  #define ADC_DEV_ERROR_DETECT                  (STD_ON)
#endif

#include "Det.h"

/* [SWS_BSW_00071] Internal ADC Driver Initialization State */
typedef enum Adc_StateType_Tag
{
    ADC_STATE_UNINIT = 0x00U,
    ADC_STATE_INIT   = 0x01U
} Adc_StateType;

/* Per-Group Runtime State Structure ([SWS_Adc_00307], [SWS_Adc_00420]) */
typedef struct Adc_GroupRuntimeType_Tag
{
    Adc_StatusType      GroupStatus;         /* [SWS_Adc_00307] Current group conversion status */
    Adc_ValueGroupType* ResultBufferPtr;     /* [SWS_Adc_00420] Application result buffer pointer */
    boolean             NotificationEnabled; /* [SWS_Adc_00077] Group notification state */
} Adc_GroupRuntimeType;

/* Module Internal State Variables */
static Adc_StateType        Adc_DriverState = ADC_STATE_UNINIT;
static Adc_GroupRuntimeType Adc_GroupRuntime[ADC_MAX_GROUPS_PER_HW_UNIT];

/* [SWS_Adc_91000] 8.3.2 Initializes the group conversion result buffer pointer (Service ID: 0x0CU) */
Std_ReturnType Adc_SetupResultBuffer(
    Adc_GroupType Group,
    Adc_ValueGroupType* DataBufferPtr
)
{
    Std_ReturnType RetVal = E_NOT_OK;

    /* [SWS_Adc_00434] [SWS_Adc_91005] Check if ADC driver has been initialized */
    if (Adc_DriverState == ADC_STATE_UNINIT)
    {
#if (ADC_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_SETUP_RESULT_BUFFER, ADC_E_UNINIT);
#endif
    }
    /* [SWS_Adc_00423] [SWS_Adc_91005] Validate requested ADC channel group ID */
    else if (Group >= ADC_MAX_GROUPS_PER_HW_UNIT)
    {
#if (ADC_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_SETUP_RESULT_BUFFER, ADC_E_PARAM_GROUP);
#endif
    }
    /* [SWS_Adc_00457] [SWS_Adc_91005] Validate application result buffer pointer */
    else if (DataBufferPtr == NULL_PTR)
    {
#if (ADC_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_SETUP_RESULT_BUFFER, ADC_E_PARAM_POINTER);
#endif
    }
    /* [SWS_Adc_00433] [SWS_Adc_91006] Report runtime error ADC_E_BUSY if group is not in ADC_IDLE state */
    else if (Adc_GroupRuntime[Group].GroupStatus != ADC_IDLE)
    {
        (void)Det_ReportRuntimeError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_SETUP_RESULT_BUFFER, ADC_E_BUSY);
    }
    else
    {
        /* [SWS_Adc_00420] Initialize group result buffer start address */
        Adc_GroupRuntime[Group].ResultBufferPtr = DataBufferPtr;
        RetVal = E_OK;
    }

    return RetVal;
}
