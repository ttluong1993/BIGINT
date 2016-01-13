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
	BIGINT a, n, m, p;
	if ((test = (char*)malloc(256*sizeof(char))) == NULL) {
		printf("error in allocate memory for test\n");
	}
	BI_init_set_ui(&a, 229);
	BI_init_set_ui(&n, 500);
	BI_init_set_ui(&m, 4567);
	BI_init(&p);
	
	if (BI_divm(p, a, n, m)) {
		r = BI_get_ui(p);
		printf("a div n (mod m) = %ju\n", r);
	} else printf("div is not exist.\n");
	BI_clear(&a);
	BI_clear(&m);
	BI_clear(&n);
	BI_clear(&p);

	free(test);
	return 0;
}