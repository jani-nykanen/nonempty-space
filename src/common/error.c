#include "common/error.h"

#include <stdio.h>


Error create_empty_error() {

    Error e;

    e.message[0] = '\0';
    e.isEmpty = true;

    return e;
}


Error create_error_no_param(const str message) {

    Error e;

    e.isEmpty = false;

    snprintf(e.message, ERROR_MAX_LENGTH, 
        "Error in file %s, line %d: %s", 
        __FILE__, __LINE__, 
        message);

    return e;
}


Error create_error_str_param_1(const str message, const str param1) {

    Error e;

    e.isEmpty = false;

    snprintf(e.message, ERROR_MAX_LENGTH, 
        "Error in file %s, line %d: %s", 
        __FILE__, __LINE__, 
        message, param1);

    return e;
}


Error create_error_str_param_2(const str message, const str param1, const str param2) {

    Error e;

    e.isEmpty = false;

    snprintf(e.message, ERROR_MAX_LENGTH, 
        "Error in file %s, line %d: %s", 
        __FILE__, __LINE__, 
        message, param1, param2);

    return e;
}
