#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "bigint.h"

/* Inverse a string
 * Input: char* ;	Return: char*
 */
char* inv_str(const char *str) {
	int i=0, j;
	char *out;
	out = (char*)malloc(256*sizeof(char));
	while (*(str+i)) {
		i++;
	}
	j=i;
	while (i>=0) {
		*(out+j-i) = *(str+i-1);
		i--;
	}
	return out;
	free(out);
}

//compute size in bits.
u8 compute_size(BIGINT rop) {
	int i;
	u8 val;
	rop->size = 0;
	for (i=0; i<=MAXSIZE; i++) {
		if ((rop->value[i]) > 0) {
			rop->size = (MAXSIZE-i)*8*sizeof(u8);
			val = rop->value[i];
			while (val >= 1) {
				rop->size = rop->size + 1;
				val = val/2;
			}
			break;
		}
	}
	if (rop->size == 0) rop->size = 1;
	return (rop->size);
}

/* Initialize a big integer before using 
 * 
 * Allocate a memory space for the variable x
 * 
 */
void BI_init(BIGINT *x) {
	if ((*x = (struct bigint_st *)malloc(sizeof(struct bigint_st))) == NULL) {
		printf("error in BI_init\n");
	}
}

/* Release a big integer after using */
void BI_clear(BIGINT *x) {
	if (*x != NULL) {
		free(*x);
	}	
}

/* Print out a big number to FILE */
void BI_print(FILE* out, u8 base, const BIGINT x) {
	char *strout;
	strout = (char*)malloc(sizeof(char));
	BI_get_str(strout, base, x);
	if (strout != NULL) {
		fprintf(out, "%s", strout);
	}
	free(strout);
}

/* Assignment functions. 
 * 
 * Assuming that big integer variables were allocated a memory space 
 * 
 * Set the value of rop from op
 */
void BI_set(BIGINT rop, const BIGINT op) {
	int i;
	for (i=0; i<=MAXSIZE; i++) {
		rop->value[i] = op->value[i];
	}
	rop->signbit = op->signbit;
	rop->size = op->size;
}

void BI_set_ui(BIGINT rop, const u64 op) {
	int i;
	u64 quot = op;
	for (i=MAXSIZE; i>=0; i--) {
		rop->value[i] = (uint8_t)quot%MAXVALUE;
		quot = quot/MAXVALUE;
	}
	compute_size(rop);
	rop->signbit = 1;
}

void BI_set_si(BIGINT rop, const int64_t op) {
	u64 unsign_op;
	if (op >= 0) {
		BI_set_ui(rop, op);
	} else {
		unsign_op = -op;
		BI_set_ui(rop, unsign_op);
		rop->signbit = -1;
	}
}

int BI_set_str(BIGINT rop, const char* str, int base) {
	int i=0, curValue;
	BIGINT op1;
	BI_set_ui(rop, 0);
	BI_init(&op1);
	while (1) {
		if ((str[i] >= 'a')&(str[i] <= 'z')) {
			curValue = str[i] - 'a' + 10;
		} else if ((str[i] >= 'A')&(str[i] <= 'Z')) {
			curValue = str[i] - 'A' + 10;
		} else if ((str[i] >= '0')&(str[i] <= '9')) {
			curValue = str[i] - '0';
		} else {
			return 0;
			break;
		}
		i++;
		if (curValue < base) {
			BI_mul_ui(op1, rop, base);
			BI_add_ui(rop, op1, curValue);
		} else {
			i = -1;
			break;
		}
		
	}
	rop->size = compute_size(rop);
	rop->signbit = 1;
	BI_clear(&op1);
	return (i>=0);
}

// Set rop = 2^n
void BI_set_pow_2(BIGINT rop, u8 n) {
	BI_set_ui(rop, 0);
	rop->value[MAXSIZE-n/8] = (uint8_t)pow(2,(n%8));
	rop->size = compute_size(rop);
	rop->signbit = 1;
}

/* Swap the values x and y */
void BI_swap(BIGINT x, BIGINT y) {
	int i;
	BIGINT tmp;
	BI_init(&tmp);
	for (i=0; i<MAXSIZE; i++) {
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
	BI_clear(&tmp);
}

/* Combined Initialization and Assignment Functions
 *  
 * Initialize rop with limb space and set the initial numeric value from op.
 */
void BI_init_set(BIGINT *rop, const BIGINT op) {
	BI_init(rop);
	BI_set(*rop, op);
}

void BI_init_set_ui(BIGINT *rop, u64 op) {
	BI_init(rop);
	BI_set_ui(*rop, op);
}

void BI_init_set_si(BIGINT *rop, int64_t op) {
	BI_init(rop);
	BI_set_si(*rop, op);
}

int BI_init_set_str(BIGINT *rop, const char *str, int base) {
	BI_init(rop);
	BI_set_str(*rop, str, base);
}

/* Conversion functions */

/* Return the value of op as an 64-bit unsigned integer.
 * If op is too big to fit an unsigned long then just the least significant bits that do fit are returned
 */
u64 BI_get_ui(const BIGINT op) {
	int i;
	u64 num=0;
	if (BI_sgn(op) != 0) {
		for (i=0; i<=MAXSIZE; i++) {
			num = num*MAXVALUE + op->value[i];
		}
	}
	return num;
}
/* If op fits into a signed long int return the value of op. 
 * Otherwise return the least significant part of op, with the same sign as op.
 */
int64_t BI_get_si(const BIGINT op) {
	int64_t signed_op;
	signed_op = BI_get_ui(op);
	if (op->signbit >= 0) {
		return signed_op;
	} else return -signed_op;
}
/* Convert op to a string of digits in base base */
char* BI_get_str(char *str, int base, const BIGINT op) {
	int i;
	u64 irem;
	BIGINT quot, rem;
	BI_init_set(&quot, op);
	BI_init(&rem);
	i = 0;
	do {
		irem = BI_mod_ui(rem, quot, base);
		str[i] = strSpace[irem];
		i++;
		BI_div_ui(rem, quot, base);
		BI_set(quot, rem);
	} while (BI_sgn(quot)!=0);
	BI_clear(&rem);
	BI_clear(&quot);
	str = inv_str(str);
	return str;
}

/* Random Number Functions */

/* Generate a uniformly distributed random integer in the range 0 to 2^n-1, inclusive. */
void BI_randb(BIGINT rop, u8 n) {
	int i;
	BIGINT max;
	BI_init(&max);
	BI_set_pow_2(max, n);
	BI_randm(rop, max);
	BI_clear(&max);
}

/* Generate a uniformly distributed random integer in the range 0 to n-1, inclusive. */
void BI_randm(BIGINT rop, const BIGINT n) {
	int i, randSize;
	srand(time(NULL));
	randSize = rand()%MAXSIZE + 1;
	for(i=0; i<=MAXSIZE; i++) {
		if (i<randSize) {
			rop->value[MAXSIZE-i] = (uint8_t)(rand()%256);
		} else {
			rop->value[MAXSIZE-i] = 0;
		}
	}
	rop->signbit = 1;
	rop->size = compute_size(rop);
	BI_mod(rop, rop, n);
}

/* Comparison Functions */

/* Compare op1 and op2. 
 * 
 * Return a positive value if op1 > op2, zero if op1 = op2, or a negative value if op1 < op2. 
 * 
 */
int BI_cmp(const BIGINT op1, const BIGINT op2) {
	int i=MAXSIZE;
	if ((op1->signbit) > (op2->signbit)) {
		return 1;
	} else if ((op1->signbit) < (op2->signbit)) {
		return -1;
	} else if (op1->signbit = 1) {
		return BI_cmpabs(op1, op2);
	} else return -BI_cmpabs(op1, op2);
}

int BI_cmp_si(const BIGINT op1, int64_t op2) {
	BIGINT big_op2;
	BI_init_set_si(&big_op2, op2);
	return BI_cmp(op1, big_op2);
	BI_clear(&big_op2);
}

int BI_cmp_ui(const BIGINT op1, u64 op2) {
	BIGINT big_op2;
	BI_init_set_ui(&big_op2, op2);
	return BI_cmp(op1, big_op2);
	BI_clear(&big_op2);
}

/* Compare the absolute values of op1 and op2. 
 * Return a positive value if abs(op1) > abs(op2), zero if abs(op1) = abs(op2), or a negative value if abs(op1) < abs(op2).
 */
int BI_cmpabs(const BIGINT op1, const BIGINT op2) {
	int i=0, sgn;
	if ((op1->size) > (op2->size)) {
		return 1;
	} else if ((op1->size) < (op2->size)) {
		return -1;
	} else {
		while (i<=MAXSIZE) {
			if ((op1->value[i]) > (op2->value[i])) {
				sgn = 1;
				break;
			} else if ((op1->value[i]) < (op2->value[i])) {
				sgn = -1;
				break;
			} else sgn = 0;
			i++;
		}
		return sgn;
	}
}

int BI_cmpabs_ui(const BIGINT op1, u64 op2) {
	BIGINT big_op2;
	BI_init_set_ui(&big_op2, op2);
	return BI_cmpabs(op1, big_op2);
	BI_clear(&big_op2);
}

/* Return +1 if op > 0, 0 if op = 0, and -1 if op < 0. */
int BI_sgn(const BIGINT op) {
	int i=0, result;
	while (i<=MAXSIZE) {
		if ((op->value[i]) != 0) {
			result = op->signbit;
			break;
		}
		i++;
		result = 0;
	}
	return result;
}

/********************************************************************************************/
/*									End of Luong'working									*/
/********************************************************************************************/


/*
 * Logical and Bit Manipulation Functions
 */
void BI_and(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	/* Set rop to op1 bitwise-and op2. */
	int i;
	for (i=0; i<=MAXSIZE; i++) {
		if ((op1->value[i] != 0)&&(op2->value[i] != 0)) {
			rop->value[i] = op1->value[i] & op2->value[i];
		} else rop->value[i] = 0;
	}
	if (op1->size > op2->size) rop->size = op1->size;
	else rop->size = op2->size;
	rop->signbit = 1;
}

void BI_ior(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	/* Set rop to op1 bitwise inclusive-or op2. */
	int i;
	for (i=0; i<=MAXSIZE; i++) {
		rop->value[i] = op1->value[i] || op2->value[i];
	}
	if (op1->size > op2->size) rop->size = op1->size;
	else rop->size = op2->size;
	rop->signbit = 1;
}

void BI_xor(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	/* Set rop to op1 bitwise exclusive-or op2. */
	int i;
	for (i=0; i<=MAXSIZE; i++) {
		rop->value[i] = op1->value[i] ^ op2->value[i];
	}
	if (op1->size > op2->size) rop->size = op1->size;
	else rop->size = op2->size;
	rop->signbit = 1;
}

/* Set rop to the one's complement of op. */
void BI_com(BIGINT rop, const BIGINT op) {
	u8 size_bit;
	BIGINT xor_num;
	BI_init(&xor_num);
	size_bit = op->size;
	BI_set_pow_2(rop, size_bit);
	BI_sub_ui(xor_num, rop, 1);
	BI_xor(rop, xor_num, op);
	BI_clear(&xor_num);
}

/* Count the number of 1 bits in the binary representation. */
u8 BI_Hammingweight(const BIGINT op) {
	int i;
	u8 val, result=0;
	for (i=0; i<=MAXSIZE; i++) {
		val = op->value[i];
		while (val > 0) {
			if ((val % 2) == 1) {
				result++;
			}
			val = val/2;
		}
	}
	return val;
}

/* Return the hamming distance between the two operands, 
	 * which is the number of bit positions where op1 and op2 have different bit values
	 */
u8 BI_hamdist(const BIGINT op1, const BIGINT op2) {
	u8 result=0;
	BIGINT xor_num;
	if ((xor_num = (struct bigint_st *)malloc(sizeof(struct bigint_st))) == NULL) {
		printf("error in allocate memory for xor_num\n");
	}
	BI_xor(xor_num, op1, op2);
	result = BI_Hammingweight(xor_num);
	free(xor_num);
	return result;
}
													
/* Scan op, starting from bit starting_bit, towards more significant bits, until the first 0 or 1 bit (respectively) is found. 
 * 
 * Return the index of the found bit. 
 * 
 * If the bit at starting_bit is already what's sought, then starting_bit is returned.
 * 
 * If there's no bit found, then the largest possible u8 is returned.
 * 
 */
u8 BI_scan0(const BIGINT op, u8 starting_bit) {
	u8 bit_index;
	for (bit_index = starting_bit; bit_index < op->size; bit_index++) {
		if (BI_tstbit(op, bit_index) == 0) {
			return bit_index;
			break;
		}
	}
}

u8 BI_scan1(const BIGINT op, u8 starting_bit) {
	u8 bit_index;
	for (bit_index = starting_bit; bit_index < op->size; bit_index++) {
		if (BI_tstbit(op, bit_index) == 1) {
			return bit_index;
			break;
		}
	}
}

/* Set bit bit_index in rop. */
void BI_setbit(BIGINT rop, u8 bit_index) {
	u8 or_val;
	or_val = pow(2, (bit_index % 8));
	rop->value[MAXSIZE - bit_index/8] = or_val | rop->value[MAXSIZE - bit_index/8];
}

/* Clear bit bit_index in rop. */
void BI_clrbit(BIGINT rop, u8 bit_index) {
	u8 and_val;
	and_val = 255 - pow(2, (bit_index % 8));
	rop->value[MAXSIZE - bit_index/8] = and_val & rop->value[MAXSIZE - bit_index/8];
}

/* Complement bit bit_index in rop. */
void BI_combit(BIGINT rop, u8 bit_index) {
	u8 xor_val;
	xor_val = pow(2, (bit_index % 8));
	rop->value[MAXSIZE - bit_index/8] = xor_val ^ rop->value[MAXSIZE - bit_index/8];
}

/* Test bit bit_index in op and return 0 or 1 accordingly. */
int BI_tstbit(const BIGINT op, u8 bit_index) {
	u8 val;
	val = op->value[MAXSIZE - bit_index/8]/pow(2, (bit_index % 8));
	if (val % 2 == 1) return 1;
	else return 0;
}

/* Determine whether op is odd or even, respectively. 
 * Return non-zero if yes, zero if no. 
 */  
int BI_odd_p(const BIGINT op) {
	if (((op->value[MAXSIZE]) % 2) != 0) return 1;
	else return 0;
}

int BI_even_p(const BIGINT op) {
	if (((op->value[MAXSIZE]) % 2) == 0) return 1;
	else return 0;
}

/* Return the size of op measured in number of digits in the given base. 
 * The sign of op is ignored, just the absolute value is used. 
 * The result will be either exact or 1 too big. If base is a power of 2, the result is always exact. If op is zero the return value is always 1.
 */
u8 BI_sizeinbase(const BIGINT op, int base) {
	u8 result=0;
	BIGINT n, m;
	BI_init(&n);
	BI_init_set_ui(&m, 1);
	while (1) {
		result++;
		BI_mul_ui(n, m, base);
		if (BI_cmpabs(n, op) >= 0) break;
	}
	BI_clear(&n);
	BI_clear(&m);
	return result;
}

/* 
 * Arithmetic Functions 
 */

/* Set rop = op1 + op2 */
void BI_add(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	int i, sum, quot=0;
	if (op1->signbit == op2->signbit) {
		for (i=MAXSIZE; i>=0; i--) {
			sum = (op1->value[i] + op2->value[i] + quot)%MAXVALUE;
			quot = (op1->value[i] + op2->value[i] + quot)/MAXVALUE;
			rop->value[i] = (uint8_t)sum;
		}
		rop->signbit = op1->signbit;
	} else if (BI_cmpabs(op1, op2) >= 0) {
		for (i=MAXSIZE; i>=0; i--) {
			sum = op1->value[i] - op2->value[i] + quot;
			if (sum >= 0) quot = 0;
			else {
				sum += MAXVALUE;
				quot = -1;
			}
			rop->value[i] = (uint8_t)sum;
		}
		rop->signbit = op1->signbit;
	} else {
		for (i=MAXSIZE; i>=0; i--) {
			sum = op2->value[i] - op1->value[i] + quot;
			if (sum >= 0) quot = 0;
			else {
				sum += MAXVALUE;
				quot = -1;
			}
			rop->value[i] = (uint8_t)sum;
		}
		rop->signbit = op2->signbit;
	}
	rop->size = compute_size(rop);
}

void BI_add_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	BIGINT big_op2;
	BI_init_set_ui(&big_op2, op2);
	BI_add(rop, op1, big_op2);
	BI_clear(&big_op2);
}

/* Set rop = op1 - op2 */
void BI_sub(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	BIGINT neg_op2;
	BI_init(&neg_op2);
	BI_neg(neg_op2, op2);
	BI_add(rop, op1, neg_op2);
	BI_clear(&neg_op2);
}

void BI_sub_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	BIGINT big_op2;
	BI_init_set_ui(&big_op2, op2);
	BI_sub(rop, op1, big_op2);
	BI_clear(&big_op2);
}

void BI_ui_sub(BIGINT rop, u64 op1, const BIGINT op2) {
	BIGINT big_op1;
	BI_init_set_ui(&big_op1, op1);
	BI_sub(rop, big_op1, op2);
	BI_clear(&big_op1);
}

/* Set rop = op1 x op2. */
void BI_mul(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	int i, j, rem, quot=0;
	u8 shift_bit;
	BIGINT tmp1, tmp2;
	BI_init(&tmp1);
	BI_init(&tmp2);
	BI_set_ui(rop, 0);
	for (i=MAXSIZE; i>=0; i--) {
		if (op2->value[i] != 0) {
			shift_bit = 8 * (MAXSIZE - i);
			BI_shift(tmp1, op1, shift_bit);
			for (j=MAXSIZE; j>=0; j--) {
				rem = (tmp1->value[j] * op2->value[i] + quot) % MAXVALUE;
				quot = (tmp1->value[j] * op2->value[i] + quot) / MAXVALUE;
				tmp2->value[j] = (uint8_t)rem;
			}
			tmp2->signbit = 1;
			tmp2->size = compute_size(tmp2);
			BI_add(tmp1, rop, tmp2);
			BI_set(rop, tmp1);
		}
		
	}
	if (BI_sgn(rop)!=0) rop->signbit = op1->signbit * op2->signbit;
	BI_clear(&tmp1);
	BI_clear(&tmp2);
}

void BI_mul_si(BIGINT rop, const BIGINT op1, int64_t op2) {
	BIGINT big_op2;
	BI_init_set_si(&big_op2, op2);
	BI_mul(rop, op1, big_op2);
	BI_clear(&big_op2);
}

void BI_mul_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	BIGINT big_op2;
	BI_init_set_ui(&big_op2, op2);
	BI_mul(rop, op1, big_op2);
	BI_clear(&big_op2);
}

void BI_mulm(BIGINT rop, const BIGINT op1, const BIGINT op2, const BIGINT m) {
	int i, j, rem, quot=0;
	u8 shift_bit;
	BIGINT tmp1, tmp2;
	BI_init(&tmp1);
	BI_init(&tmp2);
	BI_set_ui(rop, 0);
	for (i=MAXSIZE; i>=0; i--) {
		if (op2->value[i] != 0) {
			BI_set(tmp1, op1);
			shift_bit = 8 * (MAXSIZE - i);
			while (shift_bit > 0) {
				if ((shift_bit + tmp2->size) <= 256) {
					BI_shift(tmp2, tmp1, shift_bit);
					shift_bit = 0;
				} else {
					BI_shift(tmp2, tmp1, 256 - shift_bit);
					shift_bit = 256 - shift_bit;
					shift_bit = tmp2->size - shift_bit;
				}
				BI_mod(tmp1, tmp2, m);
			}
			for (j=MAXSIZE; j>=0; j--) {
				rem = (tmp1->value[j] * op2->value[i] + quot) % MAXVALUE;
				quot = (tmp1->value[j] * op2->value[i] + quot) / MAXVALUE;
				tmp2->value[j] = (uint8_t)rem;
			}
			tmp2->signbit = 1;
			tmp2->size = compute_size(tmp2);
			BI_add(tmp1, rop, tmp2);
			BI_mod(rop, tmp1, m);
		}
		
	}
	if (BI_sgn(rop)!=0) rop->signbit = op1->signbit * op2->signbit;
	BI_clear(&tmp1);
	BI_clear(&tmp2);
}

/* Set rop = rop + op1 x op2 */
void BI_addmul(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	BIGINT tmp1, tmp2;
	BI_init(&tmp1);
	BI_mul(tmp1, op1, op2);
	BI_init_set(&tmp2, rop);
	BI_add(rop, tmp1, tmp2);
	BI_clear(&tmp1);
	BI_clear(&tmp2);
}

void BI_addmul_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	BIGINT tmp1, tmp2, big_op2;
	BI_init(&tmp1);
	BI_init_set_ui(&big_op2, op2);
	BI_mul(tmp1, op1, big_op2);
	BI_init_set(&tmp2, rop);
	BI_add(rop, tmp1, tmp2);
	BI_clear(&tmp1);
	BI_clear(&tmp2);
}

/* Set rop to rop - op1 times op2 */
void BI_submul(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	BIGINT tmp1, tmp2;
	BI_init(&tmp1);
	BI_mul(tmp1, op1, op2);
	BI_init_set(&tmp2, rop);
	BI_sub(rop, tmp2, tmp1);
	BI_clear(&tmp1);
	BI_clear(&tmp2);
}

void BI_submul_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	BIGINT tmp1, tmp2, big_op2;
	BI_init(&tmp1);
	BI_init_set_ui(&big_op2, op2);
	BI_mul(tmp1, op1, big_op2);
	BI_init_set(&tmp2, rop);
	BI_sub(rop, tmp2, tmp1);
	BI_clear(&tmp1);
	BI_clear(&tmp2);
}

/* Set rop = op1 x 2 ^ op2. This operation can also be defined as a left shift by op2 bits */
void BI_shift(BIGINT rop, const BIGINT op1, u8 op2) {
	int i, n, k, rem=0;
	n = op2 / 8;
	k = op2 % 8;
	k = pow(2, k);
	for (i=MAXSIZE; i>=0; i--) {
		if ((MAXSIZE-i) < n) {
			rop->value[i] = 0;
		} else {
			rop->value[i] = (uint8_t)((k * op1->value[i+n] + rem) % MAXVALUE);
			rem = (k * op1->value[i+n] + rem) / MAXVALUE;
		}
	}
	rop->signbit = op1->signbit;
	rop->size = op1->size + op2;
}

/* Set rop = op1 / op2. Division is undefined if the divisor is zero */
void BI_div(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	u8 shift_bit;
	BIGINT uop1, uop2, tmp1, tmp2, tmp3;
	BI_init(&uop1);
	BI_init(&uop2);
	BI_init(&tmp1);
	BI_init(&tmp2);
	BI_init(&tmp3);
	BI_set_ui(rop, 0);
	BI_abs(uop1, op1);
	BI_abs(uop2, op2);
	if (BI_sgn(op2) == 0) printf("Modulo is undefined.\n");
	else {
		while (BI_cmpabs(uop1, uop2) >= 0) {
			shift_bit = uop1->size - uop2->size;
			BI_shift(tmp1, uop2, shift_bit);
			if (BI_cmpabs(uop1, tmp1) < 0) {
				shift_bit -=1;
				BI_shift(tmp1, uop2, shift_bit);
			}
			BI_sub(tmp2, uop1, tmp1);
			BI_set(uop1, tmp2);
			BI_set(tmp2, rop);
			BI_set_pow_2(tmp3, shift_bit);
			BI_add(rop, tmp2, tmp3);
		}
	}
	rop->signbit = op1->signbit * op2->signbit;
	BI_clear(&uop1);
	BI_clear(&uop2);
	BI_clear(&tmp1);
	BI_clear(&tmp2);
	BI_clear(&tmp3);
}

void BI_div_si(BIGINT rop, const BIGINT op1, int64_t op2) {
	BIGINT big_op2;
	BI_init_set_si(&big_op2, op2);
	BI_div(rop, op1, big_op2);
	BI_clear(&big_op2);
}

void BI_div_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	BIGINT big_op2;
	BI_init_set_ui(&big_op2, op2);
	BI_div(rop, op1, big_op2);
	BI_clear(&big_op2);
}

void BI_divm(BIGINT rop, const BIGINT op1, const BIGINT op2, const BIGINT m) {
	//
}

/* Set rop to -op */
void BI_neg(BIGINT rop, const BIGINT op) {
	BI_set(rop, op);
	rop->signbit = -(op->signbit);
}

/* Set rop to the absolute value of op */
void BI_abs(BIGINT rop, const BIGINT op) {
	BI_set(rop, op);
	rop->signbit = 1;
}


/* Modulo arithmetic functions */

/* Set rop = d mod n. The sign of the divisor is ignored; the result is always non-negative. */
void BI_mod(BIGINT rop, const BIGINT d, const BIGINT n) {
	u8 shift_bit;
	BIGINT tmp1, tmp2, tmp3;
	BI_init(&tmp3);
	BI_init_set(&tmp1, d);
	BI_init_set(&tmp2, n);
	tmp1->signbit = 1;
	tmp2->signbit = 1;
	if (BI_sgn(n) == 0) printf("Modulo is undefined.\n");
	else {
		if (BI_cmpabs(d, n) >= 0) {
			while (BI_cmpabs(tmp1, n) >= 0) {
				shift_bit = tmp1->size - tmp2->size;
				BI_shift(tmp3, tmp2, shift_bit);
				if (BI_cmpabs(tmp1, tmp3) < 0) {
					shift_bit -=1;
					BI_shift(tmp3, tmp2, shift_bit);
				}
				BI_sub(rop, tmp1, tmp3);
				BI_set(tmp1, rop);
			}
		} else BI_set(rop, d);
	}
	BI_clear(&tmp1);
	BI_clear(&tmp2);
	BI_clear(&tmp3);
}

u64 BI_mod_ui(BIGINT rop, const BIGINT d, u64 n) {
	u64 result;
	BIGINT big_n;
	BI_init_set_ui(&big_n, n);
	BI_mod(rop, d, big_n);
	result = BI_get_ui(rop);
	BI_clear(&big_n);
	return result;
}

/* Return non-zero if c is congruent to d modulo n */
int BI_congruent_p(const BIGINT c, const BIGINT d, const BIGINT n) {
	BIGINT dif, rem;
	BI_init(&dif);
	BI_init(&rem);
	BI_sub(dif, c, d);
	BI_mod(rem, dif, n);
	if (BI_sgn(rem) == 0) return 1;
	else return 0;
	BI_clear(&dif);
	BI_clear(&rem);
}

int BI_congruent_ui_p(const BIGINT c, u64 d, u64 n) {
	BIGINT big_d, big_n;
	BI_init_set_ui(&big_d, d);
	BI_init_set_ui(&big_n, n);
	return BI_congruent_p(c, big_d, big_n);
	BI_clear(&big_d);
	BI_clear(&big_n);
}

/* Set rop to (base raised to exp) modulo mod. */
void BI_powm(BIGINT rop, const BIGINT base, const BIGINT exp, const BIGINT mod) {
	int i;
	u8 bit_index;
	BIGINT op;
	BI_init(&op);
	BI_mod(rop, base, mod);
	bit_index = exp->size - 1;
	for (i=bit_index; i>=0; i--) {
		BI_mulm(op, rop, base, mod);
		BI_set(rop, op);
		if (BI_tstbit(exp, i) == 1) {
			BI_mulm(op, rop, base, mod);
			BI_set(rop, op);
		}
	}
	BI_clear(&op);
}

void BI_powm_ui (BIGINT rop, const BIGINT base, u64 exp, const BIGINT mod) {
	BIGINT big_exp;
	BI_init_set_ui(&big_exp, exp);
	BI_powm(rop, base, big_exp, mod);
	BI_clear(&big_exp);
}
