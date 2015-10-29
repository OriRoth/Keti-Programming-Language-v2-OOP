#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "memory.h"

#define BUILTIN_BUFFER_SIZE 22
#define NATIVE_NAME_LIMIT 16
memptr native_implementations_array[BUILTIN_BUFFER_SIZE];

enum kind {
	NIL = 0, STRING, INTEGER, CONS
};

typedef enum {
	TYPE_NIL_TRUE,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_CONS,
	TYPE_NATIVE_FUNCTION,
	TYPE_USER_FUNCTION,
	TYPE_OBJECT,
	TYPE_SYMBOL,
	TYPE_ERROR
} Type;

typedef enum {
	ENVIRONMENT, DATA
} LookUp;

typedef memptr (*builtin)(memptr func_expr);

#define NOT_FOUND ((1 << 14) - 1)
memptr environment;
memptr nil;
memptr t;
memptr security_head;
memptr security_tail;
memptr core_symbol;

// builtin functions container
builtin builtins[BUILTIN_BUFFER_SIZE];

// initialize the system
void system_initialize();

memptr translate_func_expr(memptr func_expr);
memptr resolve_expr(memptr expr);

#endif /* SYSTEM_H_ */
