#include "common/error.h"

#include <stdio.h>


Error create_empty_error() {

    Error e;

    e.message[0] = '\0';
    e.isEmpty = true;

    return e;
}


Error create_error_no_param_raw(const str message,
    i32 line, const str file) {

    Error e;

    e.isEmpty = false;

    snprintf(e.message, ERROR_MAX_LENGTH, 
        "Error in file %s, line %d: %s", 
        file, line, 
        message);

    return e;
}


Error create_error_str_param_1_raw(const str message, const str param1,
    i32 line, const str file) {

    Error e;

    e.isEmpty = false;

    snprintf(e.message, ERROR_MAX_LENGTH, 
        "Error in file %s, line %d: %s%s", 
        file, line, 
        message, param1);

    return e;
}


Error create_error_str_param_2_raw(const str message, 
    const str param1, const str param2,
    i32 line, const str file) {

    Error e;

    e.isEmpty = false;

    snprintf(e.message, ERROR_MAX_LENGTH, 
        "Error in file %s, line %d: %s%s%s", 
        file, line, 
        message, param1, param2);

    return e;
}
