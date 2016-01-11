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
	BIGINT n, m, p;
	if ((test = (char*)malloc(256*sizeof(char))) == NULL) {
		printf("error in allocate memory for test\n");
	}
	BI_init(&n);
	BI_init(&m);
	BI_init(&p);

	BI_randb(m, k);

	BI_mod_ui(n, m, 10000000);
	r = BI_get_ui(m);
	printf("the random number m: %ju\n", r);
	printf("size of m: %u\n", m->size);

	r = BI_get_ui(n);
	printf("the number n = m (mod 10.000.000) = %ju\n", r);
	printf("size of n: %u\n", n->size);

	BI_powm_ui(p, m, 1000000, n);
	test = BI_get_str(test, 16, p);
	printf("number p in hex: %s\n", test);

	BI_clear(&m);
	BI_clear(&n);
	BI_clear(&p);
	free(test);
	return 0;
}