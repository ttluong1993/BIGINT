#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

char string[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void BI_init(BIGINT x) {
	x = (BIGINT*)malloc(sizeof(BIGINT));
}

void BI_clear(BIGINT x) {
	free(x);
}

void BI_print(FILE* out, u8 base, const BIGINT x) {
	char *strout;
	strout = (char*)malloc(sizeof(char));
	BI_get_str(strout, base, x);
	if (strout != NULL) {
		fprintf("%s\n", strout);
	}
}

void BI_set(BIGINT rop, const BIGINT op) {
	for (int i=0; i<MAXSIZE; i++) {
		rop->value[i] = op->value[i];
	}
	rop->signbit = op->signbit;
	rop->size = op->size;
}

void BI_set_ui(BIGINT rop, const u64 op) {
	u64 tmp = 1;
	for (int i=0; i<=MAXSIZE; i++) {
		rop->value[i] = (op/tmp)%(MAXVALUE);
		tmp = tmp * MAXVALUE;
	}
}

void BI_set_si(BIGINT rop, const int64_t op) {
	//
}

int BI_set_str(BIGINT rop, const char* str, int base) {
	//
}

void BI_swap(BIGINT x, BIGINT y) {
	BI_init(tmp);
	for (int i=0; i<MAXSIZE; i++) {
		tmp->value[i] = x->value[i];
		x->value[i] = y->value[i];
		y->value[i] = tmp->value[i];
	}
	tmp->signbit = x->signbit;
	x->signbit = y->signbit;
	y->signbit = tmp->signbit;
	tmp->size = x->size;
	x->size = y->size;
	y->size = tmp->size;
	BI_clear(tmp);
}

void BI_init_set(BIGINT rop, const BIGINT op) {
	BI_init(rop);
	BI_set(rop, op);
}

void BI_init_set_ui(BIGINT rop, u64 op) {
	BI_init(rop);
	BI_set_ui(rop, op);
}

void BI_init_set_si(BIGINT rop, int64_t op) {
	BI_init(rop);
	BI_set_si(rop, op);
}

int BI_init_set_str(BIGINT rop, const char *str, int base) {
	BI_init(rop);
	BI_set_str(rop, op);
}

u64 BI_get_ui(const BIGINT op) {
	u64 num=0;
	if (BI_sgn(op) != 0) {
		for (int i=0; i<MAXSIZE; i++) {
			num += (op->value[i])*MAXVALUE;
		}
	}
	return num;
}

int64_t BI_get_si(const BIGINT op) {
	//
}

char* BI_get_str(char *str, int base, const BIGINT op) {
	//
}

void BI_randb(BIGINT rop, u8 n) {
	//
}
void BI_randm(BIGINT rop, const BIGINT n) {
	//
}
