#ifndef DET_H
#define DET_H

#include "Std_Types.h"

/* Spy variables so test cases can inspect the last reported error */
extern uint16 Det_LastModuleId;
extern uint8  Det_LastInstanceId;
extern uint8  Det_LastApiId;
extern uint8  Det_LastErrorId;
extern uint32 Det_ErrorCount;

extern uint16 Det_LastRuntimeModuleId;
extern uint8  Det_LastRuntimeInstanceId;
extern uint8  Det_LastRuntimeApiId;
extern uint8  Det_LastRuntimeErrorId;
extern uint32 Det_RuntimeErrorCount;

/* Standard AUTOSAR DET APIs expected by Adc driver (SWS Section 8.6.1 & 8.6.2) */
extern Std_ReturnType Det_ReportError(
    uint16 ModuleId,
    uint8 InstanceId,
    uint8 ApiId,
    uint8 ErrorId
);

extern Std_ReturnType Det_ReportRuntimeError(
    uint16 ModuleId,
    uint8 InstanceId,
    uint8 ApiId,
    uint8 ErrorId
);

#endif /* DET_H */