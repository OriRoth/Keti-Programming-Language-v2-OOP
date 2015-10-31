#ifndef ERROR_H_
#define ERROR_H_

#include <stdbool.h>

enum {
	ERROR_PLUS = 0,
	ERROR_MINUS,
	ERROR_MULT,
	ERROR_DIV,
	ERROR_EQUALS,
	ERROR_SMALLER,
	ERROR_GREATER,
	ERROR_CONS,
	ERROR_CAR,
	ERROR_CDR,
	ERROR_NIL,
	ERROR_TRUE,
	ERROR_COND,
	ERROR_SETQ,
	ERROR_DEFUN,
	ERROR_OR,
	ERROR_AND,
	ERROR_XOR,
	ERROR_LIST,
	ERROR_VALUE,
	ERROR_ASSIGN,
	ERROR_SUPPS,
	ERROR_LINE,
	ERROR_FAILURE,
	ERROR_INVALID,
	ERROR_COUNT,
	ERROR_ROW,
	ERROR_EMPTY,
	ERROR_MALFORMED
};

bool error;

#define ERROR_MESSAGE_LIMIT (20)
#define ERROR_KIND (29)

#define ERROR_MESSAGE_BUFFER_SIZE 128
char error_buffer[ERROR_MESSAGE_BUFFER_SIZE];
int error_buffer_filler;
char errors[ERROR_MESSAGE_LIMIT * ERROR_KIND];

#define ERROR_TO_BUFFER(e)	do {														\
								int i=e;												\
								while (errors[i] != '\0') {								\
									error_buffer[error_buffer_filler++] = errors[i++];	\
								}														\
							} while(0)
#define ERROR(m1, m2, m3, m4)	do {												\
									if (!error) {									\
										error = true;								\
										error_buffer_filler=0;						\
										ERROR_TO_BUFFER(ERROR_MESSAGE_LIMIT * m1);	\
										ERROR_TO_BUFFER(ERROR_MESSAGE_LIMIT * m2);	\
										ERROR_TO_BUFFER(ERROR_MESSAGE_LIMIT * m3);	\
										ERROR_TO_BUFFER(ERROR_MESSAGE_LIMIT * m4);	\
										error_buffer[error_buffer_filler] = '\0';	\
									}												\
								} while (0)

#endif /* ERROR_H_ */
