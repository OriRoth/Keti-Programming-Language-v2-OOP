#include "system.h"
#include <stdio.h>

#define MARK_OBJECTS true
#define INPUT_BUFFER_SIZE (1<<12)
#define SYNTAX_ERROR (-1)
#define SKIP_SPACES(iter, buff)	do {										\
									while (is_space(buff[iter])) {++iter;}	\
								} while(0)

char line_buffer[INPUT_BUFFER_SIZE];

FILE* input;
int stdin_backup;
bool done_reading = false;
int parentheses_counter;

char get_input(FILE *input) {
	char c;
	if (!done_reading) {
		c = fgetc(input);
	} else {
		c = getchar();
	}
	if (!done_reading) {
		if (c == '\n' && parentheses_counter == 0) {
			return get_input(input);
		}
		if (c != EOF) {
			fprintf(stdout, "%c", c);
		} else {
			done_reading = true;
			close(stdin);
			dup2(stdin_backup, stdin);
			close(stdin_backup);
			input = stdin;
			return ' ';
		}
	}
	return c;
}

void move_string_to_string_buffer(int start) {
	int i = 0;
	do {
		string_buffer[i++] = line_buffer[start];
	} while (line_buffer[start++] != '\0');
}

bool is_space(char c) {
	return c == '\n' || c == '\t' || c == ' ' || c == '\r';
}

bool is_digit(char c) {
	return (c >= '0' && c <= '9');
}

bool is_nil;
int translate_function(memptr parent, int start) {

	int iter = start + 1;
	cons_pool[parent].car = allocate_cons();
	cons_pool[parent].carKind = CONS;
	cons_pool[parent].cdrKind = NIL;
	memptr previous_node = NOT_FOUND;
	memptr current_node = cons_pool[parent].car;
	cons_pool[current_node].carKind = NIL;
	cons_pool[current_node].cdrKind = NIL;
	memptr current_value;

	while (true) {
		cons_pool[current_node].car = allocate_cons();
		cons_pool[current_node].carKind = CONS;
		current_value = cons_pool[current_node].car;
		cons_pool[current_value].carKind = NIL;
		cons_pool[current_value].cdrKind = NIL;
		SKIP_SPACES(iter, line_buffer);
		if (line_buffer[iter] == ')') {
			// end of function
			if (previous_node != NOT_FOUND) {
				cons_pool[previous_node].cdrKind = NIL;
				is_nil = false;
			} else {
				is_nil = true;
			}
			return ++iter;
		} else if (line_buffer[iter] == '(') {
			// start of a function
			iter = translate_function(current_value, iter);
			if (iter == SYNTAX_ERROR) {
				is_nil = false;
				return SYNTAX_ERROR;
			}
			if (is_nil) {
				cons_pool[current_node].car = nil;
			}
		} else if (is_digit(line_buffer[iter])) {
			// a number
			int number = 0;
			while (iter < INPUT_BUFFER_SIZE && is_digit(line_buffer[iter])) {
				number *= 10;
				number += line_buffer[iter++] - '0';
			}
			cons_pool[current_value].carKind = INTEGER;
			cons_pool[current_value].cdrKind = NIL;
			cons_pool[current_value].car = number;
		} else if (line_buffer[iter] == '\"') {
			// a string
			int string_start = iter + 1;
			int len = 0;
			while (++iter < INPUT_BUFFER_SIZE && line_buffer[iter] != '\"')
				len++;
			if (iter >= INPUT_BUFFER_SIZE) {
				is_nil = false;
				return SYNTAX_ERROR;
			}
			if (len == 0) {
				cons_pool[current_node].car = nil;
				iter++;
			} else {
				line_buffer[iter++] = 0;
				move_string_to_string_buffer(string_start);
				cons_pool[current_value].car = allocate_string_from_buffer();
				cons_pool[current_value].carKind = STRING;
				cons_pool[current_value].cdr = 0;
				cons_pool[current_value].cdrKind = INTEGER;
			}
		} else {
			// a symbol
			int symbol_start = iter;
			while (iter < INPUT_BUFFER_SIZE && line_buffer[iter] != '('
					&& line_buffer[iter] != ')' && line_buffer[iter] != '\"'
					&& !is_space(line_buffer[iter])) {
				++iter;
			};
			if (iter >= INPUT_BUFFER_SIZE) {
				is_nil = false;
				return SYNTAX_ERROR;
			}
			char temp = line_buffer[iter];
			line_buffer[iter] = 0;
			if (line_buffer[symbol_start] == 't'
					&& line_buffer[symbol_start + 1] == '\0') {
				// t
				cons_pool[current_node].car = t;
			} else {
				move_string_to_string_buffer(symbol_start);
				cons_pool[current_value].car = allocate_string_from_buffer();
				cons_pool[current_value].carKind = STRING;
				cons_pool[current_value].cdr = nil;
				cons_pool[current_value].cdrKind = CONS;
			}
			line_buffer[iter] = temp;
		}

		cons_pool[current_node].cdr = allocate_cons();
		cons_pool[current_node].cdrKind = CONS;
		previous_node = current_node;
		current_node = cons_pool[current_node].cdr;
	}

	is_nil = false;
	return SYNTAX_ERROR;
}

#define MAX_DEPTH 3
void print_result(memptr result, int depth) {

	if (result == NOT_FOUND) {
		fprintf(stderr, "undefined error");
	} else if (depth >= MAX_DEPTH) {
		fprintf(stdout, "...");
	} else {
		Type result_type = type(result);
		switch (result_type) {
		case TYPE_NIL_TRUE:
			if (result == nil) {
				fprintf(stdout, "nil : nil");
			} else if (result == true) {
				fprintf(stdout, "true : true");
			}
			break;
		case TYPE_INTEGER:
			fprintf(stdout, "%d : integer",
					(((cons_pool[result].car) & (1 << 13)) ?
							-(((cons_pool[result].car) ^ ((1 << 14) - 1)) + 1) :
							(cons_pool[result].car)));
			break;
		case TYPE_STRING:
			if ((cons_pool[result].cdr) & 1 << 13) {
				fprintf(stdout, "\"%c\" : string",
						strings_pool[get_string(cons_pool[result].car,
								cons_pool[result].cdr)]);
			} else {
				fprintf(stdout, "\"%s\" : string",
						&strings_pool[get_string(cons_pool[result].car,
								cons_pool[result].cdr)]);
			}
			break;
		case TYPE_CONS:
			fprintf(stdout, "<");
			print_result(cons_pool[result].car, depth + 1);
			fprintf(stdout, ",");
			print_result(cons_pool[result].cdr, depth + 1);
			fprintf(stdout, "> : cons");
			break;
		case TYPE_USER_FUNCTION:
			fprintf(stdout, "<function> : function");
			break;
		case TYPE_NATIVE_FUNCTION:
			fprintf(stdout, "<function> : function");
			break;
		case TYPE_OBJECT:
			print_result(object_lookup(result, core_symbol), depth);
			if (MARK_OBJECTS) fprintf(stdout, "@");
			break;
		default:
			fprintf(stdout, "unrecognized value");
			break;
		}
	}
}

void print_logo() {

	printf(
			"\
      .-.                                   .-.                   \n\
        /|/|    .-.                        / (_)     .-.          \n\
       /   |    `-'.-.    ).--..-._.      /          `-' .   .-.  \n\
      /    |   /  (      /    (   )      /          /   / \\  /  ) \n\
 .-' /     |_.(__. `---'/      `-'    .-/.    .-._.(__./ ._)/`-'  \n\
(__.'      `.                        (_/ `-._.        /    /      \n\
by Ori Roth\n\n");
}

#define PRINT_LIMIT 200
void print_mem() {
	printf("environment: %d\ncore_symbol: %d\n", environment, core_symbol);
	for (int i = 0; i < PRINT_LIMIT; i++) {
		printf("%3d=>(%d, ", i, cons_pool[i].carKind);
		if (cons_pool[i].carKind == 1) {
			printf("%s-%d",
					&(strings_pool[handlers_pool[cons_pool[i].car].string]),
					cons_pool[i].car);
		} else {
			printf("%d", cons_pool[i].car);
		}
		printf(", %d, ", cons_pool[i].cdrKind);
		if (cons_pool[i].cdrKind == 1) {
			printf("%s-%d",
					&(strings_pool[handlers_pool[cons_pool[i].cdr].string]),
					cons_pool[i].cdr);
		} else {
			printf("%d", cons_pool[i].cdr);
		}
		printf(")\n");
	}
	printf("---------------------------------------------------------------\n");
	for (int i = 0; i < PRINT_LIMIT; i++) {
		printf("%3d=>(%d)\n", i, handlers_pool[i]);
	}
	printf("---------------------------------------------------------------\n");
	for (int i = 0; i < PRINT_LIMIT; i++) {
		printf("%3d=>(%d)\n", i, strings_pool[i]);
	}
}

int main(int argc, char *argv[]) {

	if (argc > 1) {
		stdin_backup = dup(stdin);
		input = fopen(argv[1], "r+");
	} else {
		input = stdin;
		done_reading = true;
	}

	system_initialize();
	security_init();
	print_logo();
	fflush(stdout);

	while (true) {

		fprintf(stdout, "> ");
		fflush(stdout);

		int line_iter = 0;
		parentheses_counter = 0;
		line_buffer[line_iter] = get_input(input);
		while (is_space(line_buffer[line_iter]))
			line_buffer[line_iter] = get_input(input);
		if (line_buffer[line_iter] == '(') {
			// a lisp commend
			parentheses_counter = 1;
			line_iter++;
			while (parentheses_counter > 0 && line_iter < INPUT_BUFFER_SIZE) {
				line_buffer[line_iter] = get_input(input);
				if (line_buffer[line_iter] == 10) {
					continue;
				} else if (line_buffer[line_iter] == '(') {
					++parentheses_counter;
				} else if (line_buffer[line_iter] == ')') {
					--parentheses_counter;
				}
				++line_iter;
			}
			if (line_iter >= INPUT_BUFFER_SIZE) {
				fprintf(stderr, "line too big\n");
				continue;
			}

			memptr translated_expression = safe_allocate_cons();
			translate_function(translated_expression, 0);
			if (is_nil) {
				translated_expression = nil;
			}
			if (translated_expression == NOT_FOUND) {
				fprintf(stderr, "undefined error\n");
			} else {
				memptr result = resolve_expr(translated_expression);
				if (!done_reading) {
					fprintf(stdout, "\n");
				}
				fprintf(stdout, "\n~ ");
				print_result(result, 0);
				fprintf(stdout, "\n\n");
			}
		} else {
			while (get_input(input) != 10)
				;
			fprintf(stdout, ".\n");
			print_mem();
		}

		fflush(stderr);
		fflush(stdout);
		security_init();
	}

	return 0;
}
