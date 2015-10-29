#include "object.h"
#include "system.h"

char core_string[5] = { 'c', 'o', 'r', 'e', '\0' };

bool is_core(memptr strptr) {

	int iter1 = strptr, iter2 = 0;
	while ((strings_pool[iter1] != '\0') && (core_string[iter2] != '\0')
			&& (strings_pool[iter1] == core_string[iter2])) {
		iter1++;
		iter2++;
	}
	return strings_pool[iter1] == core_string[iter2];
}

memptr object_lookup(memptr object, memptr symbol) {

	memptr current_node, next_node = cons_pool[object].cdr, current_data;
	do {
		current_node = next_node;
		current_data = cons_pool[current_node].car;
		if (string_compare(get_string(cons_pool[current_data].car, 0),
				get_string(cons_pool[symbol].car, 0))) {
			return cons_pool[current_data].cdr;
		}
		next_node = cons_pool[current_node].cdr;
	} while (cons_pool[current_node].cdrKind != NIL);
	return NOT_FOUND;
}

void object_insert_symbol(memptr object, memptr symbol) {

	memptr new_head, temp;
	if (cons_pool[object].cdr == nil) {
		cons_pool[object].cdr = allocate_cons();
		new_head = cons_pool[object].cdr;
		cons_pool[new_head].car = symbol;
		cons_pool[new_head].carKind = CONS;
		cons_pool[new_head].cdrKind = NIL;
	} else {
		temp = cons_pool[object].cdr;
		cons_pool[object].cdr = allocate_cons();
		new_head = cons_pool[object].cdr;
		cons_pool[new_head].car = symbol;
		cons_pool[new_head].carKind = CONS;
		cons_pool[new_head].cdr = temp;
		cons_pool[new_head].cdrKind = CONS;
	}
}

bool object_remove_symbol(memptr object, memptr symbol) {

	memptr previous_node = object, current_node, next_node =
			cons_pool[object].cdr, current_data;
	if (next_node != nil) {
		do {
			current_node = next_node;
			current_data = cons_pool[current_node].car;
			if (string_compare(get_string(cons_pool[current_data].car, 0),
					get_string(cons_pool[symbol].car, 0))) {
				cons_pool[previous_node].cdr = cons_pool[current_node].cdr;
				cons_pool[previous_node].cdrKind =
						cons_pool[current_node].cdrKind;
				return true;
			}
			previous_node = current_node;
			next_node = cons_pool[current_node].cdr;
		} while (cons_pool[current_node].cdrKind != NIL);
	}
	return false;
}
