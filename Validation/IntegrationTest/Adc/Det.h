#ifndef DET_H
#define DET_H

#include "Std_Types.h"

/* [SWS_Adc_00377] Development Error Reporting Interface */
extern Std_ReturnType Det_ReportError(
    uint16 ModuleId,
    uint8  InstanceId,
    uint8  ApiId,
    uint8  ErrorId
);

/* [SWS_Adc_00530] Runtime Error Reporting Interface */
extern Std_ReturnType Det_ReportRuntimeError(
    uint16 ModuleId,
    uint8  InstanceId,
    uint8  ApiId,
    uint8  ErrorId
);

/* Shared DET Error Log Structure for Integration Validation */
typedef struct Det_ErrorLogType_Tag
{
    uint16 ModuleId;
    uint8  InstanceId;
    uint8  ApiId;
    uint8  ErrorId;
    uint32 ErrorCount;
} Det_ErrorLogType;

extern Det_ErrorLogType Det_DevErrorLog;
extern Det_ErrorLogType Det_RuntimeErrorLog;

extern void Det_ResetLogs(void);

#endif /* DET_H */
