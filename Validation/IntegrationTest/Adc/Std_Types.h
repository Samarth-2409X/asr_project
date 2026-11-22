#ifndef STD_TYPES_H
#define STD_TYPES_H

/* AUTOSAR Standard Primitive Types */
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned long       uint32;
typedef unsigned char       boolean;

/* [SWS_Adc_00364] Standard AUTOSAR Return Type */
typedef uint8               Std_ReturnType;

#ifndef TRUE
  #define TRUE              ((boolean)1U)
#endif

#ifndef FALSE
  #define FALSE             ((boolean)0U)
#endif

#ifndef E_OK
  #define E_OK              ((Std_ReturnType)0x00U)
#endif

#ifndef E_NOT_OK
  #define E_NOT_OK          ((Std_ReturnType)0x01U)
#endif

#ifndef STD_ON
  #define STD_ON            (0x01U)
#endif

#ifndef STD_OFF
  #define STD_OFF           (0x00U)
#endif

#ifndef NULL_PTR
  #define NULL_PTR          ((void*)0)
#endif

#endif /* STD_TYPES_H */
