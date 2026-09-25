/**
 * ---------- op_order.h ------------------------
 *
 * Declares operator order logic
 *
 * ----------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_op_order
{
    FL_OP_ORDER_LOOP = FL_SHORT_MIN,
    FL_OP_ORDER_ASSIGN = -4000,
    FL_OP_ORDER_LAMBDA = -3000,
    FL_OP_ORDER_ELSE = -2000,
    FL_OP_ORDER_IF = -1000,
    FL_OP_ORDER_NEUTRAL = 0,
    FL_OP_ORDER_ARITHM_LOW = 1000,
    FL_OP_ORDER_ARITHM_MED = 2000,
    FL_OP_ORDER_ARITHM_HIGH = 3000,
    FL_OP_ORDER_GROUP = FL_SHORT_MAX,
};
typedef fl_short fl_op_order;