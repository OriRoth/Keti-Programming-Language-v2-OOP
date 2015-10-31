#include "memory.h"
#include "system.h"
#include "object.h"
#include <stdio.h>

void mark_sweep();

memptr get_string(memptr handler_address, int stage) {

	return handlers_pool[handler_address].string
			+ ((stage & (1 << 13)) ?
					(((signed) stage) ^ ((1 << 14) - 1)) : stage);
}

// compares two strings
bool string_compare(memptr strptr1, memptr strptr2) {

	while ((strings_pool[strptr1] != '\0') && (strings_pool[strptr2] != '\0')
			&& (strings_pool[strptr1] == strings_pool[strptr2])) {
		strptr1++;
		strptr2++;
	}
	return (strings_pool[strptr1] == strings_pool[strptr2]);
}

// returns strings length
int string_length(memptr str) {

	if ((cons_pool[str].cdr) & (1 << 13)) {
		return 1;
	}

	memptr strptr = get_string(cons_pool[str].car, cons_pool[str].cdr);
	int counter = 0;
	while (strings_pool[strptr++] != '\0') {
		counter++;
	}
	return counter;
}

// copy a string to location
void string_copy(memptr dst, memptr str) {

	str--;
	do {
		str++;
		strings_pool[dst] = strings_pool[str];
	} while (strings_pool[str] != '\0');
}

// copy a string to location from buffer
void string_copy_from_buffer(memptr dst) {

	int str = 0;
	do {
		strings_pool[dst++] = string_buffer[str];
	} while (string_buffer[str++] != '\0');
}

extern char core_string[];
// copy a string to location from core_string, returns handler
memptr string_copy_from_core_string() {

	int len = CORE_STRING_SIZE;
	memptr handler;
	if (free_handlers_stack_head >= 1 && strings_filler + len < MEM_SIZE) {
		handler = free_handlers_stack[free_handlers_stack_head--];
		handlers_pool[handler].string = strings_filler;
		int str = 0, dst = strings_filler;
		do {
			strings_pool[dst++] = core_string[str];
		} while (core_string[str++] != '\0');
		strings_filler += len;
		return handler;
	}

	mark_sweep();
	if (free_handlers_stack_head >= 1 && strings_filler + len < MEM_SIZE) {
		handler = free_handlers_stack[free_handlers_stack_head--];
		handlers_pool[handler].string = strings_filler;
		int str = 0, dst = strings_filler;
		do {
			strings_pool[dst++] = string_buffer[str];
		} while (string_buffer[str++] != '\0');
		strings_filler += len;
		return handler;
	}

	fprintf(stderr, "\nMemory Failue => system out of memory\n");
	exit(0);
}

void initialize_memory_system() {

	for (int i = 0; i < MEM_SIZE; i++) {
		free_cons_stack[i] = free_handlers_stack[i] = MEM_SIZE - i - 1;
	}
	free_cons_stack_head = free_handlers_stack_head = MEM_SIZE - 1;
	strings_filler = 0;
}

memptr allocate_cons() {

	if (free_cons_stack_head >= 1) {
		return free_cons_stack[free_cons_stack_head--];
	}

	mark_sweep();
	if (free_cons_stack_head >= 1) {
		return free_cons_stack[free_cons_stack_head--];
	}

	fprintf(stderr, "\nMemory Failue => system out of memory\n");
	exit(0);
}

void announce_allocation(int num_of_allocations) {

	if (free_cons_stack_head >= num_of_allocations) {
		return;
	}

	mark_sweep();
	if (free_cons_stack_head >= num_of_allocations) {
		return;
	}

	fprintf(stderr, "\nMemory Failue => system out of memory\n");
	exit(0);
}

//  allocate a new string, return handler address
memptr allocate_string_from_buffer() {

	string_buffer[STRING_BUFFER_SIZE - 1] = '\0';
	int len = 0;
	while (string_buffer[len++] != '\0')
		;
	memptr handler;
	if (free_handlers_stack_head >= 1 && strings_filler + len < MEM_SIZE) {
		handler = free_handlers_stack[free_handlers_stack_head--];
		handlers_pool[handler].string = strings_filler;
		string_copy_from_buffer(strings_filler);
		strings_filler += len;
		return handler;
	}

	mark_sweep();
	if (free_handlers_stack_head >= 1 && strings_filler + len < MEM_SIZE) {
		handler = free_handlers_stack[free_handlers_stack_head--];
		handlers_pool[handler].string = strings_filler;
		string_copy_from_buffer(strings_filler);
		strings_filler += len;
		return handler;
	}

	fprintf(stderr, "\nMemory Failue => system out of memory\n");
	exit(0);
}

/*
 * Garbage Collection
 */

static void rec_mark(memptr root) {

	marks_stack[++marks_stack_head] = root;
	BITSET(cons_marks, root);
	while (marks_stack_head >= 0) {
		memptr current = marks_stack[marks_stack_head--];
		if (cons_pool[current].carKind == STRING) {
			BITSET(handlers_marks, cons_pool[current].car);
		}
		if (cons_pool[current].cdrKind == STRING) {
			BITSET(handlers_marks, cons_pool[current].cdr);
		}
		if (cons_pool[current].carKind == CONS
				&& (BITAT(cons_marks, cons_pool[current].car) != 1)) {
			marks_stack[++marks_stack_head] = cons_pool[current].car;
			BITSET(cons_marks, cons_pool[current].car);
		}
		if (cons_pool[current].cdrKind == CONS
				&& (BITAT(cons_marks, cons_pool[current].cdr) != 1)) {
			marks_stack[++marks_stack_head] = cons_pool[current].cdr;
			BITSET(cons_marks, cons_pool[current].cdr);
		}
	}
}

void fill_root_set() {
	root_set[0] = nil;
	root_set[1] = t;
	root_set[2] = security_head;
	root_set[3] = environment;
	root_set[4] = core_symbol;
	for (int i = 0; i < BUILTIN_BUFFER_SIZE; i++) {
		root_set[i + 4] = native_implementations_array[i];
	}
}

void mark_sweep() {

	fprintf(stderr, "\ngarbage collector start\n");

	SETBITS(cons_marks, MEM_SIZE);
	SETBITS(handlers_marks, MEM_SIZE);
	free_cons_stack_head = free_handlers_stack_head = -1;
	fill_root_set();

	// mark
	for (int i = 0; i < ROOT_SET_SIZE; i++) {
		marks_stack_head = -1;
		rec_mark(root_set[i]);
	}

	// sweep
	memptr handlers_sort_buffer_filler = 0;
	for (int i = MEM_SIZE - 2; i >= 0; i--) {
		if (BITAT(cons_marks, i) != 1) {
			free_cons_stack[++free_cons_stack_head] = i;
		}
		if (BITAT(handlers_marks, i) != 1) {
			free_handlers_stack[++free_handlers_stack_head] = i;
		} else {
			handlers_sort_buffer[handlers_sort_buffer_filler++] = i;
		}
	}

	// handlers sort
	memptr temp;
	for (int i = 0; i < handlers_sort_buffer_filler; i++) {
		for (int j = 0; j < handlers_sort_buffer_filler - i - 1; j++) {
			if (handlers_pool[handlers_sort_buffer[j]].string
					> handlers_pool[handlers_sort_buffer[j + 1]].string) {
				temp = handlers_sort_buffer[j];
				handlers_sort_buffer[j] = handlers_sort_buffer[j + 1];
				handlers_sort_buffer[j + 1] = temp;
			}
		}
	}

	// strings bottom append
	strings_filler = 0;
	for (int i = 0; i < handlers_sort_buffer_filler; i++) {

		memptr dst = strings_filler;
		memptr src = handlers_pool[handlers_sort_buffer[i]].string;
		int len = 0;
		do {
			strings_pool[dst] = strings_pool[src];
			len++;
			dst++;
		} while (strings_pool[src++] != '\0');
		handlers_pool[handlers_sort_buffer[i]].string = strings_filler;
		strings_filler += len;
	}

	fprintf(stderr, "garbage collector end\n");
}
