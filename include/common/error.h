#ifndef NONEMPTY_SPACE_COMMON_ERROR_H
#define NONEMPTY_SPACE_COMMON_ERROR_H


#include "types.h"


#define ERROR_MAX_LENGTH 2048


#define memory_error() create_error_no_param("Memory allocation error!")

#define create_error_no_param(msg)  create_error_no_param_raw(msg, __LINE__, __FILE__)
#define create_error_str_param_1(msg, param1)  create_error_str_param_1_raw(msg, param1, __LINE__, __FILE__)
#define create_error_str_param_2(msg, param1, param2)  create_error_str_param_2_raw(msg, param1, param2, __LINE__, __FILE__)


typedef struct {

    char message [ERROR_MAX_LENGTH];
    bool isEmpty;

} Error;


Error create_empty_error();

Error create_error_no_param_raw(const str message, 
    i32 line, const str file);
Error create_error_str_param_1_raw(const str message, 
    const str param1, 
    i32 line, const str file);
Error create_error_str_param_2_raw(const str message,
    const str param1, const str param2, 
    i32 line, const str file);


#endif // NONEMPTY_SPACE__COMMON_ERROR_H
