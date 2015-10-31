#include "native.h"
#include "system.h"
#include "error.h"

#define OBJECT_OPERATE_CORE (false)
//#define DEBUG
//#define DEFUN_SECURE_BODY
//#define EXTENDED_SECURITY

int fix_integer(int num) {
	if (num & (1 << 13)) {
		return -((num ^ ((1 << 14) - 1)) + 1);
	}
	return num;
}

typedef enum {
	PLUS, MINUS, MULT, DIV, EQUAL, SMALLER, GREATER, OR, AND, XOR
} JoinOperation;

memptr do_join_operation(memptr func_expr, JoinOperation operation_type) {

	if (num_nodes(func_expr) != 3) {
#ifdef DEBUG
		printf("1\n");
#endif
		switch (operation_type) {
		case PLUS:
			ERROR(ERROR_PLUS, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		case MINUS:
			ERROR(ERROR_MINUS, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		case MULT:
			ERROR(ERROR_MULT, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		case DIV:
			ERROR(ERROR_DIV, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		case EQUAL:
			ERROR(ERROR_EQUALS, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		case SMALLER:
			ERROR(ERROR_SMALLER, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		case GREATER:
			ERROR(ERROR_GREATER, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		case OR:
			ERROR(ERROR_OR, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		case AND:
			ERROR(ERROR_AND, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		case XOR:
			ERROR(ERROR_XOR, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
			break;
		default:
			break;
		}
		return NOT_FOUND;
	}

	memptr current_node = cons_pool[func_expr].cdr;
	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].carKind = NIL;
	cons_pool[local_security].cdrKind = NIL;
	memptr first_param = resolve_expr(cons_pool[current_node].car);
	if (first_param == NOT_FOUND) {
#ifdef DEBUG
		printf("2\n");
#endif
		switch (operation_type) {
		case PLUS:
			ERROR(ERROR_PLUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case MINUS:
			ERROR(ERROR_MINUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case MULT:
			ERROR(ERROR_MULT, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case DIV:
			ERROR(ERROR_DIV, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case EQUAL:
			ERROR(ERROR_EQUALS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case SMALLER:
			ERROR(ERROR_SMALLER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case GREATER:
			ERROR(ERROR_GREATER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case OR:
			ERROR(ERROR_OR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case AND:
			ERROR(ERROR_AND, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case XOR:
			ERROR(ERROR_XOR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		default:
			break;
		}
		return NOT_FOUND;
	}
	cons_pool[local_security].car = first_param;
	cons_pool[local_security].carKind = CONS;
	Type first_param_type = type(first_param);
	if (first_param_type == TYPE_OBJECT) {
		memptr lu = object_lookup(first_param, cons_pool[func_expr].car);
		if (lu == NOT_FOUND) {
			first_param = object_lookup(first_param, core_symbol);
			first_param_type = type(first_param);
		} else {
			return resolve_func_expr(func_expr, first_param, lu, true);
		}
	}

	current_node = cons_pool[current_node].cdr;
	memptr second_param = resolve_expr(cons_pool[current_node].car);
	if (second_param == NOT_FOUND) {
#ifdef DEBUG
		printf("3\n");
#endif
		switch (operation_type) {
		case PLUS:
			ERROR(ERROR_PLUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case MINUS:
			ERROR(ERROR_MINUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case MULT:
			ERROR(ERROR_MULT, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case DIV:
			ERROR(ERROR_DIV, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case EQUAL:
			ERROR(ERROR_EQUALS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case SMALLER:
			ERROR(ERROR_SMALLER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case GREATER:
			ERROR(ERROR_GREATER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case OR:
			ERROR(ERROR_OR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case AND:
			ERROR(ERROR_AND, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		case XOR:
			ERROR(ERROR_XOR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			break;
		default:
			break;
		}
		return NOT_FOUND;
	}
	cons_pool[local_security].cdr = second_param;
	cons_pool[local_security].cdrKind = CONS;
	Type second_param_type = type(second_param);
	if (second_param_type == TYPE_OBJECT) {
		second_param = object_lookup(second_param, core_symbol);
		second_param_type = type(second_param);
	}

	memptr result = NOT_FOUND;
	if (operation_type == PLUS || operation_type == MINUS
			|| operation_type == MULT || operation_type == DIV) {
		if (first_param_type != TYPE_INTEGER
				|| second_param_type != TYPE_INTEGER) {
#ifdef DEBUG
			printf("4\n");
#endif
			switch (operation_type) {
			case PLUS:
				ERROR(ERROR_PLUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case MINUS:
				ERROR(ERROR_MINUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case MULT:
				ERROR(ERROR_MULT, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case DIV:
				ERROR(ERROR_DIV, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case EQUAL:
				ERROR(ERROR_EQUALS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case SMALLER:
				ERROR(ERROR_SMALLER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case GREATER:
				ERROR(ERROR_GREATER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case OR:
				ERROR(ERROR_OR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case AND:
				ERROR(ERROR_AND, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case XOR:
				ERROR(ERROR_XOR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			default:
				break;
			}
			return NOT_FOUND;
		}
		int first_value = fix_integer(cons_pool[first_param].car);
		int second_value = fix_integer(cons_pool[second_param].car);
		result = allocate_cons();
		cons_pool[result].carKind = INTEGER;
		cons_pool[result].cdrKind = NIL;
		switch (operation_type) {
		case PLUS:
			cons_pool[result].car = first_value + second_value;
			break;
		case MINUS:
			cons_pool[result].car = first_value - second_value;
			break;
		case MULT:
			cons_pool[result].car = first_value * second_value;
			break;
		case DIV:
			if (second_value == 0) {
#ifdef DEBUG
				printf("5\n");
#endif
				return NOT_FOUND;
			}
			cons_pool[result].car = first_value / second_value;
			break;
		default:
			break;
		}
	} else if (operation_type == EQUAL || operation_type == SMALLER
			|| operation_type == GREATER) {
		if (first_param_type != second_param_type) {
#ifdef DEBUG
			printf("6\n");
#endif
			switch (operation_type) {
			case PLUS:
				ERROR(ERROR_PLUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case MINUS:
				ERROR(ERROR_MINUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case MULT:
				ERROR(ERROR_MULT, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case DIV:
				ERROR(ERROR_DIV, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case EQUAL:
				ERROR(ERROR_EQUALS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case SMALLER:
				ERROR(ERROR_SMALLER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case GREATER:
				ERROR(ERROR_GREATER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case OR:
				ERROR(ERROR_OR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case AND:
				ERROR(ERROR_AND, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case XOR:
				ERROR(ERROR_XOR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			default:
				break;
			}
		}
		int first_value;
		int second_value;
		if (first_param_type == TYPE_NIL_TRUE) {
			first_value = ((first_param == nil) ? 0 : 1);
			second_value = ((second_param == nil) ? 0 : 1);
		} else if (first_param_type == TYPE_INTEGER) {
			first_value = fix_integer(cons_pool[first_param].car);
			second_value = fix_integer(cons_pool[second_param].car);
		} else {
			// addresses comparison
			first_value = first_param;
			second_value = second_param;
		}
		// strings logic
		if (first_param_type == TYPE_STRING) {
			bool full_equal = false;
			int len1 = string_length(first_param);
			int len2 = string_length(second_param);
			if ((len1 == 1) || (len2 == 1)) {
				first_value =
						strings_pool[get_string(cons_pool[first_param].car,
								cons_pool[first_param].cdr)];
				second_value = strings_pool[get_string(
						cons_pool[second_param].car,
						cons_pool[second_param].cdr)];
				if (len1 != 1) {
					if (first_value == second_value) {
						first_value++;
					}
				}
				if (len2 != 1) {
					if (first_value == second_value) {
						second_value++;
					}
				}
			} else {
				int iter1 = get_string(cons_pool[first_param].car,
						cons_pool[first_param].cdr), iter2 = get_string(
						cons_pool[second_param].car,
						cons_pool[second_param].cdr);
				first_value = second_value = 0;
				while (first_value == second_value
						&& strings_pool[iter1] != '\0'
						&& strings_pool[iter2] != '\0') {
					first_value = strings_pool[iter1++];
					second_value = strings_pool[iter2++];
				}
				if ((strings_pool[iter1] != '\0')
						&& (strings_pool[iter2] == '\0')
						&& (first_value == second_value)) {
					first_value++;
				} else if ((strings_pool[iter1] == '\0')
						&& (strings_pool[iter2] != '\0')
						&& (first_value == second_value)) {
					second_value++;
				}
			}
		}
		switch (operation_type) {
		case EQUAL:
			result = ((first_value == second_value) ? t : nil);
			break;
		case SMALLER:
			result = ((first_value < second_value) ? t : nil);
			break;
		case GREATER:
			result = ((first_value > second_value) ? t : nil);
			break;
		default:
#ifdef DEBUG
			printf("7\n");
#endif
			return NOT_FOUND;
			break;
		}
	} else if (operation_type == XOR || operation_type == OR
			|| operation_type == AND) {
		if (first_param_type != TYPE_NIL_TRUE
				|| second_param_type != TYPE_NIL_TRUE) {
#ifdef DEBUG
			printf("7.5\n");
#endif
			switch (operation_type) {
			case PLUS:
				ERROR(ERROR_PLUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case MINUS:
				ERROR(ERROR_MINUS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case MULT:
				ERROR(ERROR_MULT, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case DIV:
				ERROR(ERROR_DIV, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case EQUAL:
				ERROR(ERROR_EQUALS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case SMALLER:
				ERROR(ERROR_SMALLER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case GREATER:
				ERROR(ERROR_GREATER, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case OR:
				ERROR(ERROR_OR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case AND:
				ERROR(ERROR_AND, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			case XOR:
				ERROR(ERROR_XOR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
				break;
			default:
				break;
			}
			return NOT_FOUND;
		}
		switch (operation_type) {
		case XOR:
			result = ((first_param != second_param) ? t : nil);
			break;
		case OR:
			result = ((first_param == t || second_param == t) ? t : nil);
			break;
		case AND:
			result = ((first_param == t && second_param == t) ? t : nil);
			break;
		default:
#ifdef DEBUG
			printf("8\n");
#endif
			return NOT_FOUND;
			break;
		}
	}

	return result;
}

// + function for 2 integer parameters
memptr func_plus(memptr func_expr) {

	return do_join_operation(func_expr, PLUS);
}

// - function for 2 integer parameters
memptr func_minus(memptr func_expr) {

	return do_join_operation(func_expr, MINUS);
}

// * function for 2 integer parameters
memptr func_mult(memptr func_expr) {

	return do_join_operation(func_expr, MULT);
}

// / function for 2 integer parameters
memptr func_div(memptr func_expr) {

	return do_join_operation(func_expr, DIV);
}

// == function for 2 parameters
memptr func_equals(memptr func_expr) {

	return do_join_operation(func_expr, EQUAL);
}

// < function for 2 parameters
memptr func_smaller(memptr func_expr) {

	return do_join_operation(func_expr, SMALLER);
}

// > function for 2 parameters
memptr func_greater(memptr func_expr) {

	return do_join_operation(func_expr, GREATER);
}

// or function for 2 parameters
memptr func_or(memptr func_expr) {

	return do_join_operation(func_expr, OR);
}

// and function for 2 parameters
memptr func_and(memptr func_expr) {

	return do_join_operation(func_expr, AND);
}

// xor function for 2 parameters
memptr func_xor(memptr func_expr) {

	return do_join_operation(func_expr, XOR);
}

memptr func_cons(memptr func_expr) {

	if (num_nodes(func_expr) != 3) {
#ifdef DEBUG
		printf("9\n");
#endif
		ERROR(ERROR_CONS, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

	memptr res = safe_allocate_cons();
	cons_pool[res].carKind = NIL;
	cons_pool[res].cdrKind = NIL;

	memptr current_node, current_data;
	// first parameter
	current_node = cons_pool[func_expr].cdr;
	current_data = cons_pool[current_node].car;
	cons_pool[res].car = resolve_expr(current_data);
	cons_pool[res].carKind = CONS;
	if (type(cons_pool[res].car) == TYPE_OBJECT) {
		memptr lu = object_lookup(cons_pool[res].car, cons_pool[func_expr].car);
		if (lu == NOT_FOUND && OBJECT_OPERATE_CORE) {
			cons_pool[res].car = object_lookup(cons_pool[res].car, core_symbol);
		} else if (lu != NOT_FOUND) {
			return resolve_func_expr(func_expr, cons_pool[res].car, lu, true);
		}
	}

	// second parameter
	current_node = cons_pool[current_node].cdr;
	current_data = cons_pool[current_node].car;
	cons_pool[res].cdr = resolve_expr(current_data);
	cons_pool[res].cdrKind = CONS;
	if (type(cons_pool[res].cdr) == TYPE_OBJECT && OBJECT_OPERATE_CORE) {
		cons_pool[res].cdr = object_lookup(cons_pool[res].cdr, core_symbol);
	}

	return res;
}

memptr func_car(memptr func_expr) {

	if (num_nodes(func_expr) != 2) {
#ifdef DEBUG
		printf("10\n");
#endif
		ERROR(ERROR_CAR, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].carKind = NIL;
	cons_pool[local_security].cdrKind = NIL;
	memptr param = resolve_expr(cons_pool[cons_pool[func_expr].cdr].car);
	cons_pool[local_security].car = param;
	cons_pool[local_security].carKind = CONS;

	while (true) {
		Type param_type = type(param);
		if (param_type == TYPE_CONS) {
			return cons_pool[param].car;
		} else if (param_type == TYPE_STRING) {
			memptr result = allocate_cons();
			cons_pool[result].car = cons_pool[param].car;
			if (cons_pool[param].cdr & (1 << 13)) {
				cons_pool[result].cdr = cons_pool[param].cdr;
			} else {
				cons_pool[result].cdr = 0 - cons_pool[param].cdr - 1;
			}
			cons_pool[result].carKind = STRING;
			cons_pool[result].cdrKind = INTEGER;
			return result;
		} else if (param_type == TYPE_OBJECT) {
			memptr lu = object_lookup(param, cons_pool[func_expr].car);
			if (lu == NOT_FOUND) {
				param = object_lookup(param, core_symbol);
				continue;
			} else {
				return resolve_func_expr(func_expr, param, lu, true);
			}
		}
		break;
	}
#ifdef DEBUG
	printf("11\n");
#endif
	ERROR(ERROR_CAR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
	return NOT_FOUND;
}

memptr func_cdr(memptr func_expr) {

	if (num_nodes(func_expr) != 2) {
#ifdef DEBUG
		printf("12\n");
#endif
		ERROR(ERROR_CDR, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].carKind = NIL;
	cons_pool[local_security].cdrKind = NIL;
	memptr param = resolve_expr(cons_pool[cons_pool[func_expr].cdr].car);
	cons_pool[local_security].car = param;
	cons_pool[local_security].carKind = CONS;

	while (true) {
		Type param_type = type(param);
		if (param_type == TYPE_CONS) {
			return cons_pool[param].cdr;
		} else if (param_type == TYPE_STRING) {
			memptr result;
			if ((cons_pool[param].cdr < 0) || (string_length(param) == 1)) {
				result = nil;
			} else {
				result = allocate_cons();
				cons_pool[result].car = cons_pool[param].car;
				cons_pool[result].cdr = cons_pool[param].cdr + 1;
				cons_pool[result].carKind = STRING;
				cons_pool[result].cdrKind = INTEGER;
			}
			return result;
		} else if (param_type == TYPE_OBJECT) {
			memptr lu = object_lookup(param, cons_pool[func_expr].car);
			if (lu == NOT_FOUND) {
				param = object_lookup(param, core_symbol);
				continue;
			} else {
				return resolve_func_expr(func_expr, param, lu, true);
			}
		}
		break;
	}

#ifdef DEBUG
	printf("13\n");
#endif
	ERROR(ERROR_CDR, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
	return NOT_FOUND;
}

memptr func_nil(memptr func_expr) {

	if (num_nodes(func_expr) != 2) {
#ifdef DEBUG
		printf("14\n");
#endif
		ERROR(ERROR_NIL, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

#ifdef EXTENDED_SECURITY
	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].carKind = NIL;
	cons_pool[local_security].cdrKind = NIL;
#endif
	memptr param = resolve_expr(cons_pool[cons_pool[func_expr].cdr].car);
#ifdef EXTENDED_SECURITY
	cons_pool[local_security].car = param;
	cons_pool[local_security].carKind = CONS;
#endif
	if (type(param) == TYPE_OBJECT) {
		memptr lu = object_lookup(param, cons_pool[func_expr].car);
		if (lu == NOT_FOUND) {
			param = object_lookup(param, core_symbol);
		} else {
			return resolve_func_expr(func_expr, param, lu, true);
		}
	}
	return ((param == nil) ? t : nil);
}

memptr func_true(memptr func_expr) {

	if (num_nodes(func_expr) != 2) {
#ifdef DEBUG
		printf("15\n");
#endif
		ERROR(ERROR_TRUE, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

#ifdef EXTENDED_SECURITY
	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].carKind = NIL;
	cons_pool[local_security].cdrKind = NIL;
#endif
	memptr param = resolve_expr(cons_pool[cons_pool[func_expr].cdr].car);
#ifdef EXTENDED_SECURITY
	cons_pool[local_security].car = param;
	cons_pool[local_security].carKind = CONS;
#endif
	if (type(param) == TYPE_OBJECT) {
		memptr lu = object_lookup(param, cons_pool[func_expr].car);
		if (lu == NOT_FOUND) {
			param = object_lookup(param, core_symbol);
		} else {
			return resolve_func_expr(func_expr, param, lu, true);
		}
	}
	return ((param == t) ? t : nil);
}

memptr func_cond(memptr func_expr) {

	if (num_nodes(func_expr) == 1) {
		// no parameters
#ifdef DEBUG
		printf("16\n");
#endif
		ERROR(ERROR_COND, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

	memptr current_node, current_data, condition, result;
	current_node = func_expr;
	Type current_data_type;
	while (cons_pool[current_node].cdrKind == CONS) {

		current_node = cons_pool[current_node].cdr;
		current_data = cons_pool[current_node].car;
		current_data_type = type(current_data);
		if (current_data_type == TYPE_CONS) {
			// cons support
			condition = cons_pool[current_data].car;
			result = cons_pool[current_data].cdr;
		} else if (current_data_type == TYPE_USER_FUNCTION) {
			// "function" support (interpreted as a function by the translator)
			current_data = cons_pool[current_data].car;
			if (num_nodes(current_data) != 2) {
#ifdef DEBUG
				printf("17\n");
#endif
				ERROR(ERROR_COND, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
				return NOT_FOUND;
			}
			condition = cons_pool[current_data].car;
			result = cons_pool[cons_pool[current_data].cdr].car;
		} else {
			// invalid
#ifdef DEBUG
			printf("18\n");
#endif
			ERROR(ERROR_COND, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
			return NOT_FOUND;
		}

		memptr condition_result = resolve_expr(condition);
		if (condition_result == NOT_FOUND) {
#ifdef DEBUG
			printf("19\n");
#endif
			ERROR(ERROR_COND, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			return NOT_FOUND;
		} else if (condition_result != nil && condition_result != t) {
#ifdef DEBUG
			printf("20\n");
#endif
			ERROR(ERROR_COND, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			return NOT_FOUND;
		}
		if (condition_result == t) {
#ifdef EXTENDED_SECURITY
			memptr local_security = safe_allocate_cons();
			cons_pool[local_security].car = result;
			cons_pool[local_security].carKind = CONS;
			cons_pool[local_security].cdrKind = NIL;
#endif
			return resolve_expr(result);
		}
	}

#ifdef DEBUG
	printf("21\n");
#endif
	ERROR(ERROR_COND, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
	return NOT_FOUND;
}

memptr func_setq(memptr func_expr) {

	if (num_nodes(func_expr) != 3) {
		// invalid call
#ifdef DEBUG
		printf("22\n");
#endif
		ERROR(ERROR_SETQ, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].carKind = NIL;
	cons_pool[local_security].cdrKind = NIL;

	memptr current_node = cons_pool[func_expr].cdr;
	memptr current_param = cons_pool[current_node].car;
	Type first_type = type(current_param);
	if (first_type != TYPE_SYMBOL) {
		// first argument not a name
#ifdef DEBUG
		printf("23\n");
#endif
		ERROR(ERROR_SETQ, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
		return NOT_FOUND;
	}
	memptr symbol_name = current_param;

	current_node = cons_pool[current_node].cdr;
	current_param = cons_pool[current_node].car;
	memptr value = resolve_expr(current_param);
	if (value == NOT_FOUND) {
		// invalid value
#ifdef DEBUG
		printf("24\n");
#endif
		ERROR(ERROR_SETQ, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
		return NOT_FOUND;
	}
	cons_pool[local_security].car = value;
	cons_pool[local_security].carKind = CONS;

	// defining the new symbol
	memptr symbol = allocate_cons();
	cons_pool[local_security].cdr = symbol;
	cons_pool[local_security].cdrKind = CONS;
	cons_pool[symbol].car = cons_pool[symbol_name].car;
	cons_pool[symbol].carKind = STRING;
	cons_pool[symbol].cdr = value;
	cons_pool[symbol].cdrKind = CONS;
	memptr env = lookup(symbol, ENVIRONMENT);
	if (env == NOT_FOUND) {
		// inserting to current environment
		insert_symbol(environment, symbol); // includes 1 allocation
	} else {
		// replacing previous interpretation
		remove_symbol(env, symbol);
		insert_symbol(env, symbol); // includes 1 allocation
	}

	return value;
}

memptr func_defun(memptr func_expr) {

	if (num_nodes(func_expr) != 4) {
#ifdef DEBUG
		printf("25\n");
#endif
		ERROR(ERROR_DEFUN, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

	memptr name = cons_pool[cons_pool[func_expr].cdr].car;
	if (cons_pool[name].carKind != STRING || cons_pool[name].cdrKind != CONS) {
		// first argument not a name
#ifdef DEBUG
		printf("26\n");
#endif
		ERROR(ERROR_DEFUN, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
		return NOT_FOUND;
	}

	memptr parameters = cons_pool[cons_pool[cons_pool[func_expr].cdr].cdr].car;
	if (parameters != nil) {
		if (cons_pool[parameters].carKind != CONS
				|| cons_pool[parameters].cdrKind != NIL) {
			// second argument not a parameters list
#ifdef DEBUG
			printf("27\n");
#endif
			ERROR(ERROR_DEFUN, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
			return NOT_FOUND;
		}

		memptr current_param_node, next_param_node = cons_pool[parameters].car,
				current_param;
		// function has parameter(s)
		do {
			current_param_node = next_param_node;
			current_param = cons_pool[current_param_node].car;
			if (cons_pool[current_param].carKind != STRING
					|| cons_pool[current_param].cdrKind != CONS) {
				// a parameter is not a name
#ifdef DEBUG
				printf("28\n");
#endif
				ERROR(ERROR_DEFUN, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
				return NOT_FOUND;
			}
			next_param_node = cons_pool[next_param_node].cdr;
		} while (cons_pool[current_param_node].cdrKind == CONS);
	}

	// now we can build the function representation in the memory pool,
	// and connect the functions name to it
	memptr function = safe_allocate_cons(), current;
	cons_pool[function].car = cons_pool[name].car;
	cons_pool[function].carKind = STRING;
	cons_pool[function].cdr = allocate_cons();
	cons_pool[function].cdrKind = CONS;

	// the name value - a user function
	current = cons_pool[function].cdr;
	cons_pool[current].car = allocate_cons();
	cons_pool[current].carKind = CONS;
	cons_pool[current].cdrKind = NIL;

	// first argument - parameters list
	current = cons_pool[current].car;
	cons_pool[current].car = (
			parameters == nil ? nil : cons_pool[parameters].car);
	cons_pool[current].carKind = CONS;
	cons_pool[current].cdr = allocate_cons();
	cons_pool[current].cdrKind = CONS;

	// second argument - function body
	current = cons_pool[current].cdr;
	cons_pool[current].carKind = CONS;
	cons_pool[current].cdrKind = NIL;
	cons_pool[current].car =
			cons_pool[cons_pool[cons_pool[cons_pool[func_expr].cdr].cdr].cdr].car;
#ifdef DEFUN_SECURE_BODY
	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].car = cons_pool[current].car;
	cons_pool[local_security].carKind = CONS;
	cons_pool[local_security].cdrKind = NIL;
#endif

	// hashing the function into the system
	memptr env = lookup(function, ENVIRONMENT);
	if (env == NOT_FOUND) {
		// inserting to current environment
		insert_symbol(environment, function);
	} else {
		// replacing previous interpretation
		remove_symbol(env, function);
		insert_symbol(env, function);
	}

	return cons_pool[function].cdr;
}

memptr func_list(memptr func_expr) {

	if (num_nodes(func_expr) == 1) {
		return nil;
	}

	memptr current_node, current_param, current_dup_node = safe_allocate_cons(),
			base = current_dup_node;
	cons_pool[current_dup_node].carKind = NIL;
	cons_pool[current_dup_node].cdrKind = NIL;

	current_node = cons_pool[func_expr].cdr;
	current_param = resolve_expr(cons_pool[current_node].car);
	if (current_param == NOT_FOUND) {
#ifdef DEBUG
		printf("29\n");
#endif
		ERROR(ERROR_LIST, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
		return NOT_FOUND;
	} else if (type(current_param) == TYPE_OBJECT) {
		memptr lu = object_lookup(current_param, cons_pool[func_expr].car);
		if (lu != NOT_FOUND) {
#ifdef EXTENDED_SECURITY
			memptr local_security = safe_allocate_cons();
			cons_pool[local_security].car = current_param;
			cons_pool[local_security].carKind = CONS;
			cons_pool[local_security].cdrKind = NIL;
#endif
			return resolve_func_expr(func_expr, current_param, lu, true);
		}
	}
	cons_pool[current_dup_node].car = current_param;
	cons_pool[current_dup_node].carKind = CONS;

	while (cons_pool[current_node].cdrKind == CONS) {

		current_node = cons_pool[current_node].cdr;
		cons_pool[current_dup_node].cdr = allocate_cons();
		cons_pool[current_dup_node].cdrKind = CONS;
		current_dup_node = cons_pool[current_dup_node].cdr;
		cons_pool[current_dup_node].carKind = NIL;
		cons_pool[current_dup_node].cdrKind = NIL;

		current_param = resolve_expr(cons_pool[current_node].car);
		if (current_param == NOT_FOUND) {
#ifdef DEBUG
			printf("30\n");
#endif
			ERROR(ERROR_LIST, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			return NOT_FOUND;
		}
		cons_pool[current_dup_node].car = current_param;
		cons_pool[current_dup_node].carKind = CONS;
	}

	cons_pool[current_dup_node].cdrKind = CONS;
	cons_pool[current_dup_node].cdr = nil;

	return base;
}

memptr func_value(memptr func_expr) {

	if (num_nodes(func_expr) < 2) {
#ifdef DEBUG
		printf("31\n");
#endif
		ERROR(ERROR_VALUE, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

	memptr next_node = cons_pool[func_expr].cdr, current_node, current_value;
	bool first_param = true;
	do {
#ifdef EXTENDED_SECURITY
		memptr local_security = safe_allocate_cons();
		cons_pool[local_security].carKind = NIL;
		cons_pool[local_security].cdrKind = NIL;
#endif
		current_node = next_node;
		current_value = resolve_expr(cons_pool[current_node].car);
#ifdef EXTENDED_SECURITY
		cons_pool[local_security].car = current_value;
		cons_pool[local_security].carKind = CONS;
#endif
		if (current_value == NOT_FOUND) {
#ifdef DEBUG
			printf("32\n");
#endif
			ERROR(ERROR_VALUE, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
			return NOT_FOUND;
		} else if (first_param && (type(current_value) == TYPE_OBJECT)) {
			memptr lu = object_lookup(current_value, cons_pool[func_expr].car);
			if (lu != NOT_FOUND) {
				return resolve_func_expr(func_expr, current_value, lu, true);
			}
		}
		next_node = cons_pool[current_node].cdr;
		first_param = false;
	} while (cons_pool[current_node].cdrKind == CONS);

	return current_value;
}

memptr func_assign(memptr func_expr) {

	if (num_nodes(func_expr) != 4) {
#ifdef DEBUG
		printf("33\n");
#endif
		ERROR(ERROR_ASSIGN, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

	memptr local_security = safe_allocate_cons();
	cons_pool[local_security].carKind = NIL;
	cons_pool[local_security].cdrKind = NIL;
	memptr current_node = cons_pool[func_expr].cdr;
	memptr symbol = cons_pool[current_node].car;
	current_node = cons_pool[current_node].cdr;
	memptr value = cons_pool[current_node].car;
	current_node = cons_pool[current_node].cdr;
	memptr obj = cons_pool[current_node].car;

	// first resolve the object, then the value
	obj = resolve_expr(obj);
	if (obj == NOT_FOUND) {
#ifdef DEBUG
		printf("34\n");
#endif
		ERROR(ERROR_ASSIGN, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
		return NOT_FOUND;
	}
	cons_pool[local_security].car = obj;
	cons_pool[local_security].carKind = CONS;

	value = resolve_expr(value);
	if (value == NOT_FOUND) {
#ifdef DEBUG
		printf("35\n");
#endif
		ERROR(ERROR_ASSIGN, ERROR_FAILURE, ERROR_INVALID, ERROR_MALFORMED);
		return NOT_FOUND;
	}
	cons_pool[local_security].cdr = value;
	cons_pool[local_security].cdrKind = CONS;

	memptr result = NOT_FOUND;
	if (type(obj) == TYPE_OBJECT) {

		announce_allocation(2);
		memptr new_symbol = allocate_cons();
		cons_pool[new_symbol].car = cons_pool[symbol].car;
		cons_pool[new_symbol].carKind = STRING;
		cons_pool[new_symbol].cdr = value;
		cons_pool[new_symbol].cdrKind = CONS;
		object_remove_symbol(obj, new_symbol);
		object_insert_symbol(obj, new_symbol); // includes 1 allocation
		result = obj;
	} else {

		announce_allocation(5);
		memptr new_obj = allocate_cons();
		cons_pool[new_obj].carKind = NIL;
		cons_pool[new_obj].cdr = allocate_cons();
		cons_pool[new_obj].cdrKind = CONS;
		memptr new_node = cons_pool[new_obj].cdr;
		cons_pool[new_node].car = allocate_cons();
		cons_pool[new_node].carKind = CONS;
		cons_pool[new_node].cdr = allocate_cons();
		cons_pool[new_node].cdrKind = CONS;
		memptr new_symbol = cons_pool[new_node].car;
		cons_pool[new_symbol].car = cons_pool[symbol].car;
		cons_pool[new_symbol].carKind = STRING;
		cons_pool[new_symbol].cdr = value;
		cons_pool[new_symbol].cdrKind = CONS;
		memptr new_core_node = cons_pool[new_node].cdr;
		cons_pool[new_core_node].cdrKind = NIL;
		cons_pool[new_core_node].car = allocate_cons();
		cons_pool[new_core_node].carKind = CONS;
		memptr new_core_symbol = cons_pool[new_core_node].car;
		cons_pool[new_core_symbol].car = cons_pool[core_symbol].car;
		cons_pool[new_core_symbol].carKind = STRING;
		cons_pool[new_core_symbol].cdr = obj;
		cons_pool[new_core_symbol].cdrKind = CONS;
		result = new_obj;
	}
	return result;
}

#define SUPPORTS(index) ((string_compare(									\
		get_string(cons_pool[native_implementations_array[index]].car, 0),	\
		get_string(cons_pool[symbol].car, 0))))
static bool supps(memptr value, memptr symbol) {

	bool s = false;
	Type value_type = type(value);
	switch (value_type) {
	case TYPE_CONS:
		s = (SUPPORTS(4) || SUPPORTS(5) || SUPPORTS(6) || SUPPORTS(7)
				|| SUPPORTS(8) || SUPPORTS(9) || SUPPORTS(10) || SUPPORTS(11)
				|| SUPPORTS(18) || SUPPORTS(19));
		break;
	case TYPE_INTEGER:
		s =
				(SUPPORTS(0) || SUPPORTS(1) || SUPPORTS(2) || SUPPORTS(3)
						|| SUPPORTS(4) || SUPPORTS(5) || SUPPORTS(6)
						|| SUPPORTS(7) || SUPPORTS(10) || SUPPORTS(11)
						|| SUPPORTS(18) || SUPPORTS(19));
		break;
	case TYPE_NATIVE_FUNCTION:
		s =
				(SUPPORTS(4) || SUPPORTS(5) || SUPPORTS(6) || SUPPORTS(7)
						|| SUPPORTS(10) || SUPPORTS(11) || SUPPORTS(18)
						|| SUPPORTS(19));
		break;
	case TYPE_NIL_TRUE:
		s = (SUPPORTS(4) || SUPPORTS(5) || SUPPORTS(6) || SUPPORTS(7)
				|| SUPPORTS(10) || SUPPORTS(11) || SUPPORTS(15) || SUPPORTS(16)
				|| SUPPORTS(17) || SUPPORTS(18) || SUPPORTS(19));
		break;
	case TYPE_STRING:
		s = (SUPPORTS(4) || SUPPORTS(5) || SUPPORTS(6) || SUPPORTS(7)
				|| SUPPORTS(8) || SUPPORTS(9) || SUPPORTS(10) || SUPPORTS(11)
				|| SUPPORTS(18) || SUPPORTS(19));
		break;
	case TYPE_USER_FUNCTION:
		s =
				(SUPPORTS(4) || SUPPORTS(5) || SUPPORTS(6) || SUPPORTS(7)
						|| SUPPORTS(10) || SUPPORTS(11) || SUPPORTS(18)
						|| SUPPORTS(19));
		break;
	case TYPE_OBJECT:
		s = (object_lookup(value, symbol) != NOT_FOUND)
				|| supps(object_lookup(value, core_symbol), symbol);
		break;
	default:
		s = false;
		break;
	}
	return s;
}

memptr func_supps(memptr func_expr) {

	if (num_nodes(func_expr) != 3) {
#ifdef DEBUG
		printf("36\n");
#endif
		ERROR(ERROR_SUPPS, ERROR_FAILURE, ERROR_INVALID, ERROR_COUNT);
		return NOT_FOUND;
	}

	memptr symbol = cons_pool[cons_pool[func_expr].cdr].car;
	memptr value = resolve_expr(
			cons_pool[cons_pool[cons_pool[func_expr].cdr].cdr].car);
	if (value == NOT_FOUND) {
#ifdef DEBUG
		printf("37\n");
#endif
		ERROR(ERROR_SUPPS, ERROR_FAILURE, ERROR_INVALID, ERROR_EMPTY);
		return NOT_FOUND;
	}

	return (supps(value, symbol) ? t : nil);
}
