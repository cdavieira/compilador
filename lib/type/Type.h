#pragma once

enum Type {
	TYPE_VOID = 0,
	TYPE_INT, //1
	TYPE_FLT, //2
	TYPE_CHAR, //3
	TYPE_STR
};


/* WARNING
 * we had to move the definitions of 'enum OperandType' and 'enum
 * BinaryOperationTable' to this header, because the AST needs that
 * information to decide if it should append conversion nodes or not to its
 * OperationNodes.
 *
 * This was necessary, because the typesystem and the AST system are
 * independent from each other but they must interact/work together.
 *
 * Therefore, although these enumerations were implementation details of the
 * type checking system, they had to become public so that they could be used
 * by the AST.
 *
 * There are still some private information/implementation details for these
 * types which can be found under 'Literal.c' in the form of static const
 * tables that allows us to retrieve the operation type of two operands of
 * type 'enum Type'.
 *
 * To access this information, a couple of functions were created to expose
 * that information whilst keeping private these tables;
 * */

/* ABOUT 'enum Operand'
 *
 * This enumeration is solely used to generate the unique values of 'enum
 * BinaryOperationTable'
 *
 * each member should use only 4 bits.
 *
 * therefore, this enum can identify up to 16 different types.
 *
 * in reality, only 15 different types are useable (0x0 is reserved).
 *
 * The reason for the bytesize limitation of its members is that the values of
 * 'BinaryOperationTable' depend on that information.  For more information,
 * see yourself how the macro 'OPCODE' operates (which holds the expression
 * that results on the values of 'BinaryOperationTable')
 * */
enum Operand {
	E = 0x0, //error (reserved)
	I = 0x1,
	C = 0x2,
	F = 0x3,
};

/* ABOUT 'enum BinaryOperationTable'
 *
 * each member should use only 8 bits.
 *
 * therefore, this enum can encode up to 256 combinations of sums.
 *
 * in reality, only 225 are available, because all combinations which use 0x0
 * are reserved.
 * */
enum BinaryOperationTable {
	BINOP_EE = 0,
	BINOP_OK = 0x01,
#define OPCODE(N,M) ((N << 4) | M)
	BINOP_II = OPCODE(I, I),
	BINOP_FF = OPCODE(F, F),
	BINOP_CC = OPCODE(C, C),
	BINOP_FI = OPCODE(F, I),
	BINOP_CI = OPCODE(C, I),
	BINOP_CF = OPCODE(C, F),
	BINOP_IF = OPCODE(I, F),
	BINOP_IC = OPCODE(I, C),
	BINOP_FC = OPCODE(F, C),
#undef OPCODE
};

const char* type_name(enum Type type);

typedef enum Conversion {
    CONV_I2F,
    CONV_I2C,
    CONV_C2I,
    CONV_C2F,
    CONV_F2I,
    CONV_F2C,
    CONV_NONE,
} Conversion;

typedef struct {
    enum Type type;
    Conversion left;
    Conversion right;
} BinaryOperationData;

typedef struct {
    enum Type type;
    Conversion right;
} UnaryOperationData;

enum BinaryOperationTable
binoptable_get_arithtype(
    enum Type op1,
    enum Type op2);

enum BinaryOperationTable
binoptable_get_logicaltype(
    enum Type op1,
    enum Type op2);

enum BinaryOperationTable
binoptable_get_assigntype(
    enum Type op1,
    enum Type res);
