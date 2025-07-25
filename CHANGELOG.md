# Change Log
All notable changes in Bluetooth Vendor Hardware Abstraction Layer(BT VHAL) will be documented in this file.
## [009.032] - 2025-June-21

### Added
- Added support of 4M baudrate for firmware download process and HCI
  communications.

## [009.031] - 2025-May-21

### Added
- Optimized VHAL performance by replacing inefficient busy-wait loops in HCI
  event handling with a poll()-based mechanism and eliminating redundant HCI
  reset at 115200 baud, reducing initialization time and improving responsiveness.
- Added support of BLE Coded PHY TX Power Configuration using power config file
  via pFilename_tx_power.

## [009.029] - 2024-October-10

### Added
- Improved Inband IR recovery by resetting the IR configuration property on
  failure and introducing a PDn trigger mechanism to ensure reliable bluetooth
  recovery.

## [009.028] - 2024-October-01

### Added
- Reduced Bluetooth VHAL system property flag string length under 31 bytes to
  meet some customer platform system property size limitation.

## [009.027] - 2024-September-13

### Added
- Added support to configure BT/BLE Power configurations using power
  configuration file via pFilename_tx_power.
- Deprecated ble_1m_power, ble_2m_power, bt_max_power_sel configurations
  from bt_vendor.conf

## [009.024] - 2024-January-04

### Fixed
- Fixed Code sonar warnings

## [009.023] - 2023-November-09

### Fixed
- CRC error seen during firmware download

## [009.022] - 2023-October-20

### Fixed
- Fixed Code Sonar warnings

## [009.021] - 2023-September-01

### Fixed
- Correction of typos

## [009.019] - 2023-July-27

### Fixed
- Added sign compare in build flag and fixed related issues

## [009.017] - 2023-June-13

### Added
- Updated code to send exit heartbeat mode command to controller on BT off event
- Added wakeup_enable_uart_low_config parameter to pull controller specific UART lines to low when HEARTBEAT timer is timed out on controller.
- Updated code in BT off event to avoid unexpected FW condition and data on BT enable event

## [009.016] - 2023-March-24

### Fixed
- Fixed static code analysis errors

## [009.015] - 2023-March-24

### Added
- Added Low Power Mode(LPM) support via Inband method

## [009.013] - 2022-Oct-13

### Fixed
- Fixed all coverity issues.

## [009.012] - 2022-Sep-30

### Fixed
- Updated Makefile to fix BT VHAL compilation error for Android-13 codebase

## [009.011] - 2022-Sep-22

### Added
- Added Boot Sleep Patch support for Android Auto
 
## [009.010] - 2022-Aug-26

### Fixed
- Fixed Inband Independent Reset Trigger for Android Automotive

## [009.008] - 2022-July-15

### Added
- Code updated to use unanimous function for logging and support 5 log level.

## [009.006] - 2022-Jun-03

### Fixed
- Remove extra wait if Firmware is already downloaded to reduce BT turn on time.

## [009.005] - 2022-Apr-21

### Fixed
- Removed compile time flag wrt BLE wake on feature for OTT project.

## [009.003] - 2022-Apr-01

### Added
- Automatically select firmware name from BT VHAL for Chips supporting boot code version 3

## [009.000] - 2022-Feb-04
 
### Added
- Remove compile time feature flag and make it runtime configurable.

### Fixed
- Add Poke command to speed up Firmware download for latest Chipset.

## [008.018] - 2022-Jan-07
 
### Fixed
- Handle "Non-empty else"" log print, by cleaning extra message in queue.
  
## [008.015] - 2021-Oct-20
 
### Added
- BLE wake on feature for OTT project

## [008.014] - 2021-Oct-14
 
### Added
- Inband Independent Reset in BT Vendor HAL.

## [008.013] - 2021-Sep-17

### Fixed
- Added FIFO check before reading data to avoid extra time for Change Buadrate Command.

## [008.011] - 2021-Aug-01
 
### Added
- Support user space host GPIO toggling from VHAL for triggering Out of band Independent Reset om imx platform.

## [008.010] - 2021-Jul-30
 
### Added
- Enable BT Tx Power Set capability in VHAL

## [008.009] - 2021-Jun-24
 
### Fixed
- Avoid going in infinite loop incase bootloader signature is not received during stress test.
- Handle FD leak causing BT crash during stress test.
- Added Delay after Change Buadrate to provide sufficient time for command to complete.

## [008.007] - 2021-May-19
 
### Added
- Support Out of Band(OOB) Independent Reset(IR) in vendor HAL
- Integration of RFKILL patch to toggle GPIO for OOB IR
- Print BT firmware version from vendor HAL

## [008.005] - 2021-May-18

### Added
 - Set BLE TX power level through vendor HAL for 1M and 2M PHY

## [008.004] - 2021-Apr-26

### Added
 - Upgraded Firmware Loader version to M322.

## [008.003] - 2021-Apr-08
 
### Added
- Loading BT Calibration Data via BT Vendor HAL
