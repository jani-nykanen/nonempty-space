#ifndef NONEMPTY_SPACE_COMMON_ERROR_H
#define NONEMPTY_SPACE_COMMON_ERROR_H


#include "types.h"


#define ERROR_MAX_LENGTH 2048


#define memory_error() create_error_no_param("Memory allocation error!")


typedef struct {

    char message [ERROR_MAX_LENGTH];
    bool isEmpty;

} Error;


Error create_empty_error();
Error create_error_no_param(const str message);
Error create_error_str_param_1(const str message, const str param1);
Error create_error_str_param_2(const str message, const str param1, const str param2);


#endif // NONEMPTY_SPACE__COMMON_ERROR_H
