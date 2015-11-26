#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

/* Inverse a string
 * Input: char* ;	Return: char*
 */
char *inv_str(const char *str) {
	int i=0, j;
	char *out;
	out = (char*)malloc(sizeof(char));
	while (*(str+i)) {
		i++;
	}
	j=i;
	while (i>=0) {
		*(out+j-i) = *(str+i-1);
		i--;
	}
	return out;
}

/* Initialize a big integer before using 
 * 
 * Allocate a memory space for the variable x
 * 
 */
void BI_init(BIGINT x) {
	x = (BIGINT)malloc(sizeof(BIGINT));
}

/* Release a big integer after using */
void BI_clear(BIGINT x) {
	free(x);
}

/* Print out a big number to FILE */
void BI_print(FILE* out, u8 base, const BIGINT x) {
	char *strout;
	strout = (char*)malloc(sizeof(char));
	BI_get_str(strout, base, x);
	if (strout != NULL) {
		fprintf(out, "%s", strout);
	}
}

/* Assignment functions. 
 * 
 * Assuming that big integer variables were allocated a memory space 
 * 
 * Set the value of rop from op
 */
void BI_set(BIGINT rop, const BIGINT op) {
	int i;
	for (i=0; i<MAXSIZE; i++) {
		rop->value[i] = op->value[i];
	}
	rop->signbit = op->signbit;
	rop->size = op->size;
}

void BI_set_ui(BIGINT rop, const u64 op) {
	int i;
	u64 tmp = 1;
	for (i=0; i<=MAXSIZE; i++) {
		rop->value[i] = (op/tmp)%(MAXVALUE);
		tmp = tmp * MAXVALUE;
	}
	rop->size = i*MAXSIZE;
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
	int i=0, j, curValue;
	char *invStr;
	BIGINT op1, op2;
	BI_init_set_ui(op1, 1);
	BI_init(op2);
	BI_set_ui(rop, 0);
	invStr = (char*)malloc(sizeof(char));
	invStr = inv_str(str);
	while (*invStr) {
		if ((*invStr >= 'a')&(*invStr <= 'z')) {
			curValue = *invStr - 'a' + 10;
		} else if ((*invStr >= 'A')&(*invStr <= 'Z')) {
			curValue = *invStr - 'A' + 10;
		} else if ((*invStr >= '0')&(*invStr <= '9')) {
			curValue = *invStr - '0';
		}
		if (curValue < base) {
			BI_mul_ui(op1, op1, base);
			BI_mul_ui(op2, op1, curValue);
			BI_add(rop, rop, op2);
			i++; invStr++;
			return 1;
		} else {
			return 0;
			break;
		}
		
	}
	free(invStr);
	BI_clear(op1);
	BI_clear(op2);
}

/* Swap the values x and y */
void BI_swap(BIGINT x, BIGINT y) {
	int i;
	BIGINT tmp;
	BI_init(tmp);
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
	BI_clear(tmp);
}

/* Combined Initialization and Assignment Functions
 *  
 * Initialize rop with limb space and set the initial numeric value from op.
 */
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
	BI_set_str(rop, str, base);
}

/* Conversion functions */

/* Return the value of op as an 64-bit unsigned integer.
 * If op is too big to fit an unsigned long then just the least significant bits that do fit are returned
 */
u64 BI_get_ui(const BIGINT op) {
	int i;
	u64 num=0;
	if (BI_sgn(op) != 0) {
		for (i=0; i<MAXSIZE; i++) {
			num += (op->value[i])*MAXVALUE;
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
	int i=0;
	u64 irem;
	BIGINT quot, rem;
	BI_init(rem);
	BI_init_set(quot, op);
	do {
		BI_mod_ui(rem, quot, base);
		irem = BI_get_ui(rem);
		str[i] = strSpace[irem];
		i++;
		BI_div_ui(quot, quot, base);
	} while (BI_cmp_ui(quot, 1)>=0);
	BI_clear(rem);
	BI_clear(quot);
	str = inv_str(str);
	return str;
}

/* Random Number Functions */

/* Generate a uniformly distributed random integer in the range 0 to 2^n-1, inclusive. */
void BI_randb(BIGINT rop, u8 n) {
	// in working...
}
/* Generate a uniformly distributed random integer in the range 0 to n-1, inclusive. */
void BI_randm(BIGINT rop, const BIGINT n) {
	// in working...
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
	} else {
		while (i>=0) {
			if ((op1->value[i]) > (op1->value[i])) {
				if ((op1->signbit) > 0) return 1;
				else return -1;
				break;
			} else if ((op1->value[i]) < (op1->value[i])) {
				if ((op1->signbit) > 0) return -1;
				else return 1;
				break;
			}
			i--;
			return 0;
		}
	}
}

int BI_cmp_si(const BIGINT op1, int64_t op2) {
	BIGINT big_op2;
	BI_init_set_si(big_op2, op2);
	return BI_cmp(op1, big_op2);
	BI_clear(big_op2);
}

int BI_cmp_ui(const BIGINT op1, u64 op2) {
	BIGINT big_op2;
	BI_init_set_ui(big_op2, op2);
	return BI_cmp(op1, big_op2);
	BI_clear(big_op2);
}

/* Compare the absolute values of op1 and op2. 
 * Return a positive value if abs(op1) > abs(op2), zero if abs(op1) = abs(op2), or a negative value if abs(op1) < abs(op2).
 */
int BI_cmpabs(const BIGINT op1, const BIGINT op2) {
	int i=0;
	while (i>=0) {
		if ((op1->value[i]) > (op1->value[i])) {
			return 1;
			break;
		} else if ((op1->value[i]) < (op1->value[i])) {
			return -1;
			break;
		}
		i--;
		return 0;
	}
}

int BI_cmpabs_ui(const BIGINT op1, u64 op2) {
	BIGINT big_op2;
	BI_init_set_ui(big_op2, op2);
	return BI_cmpabs(op1, big_op2);
	BI_clear(big_op2);
}

/* Return +1 if op > 0, 0 if op = 0, and -1 if op < 0. */
int BI_sgn(const BIGINT op) {
	int i=0;
	while (i>=0) {
		if ((op->value[i]) != 0) {
			if ((op->signbit) > 0) {
				return 1;
				break;
			} else {
				return -1;
				break;
			}
		}
		i--;
		return 0;
	}
}

/********************************************************************************************/
/*									End of Luong'working									*/
/********************************************************************************************/


/*
 * Logical and Bit Manipulation Functions
 */
void BI_and(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	/* Set rop to op1 bitwise-and op2. */
}

void BI_ior(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	/* Set rop to op1 bitwise inclusive-or op2. */
}

void BI_xor(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	/* Set rop to op1 bitwise exclusive-or op2. */
}

void BI_com(BIGINT rop, const BIGINT op) {
	/* Set rop to the one's complement of op. */
}

u8 BI_Hammingweight(const BIGINT op) {
	/* Count the number of 1 bits in the binary representation. */
}

u8 BI_hamdist(const BIGINT op1, const BIGINT op2) {
	/* Return the hamming distance between the two operands, 
	 * which is the number of bit positions where op1 and op2 have different bit values
	 */
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

}

u8 BI_scan1(const BIGINT op, u8 starting_bit) {

}

void BI_setbit(BIGINT rop, u8 bit_index) {
	/* Set bit bit_index in rop. */
}

void BI_clrbit(BIGINT rop, u8 bit_index) {
	/* Clear bit bit_index in rop. */
}

void BI_combit(BIGINT rop, u8 bit_index) {
	/* Complement bit bit_index in rop. */
}

int BI_tstbit(const BIGINT op, u8 bit_index) {
	/* Test bit bit_index in op and return 0 or 1 accordingly. */
}

/* Determine whether op is odd or even, respectively. 
 * Return non-zero if yes, zero if no. 
 */  
int BI_odd_p(const BIGINT op) {

}

int BI_even_p(const BIGINT op) {

}

/* Return the size of op measured in number of digits in the given base. 
 * The sign of op is ignored, just the absolute value is used. 
 * The result will be either exact or 1 too big. If base is a power of 2, the result is always exact. If op is zero the return value is always 1.
 */
u8 BI_sizeinbase(const BIGINT op, int base) {

}

/* 
 * Arithmetic Functions 
 */

/* Set rop = op1 + op2 */
void BI_add(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	//
}

void BI_add_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	//
}

/* Set rop = op1 - op2 */
void BI_sub(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	//
}

void BI_sub_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	//
}

void BI_ui_sub(BIGINT rop, u64 op1, const BIGINT op2) {
	//
}

/* Set rop = op1 x op2. */
void BI_mul(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	//
}

void BI_mul_si(BIGINT rop, const BIGINT op1, int64_t op2) {
	//
}

void BI_mul_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	//
}

/* Set rop = rop + op1 x op2 */
void BI_addmul(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	//
}

void BI_addmul_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	//
}

/* Set rop to rop - op1 times op2 */
void BI_submul(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	//
}

void BI_submul_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	//
}

/* Set rop = op1 x 2 ^ op2. This operation can also be defined as a left shift by op2 bits */
void BI_shift(BIGINT rop, const BIGINT op1, u8 op2) {
	//
}

/* Set rop = op1 / op2. Division is undefined if the divisor is zero */
void BI_div(BIGINT rop, const BIGINT op1, const BIGINT op2) {
	//
}

void BI_div_si(BIGINT rop, const BIGINT op1, int64_t op2) {
	//
}

void BI_div_ui(BIGINT rop, const BIGINT op1, u64 op2) {
	//
}

/* Set rop to -op */
void BI_neg(BIGINT rop, const BIGINT op) {
	//
}

/* Set rop to the absolute value of op */
void BI_abs(BIGINT rop, const BIGINT op) {
	//
}


/* Modulo arithmetic functions */

/* Set rop = d mod n. The sign of the divisor is ignored; the result is always non-negative. */
void BI_mod(BIGINT rop, const BIGINT d, const BIGINT n) {
	//
}

u64 BI_mod_ui(BIGINT rop, const BIGINT d, u64 n) {
	//
}

/* Return non-zero if c is congruent to d modulo n */
int BI_congruent_p(const BIGINT c, const BIGINT d, const BIGINT n) {
	//
}

int BI_congruent_ui_p(const BIGINT c, u64 d, u64 n) {
	//
}

/* Set rop to (base raised to exp) modulo mod. */
void BI_powm(BIGINT rop, const BIGINT base, const BIGINT exp, const BIGINT mod) {
	//
}

void BI_powm_ui (BIGINT rop, const BIGINT base, u64 exp, const BIGINT mod) {
	//
}

/******************************TEST*********************************************/

int main() {
	//do testing here
	return 0;
}