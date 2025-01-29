#pragma once

#include "base/nibr.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include <iomanip>
#include <functional>
    
#if defined(_WIN32)
    #define FUNC_NAME __FUNCTION__
    #define FILE_NAME __FILE__
    #define LINE_NO __LINE__
#else
    #define FUNC_NAME __PRETTY_FUNCTION__
    #define FILE_NAME __FILE__
    #define LINE_NO __LINE__
#endif

#define MSG_FATAL   NIBR::MESSAGE(NIBR::MESSAGE_TYPE::msg_fatal, FILE_NAME, FUNC_NAME, LINE_NO)
#define MSG_ERROR   NIBR::MESSAGE(NIBR::MESSAGE_TYPE::msg_error, FILE_NAME, FUNC_NAME, LINE_NO)
#define MSG_WARN    NIBR::MESSAGE(NIBR::MESSAGE_TYPE::msg_warn,  FILE_NAME, FUNC_NAME, LINE_NO)
#define MSG_INFO    NIBR::MESSAGE(NIBR::MESSAGE_TYPE::msg_info,  FILE_NAME, FUNC_NAME, LINE_NO)
#define MSG_DETAIL  NIBR::MESSAGE(NIBR::MESSAGE_TYPE::msg_detail,FILE_NAME, FUNC_NAME, LINE_NO)
#define MSG_DEBUG   NIBR::MESSAGE(NIBR::MESSAGE_TYPE::msg_debug, FILE_NAME, FUNC_NAME, LINE_NO)


namespace NIBR 
{
    typedef enum {
        VERBOSE_QUITE  = 0,          // No verbose.
        VERBOSE_FATAL  = 1,          // Severe errors which will terminate the execution.
        VERBOSE_ERROR  = 2,          // Error which might terminate the execution or not.
        VERBOSE_WARN   = 3,          // Potentially harmful situation.
        VERBOSE_INFO   = 4,          // Information about the execution.
        VERBOSE_DETAIL = 5,          // Detailed information about the execution.
        VERBOSE_DEBUG  = 6           // Detailed tracing of the code execution.
    } VERBOSE_LEVEL;

    typedef enum {
        msg_fatal   = 0,
        msg_error   = 1,
        msg_warn    = 2,
        msg_info    = 3,
        msg_detail  = 4,
        msg_debug   = 5
    } MESSAGE_TYPE;

    struct MESSAGE {
        MESSAGE_TYPE    type;
        std::string     fileName;
        std::string     funcName;
        int             lineNo;

        MESSAGE(
            MESSAGE_TYPE _type,
            std::string  _fileName,
            std::string  _funcName,
            int          _lineNo)
        {
            type        = _type;
            fileName    = _fileName;
            funcName    = _funcName;
            lineNo      = _lineNo;
        };

        // ~MESSAGE() {
        //     std::cerr << "Message destroyed." << std::endl;
        // }
    };

    void disableTerminalOutput();
    void enableTerminalOutput();

    int disp(MESSAGE msg, const char *format, ...);
    void wait(const char *format, ...);

}

