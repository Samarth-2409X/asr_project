#include "Det.h"

Det_ErrorLogType Det_DevErrorLog     = { 0U, 0U, 0U, 0U, 0UL };
Det_ErrorLogType Det_RuntimeErrorLog = { 0U, 0U, 0U, 0U, 0UL };

void Det_ResetLogs(void)
{
    Det_DevErrorLog.ModuleId       = 0U;
    Det_DevErrorLog.InstanceId     = 0U;
    Det_DevErrorLog.ApiId          = 0U;
    Det_DevErrorLog.ErrorId        = 0U;
    Det_DevErrorLog.ErrorCount     = 0UL;

    Det_RuntimeErrorLog.ModuleId   = 0U;
    Det_RuntimeErrorLog.InstanceId = 0U;
    Det_RuntimeErrorLog.ApiId      = 0U;
    Det_RuntimeErrorLog.ErrorId    = 0U;
    Det_RuntimeErrorLog.ErrorCount = 0UL;
}

/* [SWS_Adc_00377] Captures Development Errors ([SWS_Adc_91005]) */
Std_ReturnType Det_ReportError(
    uint16 ModuleId,
    uint8  InstanceId,
    uint8  ApiId,
    uint8  ErrorId
)
{
    Det_DevErrorLog.ModuleId   = ModuleId;
    Det_DevErrorLog.InstanceId = InstanceId;
    Det_DevErrorLog.ApiId      = ApiId;
    Det_DevErrorLog.ErrorId    = ErrorId;
    Det_DevErrorLog.ErrorCount++;

    return E_OK;
}

/* [SWS_Adc_00530] Captures Runtime Errors ([SWS_Adc_91006]) */
Std_ReturnType Det_ReportRuntimeError(
    uint16 ModuleId,
    uint8  InstanceId,
    uint8  ApiId,
    uint8  ErrorId
)
{
    Det_RuntimeErrorLog.ModuleId   = ModuleId;
    Det_RuntimeErrorLog.InstanceId = InstanceId;
    Det_RuntimeErrorLog.ApiId      = ApiId;
    Det_RuntimeErrorLog.ErrorId    = ErrorId;
    Det_RuntimeErrorLog.ErrorCount++;

    return E_OK;
}
