#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdbool.h>
#include "bitarray.h"

#define MEM_SIZE (1 << 14)

typedef unsigned int memptr;

enum {
	// How many bits for index into pool:
	LG2_POOLSIZE = 14,
	// How many bits for storing car/cdr kind:
	KIND_SIZE = 2
};

typedef struct Cons_t {
	unsigned int carKind :KIND_SIZE;
	unsigned int car :LG2_POOLSIZE;
	unsigned int cdrKind :KIND_SIZE;
	unsigned int cdr :LG2_POOLSIZE;
} Cons;
typedef struct StringHandler_t {
	memptr string;
} StringHandler;

Cons cons_pool[MEM_SIZE];
memptr free_cons_stack[MEM_SIZE];
memptr free_cons_stack_head;
bit cons_marks[BITS(MEM_SIZE)];

StringHandler handlers_pool[MEM_SIZE];
memptr free_handlers_stack[MEM_SIZE];
memptr free_handlers_stack_head;
bit handlers_marks[BITS(MEM_SIZE)];
memptr handlers_sort_buffer[MEM_SIZE];

char strings_pool[MEM_SIZE];
memptr strings_filler;

#define STRING_BUFFER_SIZE 32
char string_buffer[STRING_BUFFER_SIZE];

// TODO: change 22 to BUILTIN_BUFFER_SIZE
#define ROOT_SET_SIZE (5 + 22)
memptr root_set[ROOT_SET_SIZE];
memptr marks_stack[MEM_SIZE];
int marks_stack_head;

#define MOVE_STRING_TO_BUFFER(i)	do {																					\
										int filler = 0;																		\
										while (native_names_buffer[i * NATIVE_NAME_LIMIT + filler] != '\0') {				\
											string_buffer[filler] = native_names_buffer[i * NATIVE_NAME_LIMIT + filler];	\
											filler++;																		\
										}																					\
										string_buffer[filler] = '\0';														\
									} while(0)

#endif /* MEMORY_H_ */
