/*
 * eds.c
 *
 *  Created on: Dec 13, 2021
 *      Author: nenad
 */
#include "eds.h"

const char*
eds__error_to_str(uint32_t error)
{
    static const char * error_texts[] =
    {
        [EDS__ERROR_NONE] = "none",
        [EDS__ERROR_INVALID_ARGUMENT] = "invalid argument",
        [EDS__ERROR_NO_MEMORY] = "no memory",
        [EDS__ERROR_NO_RESOURCE] = "no resource",
        [EDS__ERROR_NO_PERMISSION] = "no permission",
        [EDS__ERROR_ALREADY_EXISTS] = "already exists",
        [EDS__ERROR_INVALID_CONFIGURATION] = "invalid configuration",
        [EDS__ERROR_NOT_EXISTS] = "not exists",
        [EDS__ERROR_MALFORMED_SM] = "malformed SM",
        [EDS__ERROR_NO_SPACE] = "no space",
        [EDS__ERROR_OUT_OF_RANGE] = "out of range",
    };
    const char * error_text = NULL;

    if (error < (sizeof(error_texts) / sizeof(error_texts[0]))) {
        error_text = error_texts[error];
    }

    if (error_text != NULL) {
        return error_text;
    } else {
        return "unknown";
    }
}
