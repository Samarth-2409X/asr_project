/* [SWS_Adc_00364] AUTOSAR Classic Platform R25-11 ADC Driver Header File (TC39x EVADC CPU0) */
#ifndef ADC_H
#define ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* [SWS_Adc_00364] Include AUTOSAR Standard Types */
#include "Std_Types.h"

/* [SWS_BSW_00059] Module and Vendor Identification */
#define ADC_VENDOR_ID                           (17U)
#define ADC_MODULE_ID                           (123U)
#define ADC_INSTANCE_ID                         (0U)

/* [SWS_BSW_00059] AUTOSAR Release R25-11 Version Information */
#define ADC_AR_RELEASE_MAJOR_VERSION            (25U)
#define ADC_AR_RELEASE_MINOR_VERSION            (11U)
#define ADC_AR_RELEASE_REVISION_VERSION         (0U)

/* [SWS_BSW_00059] Software Module Version Information */
#define ADC_SW_MAJOR_VERSION                    (1U)
#define ADC_SW_MINOR_VERSION                    (0U)
#define ADC_SW_PATCH_VERSION                    (0U)

/* Infineon AURIX TC39x EVADC Hardware Limits (Single Core CPU0) */
#define ADC_MAX_CHANNELS_PER_HW_UNIT            (2U)
#define ADC_MAX_GROUPS_PER_HW_UNIT              (2U)

/* Service IDs for Scoped AUTOSAR SWS Section 8.3 APIs */
#define ADC_SID_INIT                            (0x00U) /* [SWS_Adc_00365] Adc_Init */
#define ADC_SID_START_GROUP_CONVERSION          (0x02U) /* [SWS_Adc_00367] Adc_StartGroupConversion */
#define ADC_SID_STOP_GROUP_CONVERSION           (0x03U) /* [SWS_Adc_00368] Adc_StopGroupConversion */
#define ADC_SID_READ_GROUP                      (0x04U) /* [SWS_Adc_00369] Adc_ReadGroup */
#define ADC_SID_ENABLE_GROUP_NOTIFICATION       (0x07U) /* [SWS_Adc_91003] Adc_EnableGroupNotification */
#define ADC_SID_GET_GROUP_STATUS                (0x09U) /* [SWS_Adc_00374] Adc_GetGroupStatus */
#define ADC_SID_SETUP_RESULT_BUFFER             (0x0CU) /* [SWS_Adc_91000] Adc_SetupResultBuffer */

/* [SWS_Adc_91005] DET Development Error Codes */
#define ADC_E_UNINIT                            (0x0AU) /* API service called prior to initialization */
#define ADC_E_ALREADY_INITIALIZED               (0x0DU) /* Adc_Init called while already initialized */
#define ADC_E_PARAM_POINTER                     (0x14U) /* API service called with invalid/NULL pointer */
#define ADC_E_PARAM_GROUP                       (0x15U) /* Non-existing ADC group ID requested */
#define ADC_E_WRONG_CONV_MODE                   (0x16U) /* API called on group with wrong conversion mode */
#define ADC_E_WRONG_TRIGG_SRC                   (0x17U) /* API called on group with wrong trigger source */
#define ADC_E_NOTIF_CAPABILITY                  (0x18U) /* Notification enabled for group with NULL callback */
#define ADC_E_BUFFER_UNINIT                     (0x19U) /* Conversion started with uninitialized result buffer */

/* [SWS_Adc_91006] Runtime Error Codes */
#define ADC_E_BUSY                              (0x0BU) /* Conversion or setup requested while group is busy */
#define ADC_E_IDLE                              (0x0CU) /* Stop or read requested while group is idle */
#define ADC_E_NOT_DISENGAGED                    (0x1AU) /* Group(s) not idle during power state transition */
#define ADC_E_TRANSITION_NOT_POSSIBLE           (0x1CU) /* Requested power state transition not possible */

/* [SWS_Adc_00506] Numeric ID of an ADC channel */
typedef uint16 Adc_ChannelType;

/* [SWS_Adc_00507] Numeric ID of an ADC channel group */
typedef uint16 Adc_GroupType;

/* [SWS_Adc_00508] Type for reading converted values of a channel group (12-bit TC39x EVADC result) */
typedef uint16 Adc_ValueGroupType;

/* [SWS_Adc_00511] Type of sampling time (e.g., 1 -> 150ns in EVADC GxICLASSi.STCS) */
typedef uint8 Adc_SamplingTimeType;

/* [SWS_Adc_00518] Number of group conversions in streaming access mode */
typedef uint8 Adc_StreamNumSampleType;

/* [SWS_Adc_00513] Current conversion status of the requested ADC Channel group */
typedef enum Adc_StatusType_Tag
{
    ADC_IDLE             = 0x00U,
    ADC_BUSY             = 0x01U,
    ADC_COMPLETED        = 0x02U,
    ADC_STREAM_COMPLETED = 0x03U
} Adc_StatusType;

/* [SWS_Adc_00514] Type for configuring the trigger source for an ADC Channel group */
typedef enum Adc_TriggerSourceType_Tag
{
    ADC_TRIGG_SRC_SW = 0x00U,
    ADC_TRIGG_SRC_HW = 0x01U
} Adc_TriggerSourceType;

/* Alias matching sample Adc.h trigger source type name */
typedef Adc_TriggerSourceType enum_AdcGroupTriggSrc;

/* [SWS_Adc_00515] Type for configuring the conversion mode of an ADC Channel group */
typedef enum Adc_GroupConvModeType_Tag
{
    ADC_CONV_MODE_ONESHOT    = 0x00U,
    ADC_CONV_MODE_CONTINUOUS = 0x01U
} Adc_GroupConvModeType;

/* Alias matching sample Adc.h conversion mode type name */
typedef Adc_GroupConvModeType enum_AdcGroupConversionMode;

/* [SWS_Adc_00519] Type for configuring the streaming access mode buffer type */
typedef enum Adc_StreamBufferModeType_Tag
{
    ADC_STREAM_BUFFER_LINEAR   = 0x00U,
    ADC_STREAM_BUFFER_CIRCULAR = 0x01U
} Adc_StreamBufferModeType;

/* Alias matching sample Adc.h streaming buffer mode type name */
typedef Adc_StreamBufferModeType enum_AdcStreamingBufferMode;

/* [SWS_Adc_00528] Type for configuring the access mode to group conversion results */
typedef enum Adc_GroupAccessModeType_Tag
{
    ADC_ACCESS_MODE_SINGLE    = 0x00U,
    ADC_ACCESS_MODE_STREAMING = 0x01U
} Adc_GroupAccessModeType;

/* Alias matching sample Adc.h group access mode type name */
typedef Adc_GroupAccessModeType enum_AdcGroupAccessMode;

/* [SWS_Adc_00525] Type for alignment of ADC raw results in ADC result buffer */
typedef enum Adc_ResultAlignmentType_Tag
{
    ADC_ALIGN_LEFT  = 0x00U,
    ADC_ALIGN_RIGHT = 0x01U
} Adc_ResultAlignmentType;

/* [SWS_Adc_00084] Callback function pointer type for group conversion notification */
typedef void (*Adc_NotificationType)(void);

/* [ECUC_Adc_00244] Individual ADC Channel Configuration Container */
typedef struct AdcChannelType_Tag
{
    Adc_SamplingTimeType        AdcChannelSampTime; /* [SWS_Adc_00511] Sample time control (1 -> 150ns) */
    Adc_ChannelType             AdcChannelId;       /* [SWS_Adc_00506] Physical EVADC channel ID */
} AdcChannelType;

/* [ECUC_Adc_00243] ADC Channel Group Configuration Container */
typedef struct AdcGroupType_Tag
{
    enum_AdcGroupAccessMode     AdcGroupAccessMode;     /* [SWS_Adc_00528] Single or Streaming access mode */
    enum_AdcGroupConversionMode AdcGroupConversionMode; /* [SWS_Adc_00515] One-shot or Continuous mode */
    Adc_GroupType               AdcGroupId;             /* [SWS_Adc_00507] Numeric ID of the group */
    enum_AdcGroupTriggSrc       AdcGroupTriggSrc;       /* [SWS_Adc_00514] Software or Hardware trigger */
    Adc_NotificationType        AdcNotification;        /* [SWS_Adc_00084] Group notification callback */
    enum_AdcStreamingBufferMode AdcStreamingBufferMode; /* [SWS_Adc_00519] Linear or Circular stream buffer */
    Adc_StreamNumSampleType     AdcStreamingNumSamples; /* [SWS_Adc_00518] Number of streaming samples */
    AdcChannelType              AdcChannel;             /* [ECUC_Adc_00244] Assigned channel configuration */
} AdcGroupType;

/* [ECUC_Adc_00242] ADC Hardware Unit (TC39x EVADC Kernel) Configuration Container */
typedef struct AdcHwUnitType_Tag
{
    uint32                      ClkSrc;                                   /* [ECUC_Adc_00489] EVADC clock source */
    uint32                      HwUnitId;                                 /* [ECUC_Adc_00492] EVADC HW unit ID */
    AdcChannelType              AdcChannel[ADC_MAX_CHANNELS_PER_HW_UNIT]; /* [ECUC_Adc_00244] Configured channels */
    AdcGroupType                AdcGroup[ADC_MAX_GROUPS_PER_HW_UNIT];     /* [ECUC_Adc_00243] Configured groups */
} AdcHwUnitType;

/* [SWS_Adc_00505] Root ADC Driver Configuration Data Structure */
typedef struct Adc_ConfigType_Tag
{
    AdcHwUnitType               AdcHwUnit; /* [ECUC_Adc_00242] Configured EVADC hardware unit container */
} Adc_ConfigType;

/* [SWS_Adc_00365] 8.3.1 Initializes the ADC hardware units and driver */
extern void Adc_Init(const Adc_ConfigType* ConfigPtr);

/* [SWS_Adc_91000] 8.3.2 Initializes the group conversion result buffer pointer */
extern Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

/* [SWS_Adc_00367] 8.3.4 Starts the conversion of all channels of the requested ADC Channel group */
extern void Adc_StartGroupConversion(Adc_GroupType Group);

/* [SWS_Adc_00368] 8.3.5 Stops the conversion of the requested ADC Channel group */
extern void Adc_StopGroupConversion(Adc_GroupType Group);

/* [SWS_Adc_00369] 8.3.6 Reads the group conversion result of the last completed conversion round */
extern Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

/* [SWS_Adc_91003] 8.3.9 Enables the notification mechanism for the requested ADC Channel group */
extern void Adc_EnableGroupNotification(Adc_GroupType Group);

/* [SWS_Adc_00374] 8.3.11 Returns the conversion status of the requested ADC Channel group */
extern Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);

#ifdef __cplusplus
}
#endif

#endif /* ADC_H */
