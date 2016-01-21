/*
   Test case for bigint.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigint.h"

int main() {
	//do testing here
	int i;
	u8 k=64;
	u64 r;
	clock_t begin, end;
	double xtime;
	char *test;
	BIGINT a, b, c, m;
	FILE *p;
	if ((test = (char*)malloc(265*sizeof(char))) == NULL) {
		printf("error in allocate memory for test\n");
	}

	BI_init(&a);
	BI_init(&b);
	BI_init(&c);
	BI_init(&m);
	//Result is recorded in result.txt
	if(!(p = fopen("result.txt", "w"))) printf("Can't open file \"result.txt\"\n");

	//random numbers
	srand(time(NULL));

	BI_randb(a, 256);
	test = BI_get_str(test, 10, a);
	printf("Random number a = %s\n", test);
	fprintf(p, "Random number a = %s\n", test);
	BI_randb(b, 256);
	test = BI_get_str(test, 10, b);
	printf("Random number b = %s\n", test);
	fprintf(p, "Random number b = %s\n", test);
	
	BI_randm(m, a);
	test = BI_get_str(test, 10, m);
	printf("Random number m = %s\n", test);
	fprintf(p, "Random number m = %s\n", test);

	//test BI_cmp
	if (BI_cmp(a, b)) {
		printf("a > b\n");
		fprintf(p, "a > b\n");
	} else {
		printf("a < b\n");
		fprintf(p, "a < b\n");
	}

	//test BI_sgn
	BI_set_ui(c, 0);
	printf("c = 0, BI_sgn(c) = %d\n", BI_sgn(c));
	fprintf(p, "c = 0, BI_sgn(c) = %d\n", BI_sgn(c));
	BI_set(c, a);
	printf("c = a, BI_sgn(c) = %d\n", BI_sgn(c));
	fprintf(p, "c = a, BI_sgn(c) = %d\n", BI_sgn(c));
	BI_neg(c, b);
	printf("c = -b, BI_sgn(c) = %d\n", BI_sgn(c));
	fprintf(p, "c = -b, BI_sgn(c) = %d\n", BI_sgn(c));

	//test BI_add
	BI_add(c, a, b);
	test = BI_get_str(test, 10, c);
	printf("The number c = a + b = %s\n", test);
	fprintf(p, "The number c = a + b = %s\n", test);

	//test BI_sub
	BI_sub(c, a, b);
	test = BI_get_str(test, 10, c);
	printf("The number c = a - b = %s\n", test);
	fprintf(p, "The number c = a - b = %s\n", test);

	//test BI_mul
	BI_mul(c, a, b);
	test = BI_get_str(test, 10, c);
	printf("The number c = a x b = %s\n", test);
	fprintf(p, "The number c = a x b = %s\n", test);

	//test BI_div
	if (BI_sgn(b) != 0) {
		BI_div(c, a, b);
		test = BI_get_str(test, 10, c);
		printf("The number c = a / b = %s\n", test);
		fprintf(p, "The number c = a / b = %s\n", test);
	} else {
		printf("a / b is undefine.\n");
		fprintf(p, "a / b is undefine.\n");
	}

	//test BI_mod
	BI_mod(c, a, m);
	test = BI_get_str(test, 10, c);
	printf("The number c = a mod m = %s\n", test);
	fprintf(p, "The number c = a mod m = %s\n", test);

	//test BI_mulm
	BI_mulm(c, a, b, m);
	test = BI_get_str(test, 10, c);
	printf("The number c = a x b (mod m) = %s\n", test);
	fprintf(p, "The number c = a x b (mod m) = %s\n", test);

	//test BI_divm
	if (BI_divm(c, a, b, m)) {
		test = BI_get_str(test, 10, c);
		printf("The number c = a div b (mod m) = %s\n", test);
		fprintf(p, "The number c = a div b (mod m) = %s\n", test);
	} else {
		printf("a div b (mod m) is not exist.\n");
		fprintf(p, "a div b (mod m) is not exist.\n");
	}

	//test BI_powm
	begin = clock();
	BI_powm(c, a, b, m);
	end = clock();
	xtime = (double)(end - begin) / CLOCKS_PER_SEC;
	test = BI_get_str(test, 10, c);
	printf("The number c = a ^ b (mod m) = %s\n", test);
	printf("Time excuted BI_powm: %.5f\n", xtime);
	fprintf(p, "The number c = a ^ b (mod m) = %s\n", test);
	fprintf(p, "Time excuted BI_powm: %.5f\n", xtime);

	//test Logical and Bit Manipulation Functions
	//a and b in binary
	test = BI_get_str(test, 2, a);
	printf("The number a: %s\n", test);
	fprintf(p, "The number a: %s\n", test);
	test = BI_get_str(test, 2, b);
	printf("The number b: %s\n", test);
	fprintf(p, "The number b: %s\n", test);
	//bit and
	BI_and(c, a, b);
	test = BI_get_str(test, 2, c);
	printf("The number c = a and b = %s\n", test);
	fprintf(p, "The number c = a and b = %s\n", test);
	//bit inclusive or
	BI_ior(c, a, b);
	test = BI_get_str(test, 2, c);
	printf("The number c = a ior b = %s\n", test);
	fprintf(p, "The number c = a ior b = %s\n", test);
	//bit exclusive or
	BI_xor(c, a, b);
	test = BI_get_str(test, 2, c);
	printf("The number c = a xor b = %s\n", test);
	fprintf(p, "The number c = a xor b = %s\n", test);
	//bit complement
	BI_com(c, a);
	test = BI_get_str(test, 2, c);
	printf("The complement of a: %s\n", test);
	fprintf(p, "The complement of a: %s\n", test);
	//Hamming weight and distance
	printf("The Hamming weight of a: %d\n", BI_Hammingweight(a));
	fprintf(p, "The Hamming weight of a: %d\n", BI_Hammingweight(a));
	printf("The hamming distance between a and b: %d\n", BI_hamdist(a, b));
	fprintf(p, "The hamming distance between a and b: %d\n", BI_hamdist(a, b));

	//test BI_shift
	BI_shift(c, a, 20);
	test = BI_get_str(test, 2, c);
	printf("The number c = a left shift 10 bits = %s\n", test);
	fprintf(p, "The number c = a left shift 10 bits = %s\n", test);

	//test BI_gcd
	begin = clock();
	BI_gcd(c, a, b);
	end = clock();
	xtime = (double)(end - begin) / CLOCKS_PER_SEC;
	test = BI_get_str(test, 10, c);
	printf("The number c = gcd(a, b) = %s\n", test);
	fprintf(p, "The number c = gcd(a, b) = %s\n", test);
	printf("Time excuted BI_gcd: %.5f\n", xtime);
	fprintf(p, "Time excuted BI_gcd: %.5f\n", xtime);

	fclose(p);

	BI_clear(&a);
	BI_clear(&b);
	BI_clear(&c);
	BI_clear(&m);

	free(test);
	return 0;
}