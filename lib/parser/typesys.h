#pragma once

#include "types/Type.h"

typedef enum Conversion {
    CONV_I2F,
    CONV_I2C,
    CONV_C2I,
    CONV_C2F,
    CONV_F2I,
    CONV_F2C,
    CONV_NONE,
} Conversion;

typedef struct TypeData{
  enum Type type;
  Conversion left;
  Conversion right;
} TypeData;

const TypeData typesys_sum(enum Type t1, enum Type t2);
const TypeData typesys_sub(enum Type t1, enum Type t2);
const TypeData typesys_mul(enum Type t1, enum Type t2);
const TypeData typesys_div(enum Type t1, enum Type t2);
const TypeData typesys_mod(enum Type op1, enum Type op2);
const TypeData typesys_and(enum Type t1, enum Type t2);
const TypeData typesys_or(enum Type t1, enum Type t2);
const TypeData typesys_lt(enum Type t1, enum Type t2);
const TypeData typesys_gt(enum Type t1, enum Type t2);
const TypeData typesys_ne(enum Type t1, enum Type t2);
const TypeData typesys_eq(enum Type t1, enum Type t2);
const TypeData typesys_assign(enum Type lhs, enum Type rhs);
