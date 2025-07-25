/******************************************************************************
 *
 *  Copyright 2009-2024 NXP
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/******************************************************************************
 *
 *  Filename:      fw_loader_io.c
 *
 *  Description:   Kernel Input/Output functions
 *
 ******************************************************************************/

#define LOG_TAG "fw_loader_linux"

/*============================== Include Files ===============================*/
#include "fw_loader_io.h"

#include <cutils/properties.h>
#include <errno.h>
#include <string.h>

#include "bt_vendor_log.h"
/*================================== Macros ==================================*/
#define TIMEOUT_SEC 6

/*================================== Typedefs=================================*/

/*================================ Variables =================================*/

/*============================ Function Prototypes ===========================*/

/*============================== Coded Procedures ============================*/

/******************************************************************************
 *
 * Name: fw_upload_lenValid
 *
 * Description:
 *   This function validates the length from 5 bytes request.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   ucArray: store the 5 bytes request.
 *
 * Return Value:
 *   uiLenToSend: if the length is valid, get value from ucArray.
 *
 * Notes:
 *   None.
 *

*****************************************************************************/
bool fw_upload_lenValid(uint16* uiLenToSend, uint8* ucArray) {
  uint16 uiLen, uiLenComp;
  uint16 uiXorOfLen = 0xFFFF;
  uiLen = (uint16)((ucArray[1] & 0xFF) | ((ucArray[2] << 8) & 0xFF00));
  uiLenComp = (uint16)((ucArray[3] & 0xFF) | ((ucArray[4] << 8) & 0xFF00));
  // LEN valid if len & complement match
  if ((uiLen ^ uiLenComp) == uiXorOfLen)  // All 1's
  {
    *uiLenToSend = uiLen;
    return true;
  } else {
    VND_LOGE("Length and complement check failed uiLen = %d uiLenComp = %d",
             uiLen, uiLenComp);
    return false;
  }
}

/******************************************************************************
 *
 * Name: fw_upload_GetDataLen
 *
 * Description:
 *   This function gets buf data length.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   *buf: buffer that stores header and following data.
 *
 * Return Value:
 *   length of data part in the buffer.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
uint16 fw_upload_GetDataLen(uint8* buf) { return (buf[8] | (buf[9] << 8)); }

/******************************************************************************
 *
 * Name: fw_upload_DelayInMs
 *
 * Description:
 *   This function delays the execution of the program for the time
 *   specified in uiMs.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   uiMs - Delay in Milliseconds.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
void fw_upload_DelayInMs(uint32 uiMs) {
  struct timespec ReqTime;
  time_t sec;

  // Initialize to 0
  ReqTime.tv_sec = 0;
  ReqTime.tv_nsec = 0;

  // Calculate the Delay
  sec = (int)(uiMs / 1000);
  uiMs = uiMs - ((uint32)(sec * 1000));
  ReqTime.tv_sec = sec;
  ReqTime.tv_nsec = (long)(uiMs * 1000000L);  // 1 ms = 1000000 ns

  // Sleep
  while (nanosleep(&ReqTime, &ReqTime) == -1) {
    continue;
  }
}

/******************************************************************************
 *
 * Name: fw_upload_ComReadChar
 *
 * Description:
 *   Read a character from the port specified by nPortID.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   fd : Port ID.
 *
 * Return Value:
 *   Returns the character, if Successful.
 *   Returns -1 if no character available (OR TIMED-OUT)
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
uint8 fw_upload_ComReadChar(int32 fd) {
  int32 iResult = 0;
  size_t ucNumCharToRead = 1;
  uint8 ret = 0;

  if (read(fd, &iResult, ucNumCharToRead) == (ssize_t)ucNumCharToRead) {
    ret = (uint8)(iResult & 0xFF);
  } else {
    //  VND_LOGV("Read error: %s (%d)", strerror(errno), errno);
    ret = 0;
  }
  return ret;
}

/******************************************************************************
 *
 * Name: fw_upload_ComReadChars
 *
 * Description:
 *   Read iCount characters from the port specified by nPortID.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   fd   : Port ID.
 *   pBuffer : Destination buffer for the characters read
 *   iCount    : Number of Characters to be read.
 *
 * Return Value:
 *   Returns the number of characters read if Successful.
 *   Returns -1 if iCount characters could not be read or if Port ID is invalid.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
void fw_upload_ComReadChars(int32 fd, uint8* pBuffer, uint32 uiCount) {
  if (read(fd, pBuffer, uiCount) != (ssize_t)uiCount) {
    VND_LOGV("Read error: %s (%d)", strerror(errno), errno);
  }
  return;
}

/******************************************************************************
 *
 * Name: fw_upload_ComWriteChar
 *
 * Description:
 *   Write a character to the port specified by fd.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   fd : Port ID.
 *   iChar   : Character to be written
 *
 * Return Value:
 *   Returns true, if write is Successful.
 *   Returns false if write is a failure.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
void fw_upload_ComWriteChar(int32 fd, uint8 iChar) {
  ssize_t ucNumCharToWrite = 1;

  if (write(fd, &iChar, (size_t)ucNumCharToWrite) != ucNumCharToWrite) {
    VND_LOGE("Write error: %s (%d)", strerror(errno), errno);
  }
  return;
}

/******************************************************************************
 *
 * Name: fw_upload_ComWriteChars
 *
 * Description:
 *   Write iLen characters to the port specified by fd.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   fd : Port ID.
 *   pBuffer : Buffer where characters are available to be written to the Port.
 *   iLen    : Number of Characters to write.
 *
 * Return Value:
 *   Returns true, if write is Successful.
 *   Returns false if write is a failure.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
void fw_upload_ComWriteChars(int32 fd, uint8* pBuffer, uint32 uiLen) {
  if (write(fd, pBuffer, uiLen) != (ssize_t)uiLen) {
    VND_LOGE("Write error: %s (%d)", strerror(errno), errno);
  }
  return;
}

/******************************************************************************
 *
 * Name: fw_upload_ComGetCTS
 *
 * Description:
 *   Check CTS status
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *
 * Return Value:
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
int32 fw_upload_ComGetCTS(int32 fd) {
  int32 status;
  if (ioctl(fd, TIOCMGET, &status) < 0) {
    VND_LOGE("ioctl error: %s (%d)", strerror(errno), errno);
  }
  if (status & TIOCM_CTS) {
    return 0;
  } else {
    return 1;
  }
}
/******************************************************************************
 *
 * Name: fw_upload_ComGetBufferSize
 *
 * Description:
 *   Check buffer size
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   fd
 *
 * Return Value:
 *   size in buffer
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
uint32 fw_upload_GetBufferSize(int32 fd) {
  uint32 bytes = 0;
  if (ioctl(fd, FIONREAD, &bytes) < 0) {
    VND_LOGE("ioctl error: %s (%d)", strerror(errno), errno);
  }
  return bytes;
}

/******************************************************************************
 *
 * Name: fw_upload_GetTime
 *
 * Description:
 *   Get the current time
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *
 * Return Value:
 *   return the current time in milliseconds
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/

uint64 fw_upload_GetTime(void) {
  struct timespec time;
  clockid_t clk_id;
  uint64 millisectime = -1;

  clk_id = CLOCK_MONOTONIC;
  if (!clock_gettime(clk_id, &time)) {
    millisectime =
        (((uint64)time.tv_sec) * 1000) + (((uint64)time.tv_nsec) / 1000000);
  } else {
    VND_LOGE("clock_gettime error:%s (%d)", strerror(errno), errno);
  }
  return millisectime;
}

/******************************************************************************
 *
 * Name: fw_upload_ComGetCTS_after_fw_dwnl
 *
 * Description:
 *   Polls for cts_timeout duration to check the cts status after FW download
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *
 * Return Value:
 *   return true, if cts is low
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/

bool fw_upload_ComGetCTS_after_fw_dwnl(int32 fd, int32 cts_timeout) {
  bool cts_high_after_fw_dnl = false;
  int32 cts_status = 0;
  uint64 endTime = fw_upload_GetTime() + cts_timeout;
  do {
    cts_status = fw_upload_ComGetCTS(fd);
    if (!cts_high_after_fw_dnl) {
      cts_high_after_fw_dnl = (cts_status == 1) ? true : false;
    }
    if (!cts_status && cts_high_after_fw_dnl) {
      return true;
    }
  } while (endTime > fw_upload_GetTime());
  return false;
}
