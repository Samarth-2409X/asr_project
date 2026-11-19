# AUTOSAR-Compliant Electronic Fan Speed Controller

## Project Overview

This project focuses on the design, development, and validation of an **AUTOSAR-compliant Electronic Fan Speed Controller** using the **Infineon AURIX TC3xx microcontroller**.

The system reads an analog input from a sensor or potentiometer using the ADC, processes the acquired value in the application software, and controls the fan speed through PWM.

The project follows the AUTOSAR layered software architecture to achieve modularity, reusability, maintainability, and hardware abstraction.

## Objectives

- Develop an AUTOSAR-compliant electronic fan speed controller.
- Acquire analog input using the ADC driver.
- Process sensor data through application software components.
- Control fan speed using PWM.
- Implement a layered architecture consisting of MCAL, IoHwAb, and ASW.
- Perform unit testing and integration testing.
- Maintain traceability between requirements, design, implementation, and validation.

## Hardware and Software

### Hardware
- Infineon AURIX TC3xx evaluation board.
- Analog input source (potentiometer or temperature sensor).
- Electronic fan and suitable driver circuit.
- Required power supply and interfacing components.

### Software and Technologies
- Embedded C
- AUTOSAR Classic Platform
- Infineon AURIX Development Studio
- Visual Studio Code
- ADC and PWM drivers
- Unit testing and integration testing tools

## System Architecture

The system follows a layered architecture:

1. **Application Software (ASW):** Processes sensor data and determines the required fan speed.
2. **I/O Hardware Abstraction (IoHwAb):** Provides an abstraction between application software and hardware-specific I/O operations.
3. **Microcontroller Abstraction Layer (MCAL):** Provides low-level peripheral drivers such as ADC, PWM, MCU, and Port.

### System Working

1. The ADC reads the analog input from the sensor or potentiometer.
2. The ADC converts the analog signal into a digital value.
3. IoHwAb provides the acquired value to the application layer.
4. SensorSWC processes the input value.
5. ActuatorSWC determines the required PWM duty cycle.
6. The PWM driver generates the corresponding signal to control fan speed.

## Repository Structure

```text
asr_project/
│
├── Architecture_Design/
│   └── Adc/
│       └── Adc_Architecture_Design.docx
│
├── Design/
│   └── Adc/
│
├── Development/
│   ├── Asw/
│   │   ├── ActuatorSwc/
│   │   └── SensorSwc/
│   │
│   ├── IoHwAb/
│   │   └── IoHwAb_Adc/
│   │
│   ├── Mcal/
│   │   ├── Adc/
│   │   │   ├── Adc.c
│   │   │   └── Adc.h
│   │   │
│   │   ├── cfg/
│   │   │   ├── Adc_Cfg.h
│   │   │   ├── Adc_PBcfg.c
│   │   │   └── Adc_PBcfg.h
│   │   │
│   │   ├── Interrupt/
│   │   ├── Mcu/
│   │   ├── Port/
│   │   └── Pwm/
│   │
│   ├── Scheduling/
│   │   └── Stm/
│   │
│   └── Startup/
│
├── Requirements/
│   ├── AUTOSAR_CP_SWS_ADCDriver.pdf
│   ├── AUTOSAR_CP_SWS_BSWModeManager.pdf
│   ├── AUTOSAR_CP_SWS_IOHardwareAbstraction.pdf
│   ├── AUTOSAR_CP_SWS_MCUDriver.pdf
│   ├── AUTOSAR_CP_SWS_PortDriver.pdf
│   └── AUTOSAR_CP_SWS_PWMDriver.pdf
│
├── Traceability/
│   ├── ArchToVal/
│   ├── DesignToVal/
│   └── ReqToVal/
│
├── UserManual/
│
└── Validation/
    ├── IntegrationTest/
    │   ├── Adc/
    │   │   └── Test_Adc.c
    │   └── Pwm/
    │
    └── UnitTest/
        ├── Adc/
        └── Pwm/
```

## Repository Directory Description

| Directory | Description |
|---|---|
| `Architecture_Design/` | Contains architectural design documents. |
| `Design/` | Contains detailed software design artifacts. |
| `Development/` | Contains the project source code. |
| `Development/Asw/` | Contains application software components. |
| `Development/IoHwAb/` | Contains I/O hardware abstraction modules. |
| `Development/Mcal/` | Contains microcontroller abstraction layer drivers. |
| `Development/Scheduling/` | Contains scheduling-related implementation. |
| `Development/Startup/` | Contains startup-related implementation. |
| `Requirements/` | Contains AUTOSAR software specifications and project requirements. |
| `Traceability/` | Contains requirement-to-validation, design-to-validation, and architecture-to-validation traceability artifacts. |
| `UserManual/` | Intended for user and setup documentation. |
| `Validation/UnitTest/` | Contains unit test source files. |
| `Validation/IntegrationTest/` | Contains integration test source files. |

## Development and Validation Approach

The project follows a modular development and validation workflow.

### Development Workflow

1. Individual developers implement their assigned software modules.
2. Each module is integrated with the relevant project components.
3. The integrated code is compiled.
4. The compiled software is released to the validation team.

### Validation Workflow

1. The validation team receives the released software.
2. Validation engineers integrate the assigned test modules.
3. The test code is compiled.
4. Unit testing and integration testing are performed.
5. Test results are documented and released.

### Unit Testing

Unit testing verifies individual software units independently.

- Tests individual APIs and functions.
- Uses simulators rather than target hardware, as specified in the project testing workflow.
- Checks valid inputs, invalid inputs, boundary conditions, and error handling.
- Uses mocks or stubs wherever dependencies need to be isolated.

### Integration Testing

Integration testing verifies the interaction between software modules and their dependencies.

- Tests interactions between related APIs and modules.
- Checks initialization sequences and dependencies.
- Verifies correct data flow and expected behavior across integrated components.
- Documents test cases, expected results, and actual results.

## Requirements and Documentation

The `Requirements/` directory contains the AUTOSAR specifications used as references for implementation and validation.

The `Traceability/` directory maintains links between:
- Architecture and validation.
- Design and validation.
- Requirements and validation.

These artifacts help ensure that project requirements are addressed during implementation and testing.

## Project Status

The repository is organized into architecture design, software design, development, requirements, traceability, user documentation, and validation directories.

Implementation and validation activities are organized module-wise, with ADC and PWM as key peripheral areas.

## Contributors

**Project:** AUTOSAR-Compliant Electronic Fan Speed Controller  
**Platform:** Infineon AURIX TC3xx  
**Department:** Electrical and Electronics Engineering  
**Institution:** KLE Technological University

---

*This repository is maintained for the development, integration, testing, and documentation of the AUTOSAR-compliant Electronic Fan Speed Controller project.*