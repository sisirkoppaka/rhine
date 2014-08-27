#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

struct value_t {
	int type_tag;
	long int_val;
	bool bool_val;
	char *string_val;
	struct value_t **array_val;
	long array_len;
	double dbl_val;
	struct value_t *(*function_val)(struct value_t *);
	char char_val;
};

void print_atom(struct value_t *v) {
	int i;
	if (!v) {
		printf("(nil)");
		return;
	}
	switch(v->type_tag) {
	case 1:
		printf("(int) %ld", v->int_val);
		break;
	case 2:
		printf("(bool) %s", !v->bool_val ? "false" : "true");
		break;
	case 3:
		printf("(string len %ld) %s", v->array_len, v->string_val);
		break;
	case 4:
		printf("(array len %ld) [", v->array_len);
		for (i = 0; i < v->array_len; i++) {
			struct value_t *el = (v->array_val)[i];
			print_atom(el);
			printf(";");
		}
		printf("]");
		break;
	case 6:
		printf("(double) %f", v->dbl_val);
		break;
	case 8:
		printf("(char) %c", v->char_val);
		break;
	default:
		printf("Don't know how to print type %d", v->type_tag);
	}
}

extern struct value_t *print(int nargs, ...) {
	struct value_t *ret;
	va_list ap;
	va_start(ap, nargs);
	struct value_t *v = va_arg(ap, struct value_t *);
	print_atom(v);
	va_end(ap);
	ret = malloc(sizeof(struct value_t));
	ret->int_val = 0;
	ret->type_tag = 1;
	return ret;
}

extern struct value_t *println(int nargs, ...) {
	struct value_t *ret;
	va_list ap;
	va_start(ap, nargs);
	struct value_t *v = va_arg(ap, struct value_t *);
	print_atom(v);
	printf("\n");
	va_end(ap);
	ret = malloc(sizeof(struct value_t));
	ret->int_val = 0;
	ret->type_tag = 1;
	return ret;
}

struct value_t *save_value(double val, int ret_type) {
	struct value_t *ret;
	ret = malloc(sizeof(struct value_t));

	if (ret_type == 6) {
		ret->dbl_val = val;
		ret->type_tag = 6;
	} else if (ret_type == 2) {
		ret->bool_val = (bool) val;
		ret->type_tag = 2;
	} else {
		ret->int_val = (int) val;
		ret->type_tag = 1;
	}
	return ret;
}

extern struct value_t *cequ(int nargs, ...) {
	int ret_type = 0;
	struct value_t *ret;
	va_list ap;
	va_start(ap, nargs);
	struct value_t *v = va_arg(ap, struct value_t *);
	struct value_t *v2 = va_arg(ap, struct value_t *);
	va_end(ap);

	// only makes sense for integers and bools currently
	switch(v->type_tag) {
	case 1:
		if (v->int_val == v2->int_val) {
			ret = save_value(1.0, 2);
		} else {
			ret = save_value(0.0, 2);
		}
		break;
	case 2:
		if (!v->bool_val == !v2->bool_val) {
			ret = save_value(1.0, 2);
		} else {
			ret = save_value(0.0, 2);
		}
		break;
	case 4:
		if (v->array_len == v2->array_len) {
			ret = save_value(1.0, 2);
		} else {
			ret = save_value(0.0, 2);
		}
		break;
	case 3:
		if (v->array_len == v2->array_len && memcmp(v->array_val, v2->array_val, sizeof(char)*v->array_len) == 0) {
			ret = save_value(1.0, 2);
		} else {
			ret = save_value(0.0, 2);
		}
		break;
	}
	return ret;
}

extern struct value_t *cstrjoin(int nargs, ...) {
	struct value_t *ret;
	va_list ap;
	va_start(ap, nargs);
	struct value_t *v = va_arg(ap, struct value_t *);
	va_end(ap);
	int i = 0;
	ret = malloc(sizeof(struct value_t));
	ret->string_val = malloc(sizeof(char) * (v->array_len+1));
	ret->type_tag = 3;
	ret->array_len = v->array_len;
	while (i < v->array_len) {
		ret->string_val[i] = (v->array_val)[i]->char_val;
		i++;
	}
	*(ret->string_val+i) = '\0';

	return ret;
}
