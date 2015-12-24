/*
   Test case for bigint.h
*/
#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

int main() {
	//do testing here
	int i;
	u8 k=64;
	u64 r;
	char *test;
	BIGINT n, m;
	if ((test = (char*)malloc(sizeof(char))) == NULL) {
		printf("error in allocate memory for test\n");
	}
	if ((n = (struct bigint_st *)malloc(sizeof(struct bigint_st))) == NULL) {
		printf("error in allocate memory for n\n");
	}
	if ((m = (struct bigint_st *)malloc(sizeof(struct bigint_st))) == NULL) {
		printf("error in allocate memory for m\n");
	}
	BI_randb(m, k);
	BI_mod_ui(n, m, 10000000);
	r = BI_get_ui(m);
	printf("the random number m: %ju\n", r);
	// for (i=0; i<=MAXSIZE; i++) {
	// 	printf("%d: %u\n", i, n->value[i]);
	// }
	printf("size of m: %u\n", m->size);
	r = BI_get_ui(n);
	printf("the number n = m (mod 10.000.000) = %ju\n", r);
	printf("size of n: %u\n", n->size);
	test = BI_get_str(test, 16, n);
	printf("number n in hex: %s\n", test);
	free(m);
	free(n);
	free(test);
	return 0;
}