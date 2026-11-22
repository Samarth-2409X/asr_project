#include "Adc.h"
#include "Adc_Cfg.h"
#include "Adc_PBcfg.h"
#include "Det.h"

/* Integration Test Result Flags (TRUE = Passed, FALSE = Failed) */
volatile boolean TestResult_SWS_Adc_00301                   = FALSE;
volatile boolean TestResult_SWS_Adc_00054_NullPtr           = FALSE;
volatile boolean TestResult_SWS_Adc_00365_00054_00056_00511 = FALSE;
volatile boolean TestResult_SWS_Adc_00307_00513             = FALSE;
volatile boolean TestResult_SWS_Adc_00077_00083             = FALSE;
volatile boolean TestResult_SWS_Adc_00107                   = FALSE;

static uint32 NotifySensor_CallCount = 0UL;

/* [SWS_Adc_00082] Configurable Notification Callback Stub */
void NotifySensor(void)
{
    NotifySensor_CallCount++;
}

/* [SWS_Adc_00301] [SWS_Adc_91005]
 * Calling API prior to Adc_Init shall raise ADC_E_UNINIT (0x0A) and return ADC_IDLE.
 */
void Test_Adc_Init_UninitCheck(void)
{
    Adc_StatusType GroupStatus;

    Det_ResetLogs();
    GroupStatus = Adc_GetGroupStatus(ADC_GROUP_ID0);

    if ((GroupStatus                == ADC_IDLE) &&
        (Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId   == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId      == ADC_SID_GET_GROUP_STATUS) &&
        (Det_DevErrorLog.ErrorId    == ADC_E_UNINIT))
    {
        TestResult_SWS_Adc_00301 = TRUE;
    }
    else
    {
        TestResult_SWS_Adc_00301 = FALSE;
    }
}

/* [SWS_Adc_00054] [SWS_Adc_91005]
 * Calling Adc_Init with NULL_PTR in Variant PB shall raise ADC_E_PARAM_POINTER (0x14)
 * and leave EVADC hardware registers unmodified.
 */
void Test_Adc_Init_NullConfigPtr(void)
{
    Det_ResetLogs();
    Adc_Init(NULL_PTR);

    if ((Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId   == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId      == ADC_SID_INIT) &&
        (Det_DevErrorLog.ErrorId    == ADC_E_PARAM_POINTER) &&
        (G0ICLASS0.STCS.B           == 0UL) &&
        (G0ICLASS1.STCS.B           == 0UL))
    {
        TestResult_SWS_Adc_00054_NullPtr = TRUE;
    }
    else
    {
        TestResult_SWS_Adc_00054_NullPtr = FALSE;
    }
}

/* [SWS_Adc_00365] [SWS_Adc_00054] [SWS_Adc_00056] [SWS_Adc_00511]
 * Adc_Init shall enable EVADC clock, configure GLOBCFG clock source, program sampling
 * times (G0ICLASS0/1.STCS) and input class selections (G0CHCTR0/1.ICLSEL) for only
 * configured channels without raising DET errors.
 */
void Test_Adc_Init_ValidConfig(void)
{
    Det_ResetLogs();
    Adc_Init(&adcConfig);

    if ((Det_DevErrorLog.ErrorCount == 0UL) &&
        (CLC.DISR.B                 == 0UL) &&
        (CLC.DISS.B                 == 0UL) &&
        (GLOBCFG.U                  == (1UL << 15U)) &&
        (G0ICLASS0.STCS.B           == 1UL) &&
        (G0ICLASS1.STCS.B           == 2UL) &&
        (G0CHCTR0.ICLSEL.B          == 0UL) &&
        (G0CHCTR1.ICLSEL.B          == 1UL))
    {
        TestResult_SWS_Adc_00365_00054_00056_00511 = TRUE;
    }
    else
    {
        TestResult_SWS_Adc_00365_00054_00056_00511 = FALSE;
    }
}

/* [SWS_Adc_00307] [SWS_Adc_00513]
 * Adc_Init shall set all configured ADC channel groups to ADC_IDLE (0x00U).
 */
void Test_Adc_Init_GroupsIdle(void)
{
    Det_ResetLogs();

    if ((Adc_GetGroupStatus(ADC_GROUP_ID0) == ADC_IDLE) &&
        (Adc_GetGroupStatus(ADC_GROUP_ID1) == ADC_IDLE) &&
        (Det_DevErrorLog.ErrorCount        == 0UL))
    {
        TestResult_SWS_Adc_00307_00513 = TRUE;
    }
    else
    {
        TestResult_SWS_Adc_00307_00513 = FALSE;
    }
}

/* [SWS_Adc_00077] [SWS_Adc_00083]
 * Adc_Init shall disable group notifications and hardware trigger capability;
 * no notification callback shall be invoked during initialization.
 */
void Test_Adc_Init_NotificationsDisabled(void)
{
    if (NotifySensor_CallCount == 0UL)
    {
        TestResult_SWS_Adc_00077_00083 = TRUE;
    }
    else
    {
        TestResult_SWS_Adc_00077_00083 = FALSE;
    }
}

/* [SWS_Adc_00107] [SWS_Adc_91005]
 * Calling Adc_Init when already initialized shall raise ADC_E_ALREADY_INITIALIZED (0x0D)
 * and return without modifying configured EVADC registers.
 */
void Test_Adc_Init_AlreadyInitialized(void)
{
    Det_ResetLogs();
    Adc_Init(&adcConfig1);

    if ((Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId   == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId      == ADC_SID_INIT) &&
        (Det_DevErrorLog.ErrorId    == ADC_E_ALREADY_INITIALIZED) &&
        (G0ICLASS0.STCS.B           == 1UL) &&
        (G0ICLASS1.STCS.B           == 2UL) &&
        (GLOBCFG.U                  == (1UL << 15U)))
    {
        TestResult_SWS_Adc_00107 = TRUE;
    }
    else
    {
        TestResult_SWS_Adc_00107 = FALSE;
    }
}
