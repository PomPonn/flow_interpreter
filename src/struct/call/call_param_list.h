#pragma once

#include "call_param.h"

#define FL_ARR_NAME fl_call_param_list
#define FL_ARR_TYPE fl_call_param
#define FL_ARR_DESTROYER(elem) fl_call_param_free(&elem)
#include "generics/array.h"