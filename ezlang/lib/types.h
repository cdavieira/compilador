#pragma once

typedef enum Type {
	TYPE_INT = 0,
	TYPE_FLT = 1,
	TYPE_BOO = 2,
	TYPE_STR = 3,
	TYPE_NOTHING = 4,
} Type;

const char* type_name(enum Type type);





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

/* ABOUT 'enum OperandType'
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
enum OperandType {
	I = 0x1,
	R = 0x2,
	B = 0x3,
	S = 0x4,
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
	EE = 0,
	OK = 0x01,
#define OPCODE(N,M) ((N << 4) | M)
	II = OPCODE(I, I),
	RR = OPCODE(R, R),
	BB = OPCODE(B, B),
	SS = OPCODE(S, S),
	RI = OPCODE(R, I),
	BI = OPCODE(B, I),
	SI = OPCODE(S, I),
	BR = OPCODE(B, R),
	BS = OPCODE(B, S),
	SR = OPCODE(S, R),
	IR = OPCODE(I, R),
	IB = OPCODE(I, B),
	IS = OPCODE(I, S),
	RB = OPCODE(R, B),
	SB = OPCODE(S, B),
	RS = OPCODE(R, S),
#undef OPCODE
};

typedef enum Conversion {
    CONV_B2I,
    CONV_B2R,
    CONV_B2S,
    CONV_I2R,
    CONV_I2S,
    CONV_R2S,
    CONV_NONE,
} Conversion;

typedef struct {
    Type type;
    Conversion left;
    Conversion right;
} BinaryOperationData;

typedef struct {
    Type type;
    Conversion right;
} UnaryOperationData;

enum BinaryOperationTable
binoptable_get_sumtype(
    Type op1,
    Type op2);

enum BinaryOperationTable
binoptable_get_minustype(
    Type op1,
    Type op2);

enum BinaryOperationTable
binoptable_get_logicaltype(
    Type op1,
    Type op2);

enum BinaryOperationTable
binoptable_get_assigntype(
    Type op1,
    Type res);
