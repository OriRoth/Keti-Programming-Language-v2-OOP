#include "system.h"
#include "native.h"
#include "error.h"
#include <stdio.h>

//#define DEBUG
//#define EXTENDED_SECURITY

char native_names_buffer[NATIVE_NAME_LIMIT * BUILTIN_BUFFER_SIZE] = { '+', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '-', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '*', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '/', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '=', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '<', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '>', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', 'c', 'o', 'n', 's', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', 'c', 'a', 'r', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 'c', 'd',
		'r', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', 'n', 'i', 'l', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', 't', 'r', 'u', 'e', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 'c', 'o',
		'n', 'd', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', 's', 'e', 't', 'q', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', 'd', 'e', 'f', 'u', 'n', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 'o', 'r', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', 'a', 'n', 'd', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', 'x', 'o', 'r', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 'l', 'i', 's',
		't', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', 'v', 'a', 'l', 'u', 'e', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', 'a', 's', 's', 'i', 'g', 'n', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', 's', 'u', 'p', 'p', 's', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };

Type type(memptr address) {

	if (cons_pool[address].carKind == NIL
			&& cons_pool[address].cdrKind == NIL) {
		// nil or true
		return TYPE_NIL_TRUE;
	}

	if (cons_pool[address].carKind == INTEGER
			&& cons_pool[address].cdrKind == NIL) {
		// integer
		return TYPE_INTEGER;
	}

	if (cons_pool[address].carKind == STRING
			&& cons_pool[address].cdrKind == INTEGER) {
		// string
		return TYPE_STRING;
	}

	if (cons_pool[address].carKind == CONS
			&& cons_pool[address].cdrKind == CONS) {
		// cons
		return TYPE_CONS;
	}

	if (cons_pool[address].carKind == NIL
			&& cons_pool[address].cdrKind == INTEGER) {
		// native function
		return TYPE_NATIVE_FUNCTION;
	}

	if (cons_pool[address].carKind == CONS
			&& cons_pool[address].cdrKind == NIL) {
		// user function expression
		return TYPE_USER_FUNCTION;
	}

	if (cons_pool[address].carKind == NIL
			&& cons_pool[address].cdrKind == CONS) {
		// user function expression
		return TYPE_OBJECT;
	}

	if (cons_pool[address].carKind == STRING
			&& cons_pool[address].cdrKind == CONS) {
		// user function expression
		return TYPE_SYMBOL;
	}

	return TYPE_ERROR;
}

memptr resolve_func_expr(memptr func_expr, memptr first_param,
		memptr optional_func_body, bool internal_object_function);
memptr lookup(memptr expr, LookUp lu);

void security_init() {
	security_head = nil;
}

memptr safe_allocate_cons() {

	if (security_head != nil) {
		cons_pool[security_tail].cdr = allocate_cons();
		cons_pool[security_tail].cdrKind = CONS;
		security_tail = cons_pool[security_tail].cdr;
		cons_pool[security_tail].carKind = NIL;
		cons_pool[security_tail].cdrKind = NIL;
		cons_pool[security_tail].car = allocate_cons();
		cons_pool[security_tail].carKind = CONS;
	} else {
		security_head = allocate_cons();
		cons_pool[security_head].carKind = NIL;
		cons_pool[security_head].cdrKind = NIL;
		cons_pool[security_head].car = allocate_cons();
		cons_pool[security_head].carKind = CONS;
		security_tail = security_head;
	}

	cons_pool[cons_pool[security_tail].car].carKind = NIL;
	cons_pool[cons_pool[security_tail].car].cdrKind = NIL;
	return cons_pool[security_tail].car;
}

// alllocate a new string from existing one, return cons address
memptr allocate_string(memptr handler_address, int stage) {

	memptr result = safe_allocate_cons();
	cons_pool[result].carKind = STRING;
	cons_pool[result].car = handler_address;
	cons_pool[result].cdrKind = INTEGER;
	cons_pool[result].cdr = stage;
	return result;
}

// returns the number of nodes in a list
int num_nodes(memptr list) {

	if (list == nil) {
		return 0;
	}

	int counter = 1;
	while (cons_pool[list].cdrKind == CONS) {
		counter++;
		list = cons_pool[list].cdr;
	}
	return counter;
}

static memptr list_insert(memptr list_head, memptr data) {

	memptr node = allocate_cons();
	cons_pool[node].carKind = CONS;
	cons_pool[node].car = data;
	cons_pool[node].cdrKind = NIL;

	memptr new_node = node, old_node;
	if (cons_pool[list_head].carKind == NIL) {
		// list is empty
		cons_pool[list_head].carKind = CONS;
		cons_pool[list_head].car = new_node;
		cons_pool[new_node].cdrKind = NIL;
	} else {
		// list is not empty
		old_node = cons_pool[list_head].car;
		cons_pool[list_head].car = new_node;
		cons_pool[new_node].cdrKind = CONS;
		cons_pool[new_node].cdr = old_node;
	}
	return new_node;
}

void insert_symbol(memptr env, memptr data) {

	list_insert(env, data);
}

bool remove_symbol(memptr env, memptr data) {

	memptr next_node, current_node, previous_node, current_data;

	previous_node = env;
	next_node = cons_pool[env].car;
	if (cons_pool[env].carKind == NIL) {
		// symbols list is empty
		return false;
	}

	do {
		current_node = next_node;
		current_data = cons_pool[current_node].car;
		if (string_compare(get_string(cons_pool[current_data].car, 0),
				get_string(cons_pool[data].car, 0))) {
			if (previous_node == env) {
				cons_pool[previous_node].carKind =
						cons_pool[current_node].cdrKind;
				cons_pool[previous_node].car = cons_pool[current_node].cdr;
			} else {
				cons_pool[previous_node].cdrKind =
						cons_pool[current_node].cdrKind;
				cons_pool[previous_node].cdr = cons_pool[current_node].cdr;
			}
			return true;
		}
		previous_node = current_node;
		next_node = cons_pool[current_node].cdr;
	} while (cons_pool[current_node].cdrKind == CONS);

	return false;
}

#define INSERT_BUILTIN_SETUP(func, i)	do {								\
												MOVE_STRING_TO_BUFFER(i);	\
												insert_builtin(func, i);	\
											} while (0)

// we assume all allocations would be successful
void insert_builtin(builtin func, int index) {

	builtins[index] = func;
	memptr data = allocate_cons();
	cons_pool[data].carKind = NIL;
	cons_pool[data].cdrKind = NIL;
	cons_pool[data].car = allocate_string_from_buffer();
	cons_pool[data].carKind = STRING;
	cons_pool[data].cdr = allocate_cons();
	cons_pool[data].cdrKind = CONS;
	cons_pool[cons_pool[data].cdr].carKind = NIL;
	cons_pool[cons_pool[data].cdr].cdr = index;
	cons_pool[cons_pool[data].cdr].cdrKind = INTEGER;
	list_insert(environment, data);
	native_implementations_array[index] = data;
}

void system_initialize() {

	initialize_memory_system();

	nil = allocate_cons();	// 0
	t = allocate_cons();	// 1
	cons_pool[nil].carKind = NIL;
	cons_pool[nil].cdrKind = NIL;
	cons_pool[t].carKind = NIL;
	cons_pool[t].cdrKind = NIL;

	environment = allocate_cons();
	cons_pool[environment].carKind = NIL;
	cons_pool[environment].cdrKind = NIL;

	INSERT_BUILTIN_SETUP(func_plus, 0);
	INSERT_BUILTIN_SETUP(func_minus, 1);
	INSERT_BUILTIN_SETUP(func_mult, 2);
	INSERT_BUILTIN_SETUP(func_div, 3);
	INSERT_BUILTIN_SETUP(func_equals, 4);
	INSERT_BUILTIN_SETUP(func_smaller, 5);
	INSERT_BUILTIN_SETUP(func_greater, 6);
	INSERT_BUILTIN_SETUP(func_cons, 7);
	INSERT_BUILTIN_SETUP(func_car, 8);
	INSERT_BUILTIN_SETUP(func_cdr, 9);
	INSERT_BUILTIN_SETUP(func_nil, 10);
	INSERT_BUILTIN_SETUP(func_true, 11);
	INSERT_BUILTIN_SETUP(func_cond, 12);
	INSERT_BUILTIN_SETUP(func_setq, 13);
	INSERT_BUILTIN_SETUP(func_defun, 14);
	INSERT_BUILTIN_SETUP(func_or, 15);
	INSERT_BUILTIN_SETUP(func_and, 16);
	INSERT_BUILTIN_SETUP(func_xor, 17);
	INSERT_BUILTIN_SETUP(func_list, 18);
	INSERT_BUILTIN_SETUP(func_value, 19);
	INSERT_BUILTIN_SETUP(func_assign, 20);
	INSERT_BUILTIN_SETUP(func_supps, 21);

	core_symbol = allocate_cons();
	cons_pool[core_symbol].car = string_copy_from_core_string();
	cons_pool[core_symbol].carKind = STRING;
	cons_pool[core_symbol].cdr = nil;
	cons_pool[core_symbol].cdrKind = CONS;
}

memptr lookup(memptr expr, LookUp lu) {

	memptr next_env = environment, current_env, next_node, current_node,
			current_data;
	do {
		current_env = next_env;
		if (cons_pool[current_env].carKind == NIL) {
			// symbols list is empty
			next_env = cons_pool[current_env].cdr;
			continue;
		}

		next_node = cons_pool[current_env].car;
		do {
			current_node = next_node;
			current_data = cons_pool[current_node].car;
			if (string_compare(get_string(cons_pool[current_data].car, 0),
					get_string(cons_pool[expr].car, 0))) {
				switch (lu) {
				case ENVIRONMENT:
					return current_env;
				case DATA:
					return cons_pool[current_data].cdr;
				default:
					return NOT_FOUND;
				}
			}
			next_node = cons_pool[current_node].cdr;
		} while (cons_pool[current_node].cdrKind != NIL);

		next_env = cons_pool[current_env].cdr;
	} while (cons_pool[current_env].cdrKind != NIL);

	return NOT_FOUND;
}

static bool create_environment(memptr values_list, memptr parameters_list,
		memptr first_param, bool first_param_computed) {

	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].carKind = NIL;
	cons_pool[local_security].cdrKind = NIL;

	int values_size = num_nodes(values_list), parameters_size = num_nodes(
			parameters_list);
	if (values_size != parameters_size) {
		ERROR(ERROR_LINE, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return false;
	}

	if (values_size == 0) {
		memptr temp = environment;
		environment = allocate_cons();
		cons_pool[environment].carKind = NIL;
		cons_pool[environment].cdrKind = CONS;
		cons_pool[environment].cdr = temp;
		return true;
	}

	memptr current_value_node, next_value_node = values_list, current_value,
			current_param_node, next_param_node = parameters_list,
			current_param, current_value_resolved, current_param_dup;

	memptr dup_list = allocate_cons(), dup_list_end = NOT_FOUND;
	cons_pool[local_security].cdr = dup_list;
	cons_pool[local_security].cdrKind = CONS;
	cons_pool[dup_list].carKind = NIL;
	cons_pool[dup_list].cdrKind = NIL;
	bool first = true;
	do {
		current_value_node = next_value_node;
		current_param_node = next_param_node;
		current_value = cons_pool[current_value_node].car;
		current_param = cons_pool[current_param_node].car;

		current_value_resolved =
				(first_param_computed && first) ?
						first_param : resolve_expr(current_value);
		if (current_value_resolved == NOT_FOUND) {
			ERROR(ERROR_LINE, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			return false;
		}
		cons_pool[local_security].car = current_value_resolved;
		cons_pool[local_security].carKind = CONS;

		announce_allocation(2);
		current_param_dup = allocate_cons();
		cons_pool[current_param_dup].carKind = STRING;
		cons_pool[current_param_dup].car = cons_pool[current_param].car;
		cons_pool[current_param_dup].cdrKind = CONS;
		cons_pool[current_param_dup].cdr = current_value_resolved;
		if (dup_list_end == NOT_FOUND) {
			cons_pool[dup_list].car = current_param_dup;
			cons_pool[dup_list].carKind = CONS;
			cons_pool[dup_list].cdrKind = NIL;
			dup_list_end = dup_list;
		} else {
			cons_pool[dup_list_end].cdr = allocate_cons();
			cons_pool[dup_list_end].cdrKind = CONS;
			dup_list_end = cons_pool[dup_list_end].cdr;
			cons_pool[dup_list_end].car = current_param_dup;
			cons_pool[dup_list_end].carKind = CONS;
			cons_pool[dup_list_end].cdrKind = NIL;
		}

		next_value_node = cons_pool[current_value_node].cdr;
		next_param_node = cons_pool[current_param_node].cdr;
		first = false;
		cons_pool[local_security].carKind = NIL;
	} while (cons_pool[current_value_node].cdrKind == CONS);

	memptr temp = environment;
	environment = allocate_cons();
	cons_pool[environment].cdrKind = CONS;
	cons_pool[environment].cdr = temp;
	cons_pool[environment].carKind = CONS;
	cons_pool[environment].car = dup_list;

	return true;
}

static void destroy_environment() {

	// we assume the base environment is not being destroyed
	environment = cons_pool[environment].cdr;
}

memptr resolve_expr(memptr expr) {

	Type expr_type = type(expr);
	if (expr_type == TYPE_NIL_TRUE || expr_type == TYPE_INTEGER
			|| expr_type == TYPE_STRING || expr_type == TYPE_CONS
			|| expr_type == TYPE_NATIVE_FUNCTION || expr_type == TYPE_OBJECT) {
		return expr;
	}

	if (expr_type == TYPE_USER_FUNCTION) {
		return resolve_func_expr(cons_pool[expr].car, NOT_FOUND, NOT_FOUND,
		false);
	}

	if (expr_type == TYPE_SYMBOL) {
		return lookup(expr, DATA);
	}

#ifdef DEBUG
	printf("38\n");
#endif
	ERROR(ERROR_LINE, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
	return NOT_FOUND;
}

memptr resolve_func_expr(memptr func_expr, memptr first_param,
		memptr optional_func_body, bool internal_object_function) {

#ifdef EXTENDED_SECURITY
	if (internal_object_function) {
		memptr advenced_security = safe_allocate_cons();
		cons_pool[advenced_security].car = first_param;
		cons_pool[advenced_security].carKind = CONS;
		cons_pool[advenced_security].cdr = optional_func_body;
		cons_pool[advenced_security].cdrKind = CONS;
	}
#endif

	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].carKind = NIL;
	cons_pool[local_security].cdrKind = NIL;

	memptr func_symbol = cons_pool[func_expr].car;
	memptr func_body;
	Type func_body_type;

	func_body =
			internal_object_function ?
					optional_func_body : resolve_expr(cons_pool[func_expr].car);
	cons_pool[local_security].car = func_body;
	cons_pool[local_security].carKind = CONS;
	func_body_type = type(func_body);
	if (func_body_type == TYPE_NATIVE_FUNCTION) {
		// builtin function
		return builtins[cons_pool[func_body].cdr](func_expr);
	}

	if (num_nodes(func_expr) > 1) {

		first_param =
				internal_object_function ?
						first_param :
						resolve_expr(cons_pool[cons_pool[func_expr].cdr].car);
		cons_pool[local_security].cdr = first_param;
		cons_pool[local_security].cdrKind = CONS;
		internal_object_function = true;
		if ((type(first_param) == TYPE_OBJECT)
				&& (type(func_symbol) == TYPE_SYMBOL)) {
			memptr lu = object_lookup(first_param, func_symbol);
			if (lu != NOT_FOUND) {
				if ((num_nodes(func_expr) == 2)
						&& (type(lu) != TYPE_NATIVE_FUNCTION)
						&& (type(lu) != TYPE_USER_FUNCTION)) {
					return lu;
				}
				func_body = lu;
				cons_pool[local_security].car = func_body;
				func_body_type = type(func_body);
			}
		}
	}

	if (func_body_type == TYPE_USER_FUNCTION) {
		// user made function
		bool create_environment_success = create_environment(
				cons_pool[func_expr].cdrKind == NIL ?
						nil : cons_pool[func_expr].cdr,
				cons_pool[cons_pool[func_body].car].car, first_param,
				internal_object_function);
		if (!create_environment_success) {
#ifdef DEBUG
			printf("39\n");
#endif
			return NOT_FOUND;
		} else {
			memptr result = resolve_expr(
					cons_pool[cons_pool[cons_pool[func_body].car].cdr].car);
			destroy_environment();
			return result;
		}
	}

#ifdef DEBUG
	printf("40\n");
#endif
	ERROR(ERROR_LINE, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
	return NOT_FOUND;
}
