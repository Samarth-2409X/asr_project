#include "Det.h"

uint16 Det_LastModuleId   = 0U;
uint8  Det_LastInstanceId = 0U;
uint8  Det_LastApiId      = 0U;
uint8  Det_LastErrorId    = 0U;
uint32 Det_ErrorCount     = 0U;

uint16 Det_LastRuntimeModuleId   = 0U;
uint8  Det_LastRuntimeInstanceId = 0U;
uint8  Det_LastRuntimeApiId      = 0U;
uint8  Det_LastRuntimeErrorId    = 0U;
uint32 Det_RuntimeErrorCount     = 0U;

Std_ReturnType Det_ReportError(
    uint16 ModuleId,
    uint8 InstanceId,
    uint8 ApiId,
    uint8 ErrorId
)
{
    Det_LastModuleId   = ModuleId;
    Det_LastInstanceId = InstanceId;
    Det_LastApiId      = ApiId;
    Det_LastErrorId    = ErrorId;
    Det_ErrorCount++;

    return E_OK;
}

Std_ReturnType Det_ReportRuntimeError(
    uint16 ModuleId,
    uint8 InstanceId,
    uint8 ApiId,
    uint8 ErrorId
)
{
    Det_LastRuntimeModuleId   = ModuleId;
    Det_LastRuntimeInstanceId = InstanceId;
    Det_LastRuntimeApiId      = ApiId;
    Det_LastRuntimeErrorId    = ErrorId;
    Det_RuntimeErrorCount++;

    return E_OK;
}