/* [SWS_Adc_00367] AUTOSAR Classic Platform R25-11 ADC Driver - Adc_StartGroupConversion Implementation (TC39x EVADC CPU0) */

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
static Adc_StateType         Adc_DriverState = ADC_STATE_UNINIT;
static Adc_GroupRuntimeType  Adc_GroupRuntime[ADC_MAX_GROUPS_PER_HW_UNIT];
static const Adc_ConfigType* Adc_ConfigPtr = NULL_PTR;

/* [SWS_Adc_00367] 8.3.4 Starts the conversion of all channels of the requested ADC Channel group (Service ID: 0x02U) */
void Adc_StartGroupConversion(
    Adc_GroupType Group
)
{
    /* [SWS_Adc_00294] [SWS_Adc_91005] Check if ADC driver has been initialized */
    if (Adc_DriverState == ADC_STATE_UNINIT)
    {
#if (ADC_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_START_GROUP_CONVERSION, ADC_E_UNINIT);
#endif
    }
    /* [SWS_Adc_00125] [SWS_Adc_91005] Validate requested ADC channel group ID */
    else if (Group >= ADC_MAX_GROUPS_PER_HW_UNIT)
    {
#if (ADC_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_START_GROUP_CONVERSION, ADC_E_PARAM_GROUP);
#endif
    }
    /* [SWS_Adc_00424] [SWS_Adc_91005] Check if result buffer has been initialized for the group */
    else if (Adc_GroupRuntime[Group].ResultBufferPtr == NULL_PTR)
    {
#if (ADC_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_START_GROUP_CONVERSION, ADC_E_BUFFER_UNINIT);
#endif
    }
    /* [SWS_Adc_00133] [SWS_Adc_91005] Check if trigger source is configured as Software API (ADC_TRIGG_SRC_SW) */
    else if ((Adc_ConfigPtr != NULL_PTR) &&
             (Adc_ConfigPtr->AdcHwUnit.AdcGroup[Group].AdcGroupTriggSrc != ADC_TRIGG_SRC_SW))
    {
#if (ADC_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_START_GROUP_CONVERSION, ADC_E_WRONG_TRIGG_SRC);
#endif
    }
    /* [SWS_Adc_00346] [SWS_Adc_00426] [SWS_Adc_91006] Report runtime error ADC_E_BUSY if group is not in ADC_IDLE state */
    else if (Adc_GroupRuntime[Group].GroupStatus != ADC_IDLE)
    {
        (void)Det_ReportRuntimeError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_START_GROUP_CONVERSION, ADC_E_BUSY);
    }
    else
    {
        /* [SWS_Adc_00061] Transition group status from ADC_IDLE to ADC_BUSY */
        Adc_GroupRuntime[Group].GroupStatus = ADC_BUSY;

        /* Trigger Infineon AURIX TC39x EVADC Group 0 Queued/Arbitration Request Source (Software Trigger) */
        /* Note: In TC39x EVADC, setting G0QMR0.TREV = 1 triggers a software conversion event for Queue 0 */
        /* G0QINR0 = Adc_ConfigPtr->AdcHwUnit.AdcGroup[Group].AdcChannel.AdcChannelId; */
        /* G0QMR0.B.TREV = 1U; */
    }
}
