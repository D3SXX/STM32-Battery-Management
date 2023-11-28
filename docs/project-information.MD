# Description

The Batter Management System is for the VAMK 4th year course [IITS2207-3005 Embedded Systems Programming and Project](https://portal.vamk.fi/course/view.php?id=16051). The objective is for students get familiar with the modern CI/CD embedded system development. The team consists four students, and each student would take roles of project manager/scrum master, software designer, hardware designer.

STM32L152RE microcontroller board was used to monitor the status of four LiFePo4 cells in seriers such as voltage, current and temperature. Depends on the monitored status the uC would set battery current flow into charging-only, discharging-only or bidirectional. The modbus slave/server was implemented for hosting all the status value. A python modbus master/client was developed as a gateway for fetch status data from BMS and pushing it to the cloud for visualization. The cloud service used was Wapice IoT-Ticket 3.

# Requirement Specification

- MOSFET driven switch (both charging and discharging)
- Temperature monitoring
- Individual battery cell voltages
- STM32 controller with over/under voltage per cell, over/under temperature (charge: 0C to 40C and discharge: -20C to 40C)
- Communication of cell voltages over Modbus RTU
- Some kind of visualization of the Modbus data

# Hardware

- STM32 Nucleo L152RE.
- MAX3485 RS485 to USART converter.
- USB-RS485-WE-1800-BT USB to RS485 converter.
- FDS6898A MOSFETS.
- CD4051BE Multiplexer.
- LM324 OP AMPS.
- LM35 Ananlog Thermalmeter.
- Resisters.
- Breadboards.

# Environment

- Language - C
- Software
  - PlatformIO (STM32CUBE framework) for researching.
  - PlatformIO (CMSIS framework) for development.
  - Unity framwork for unit testing.
  - Python 3.11.6.
  - Minimum Modbus Library.
  - IoT-Ticket 3 Library.
  - Github for CI/CD.

# Coding Conventions

- See [cpp-naming-convention.MD](./cpp-naming-convention.MD)
- See [branch-naming.convention.MD](./branch-naming-convention.MD)

# Scrum

- See /scrum_reports

# Documentation index

- [Temperature Sensor Analysis](./temperature-sensors.MD)
- [Git Action for CI](./git-action-for-ci.MD)
- [Atolic TrueStudio Pipeline](./atollic-true-studio-pipeline.MD)
- [CD4051BE Multiplexer Overview](./CD4051BE-overview.md)
- [Multicell Battery Meter Module](./KiCAD%20Files/MultiCellBatteryMeter/README.MD)
- [Mosfet Power Control Module](./KiCAD%20Files/MosfetPowerControl/README.MD)
