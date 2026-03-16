#pragma once

#include "call_param.h"

#define FL_ARR_NAME fl_call_param_vec
#define FL_ARR_TYPE fl_call_param
#define FL_ARR_DESTRUCTOR(elem) fl_call_param_free(&elem)
#include "templates/array.h"