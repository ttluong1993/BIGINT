/*
 * bigint.h - declaration of a simple implementation of BIG integers 
 * 
 * created: October 20, 2015
 * 
 * by Le Duc Phong
 *
 */
 #include <stdint.h>
 
 #define MAXSIZE	( 256 / 8 )	// = 32 bytes
 #define MAXVALUE	256

 #define strSpace "0123456789abcdefghijklmnopqrstuvwxzy"
 
 typedef uint8_t u8;			// unsigned 8-bits integer 
 typedef uint16_t u16; 			// unsigned 16-bits integer 
 typedef uint32_t u32; 			// unsigned 32-bits integer 
 typedef uint64_t u64; 			// unsigned 64-bits integer 
 //typedef uint128_t u128; 		// unsigned 64-bits integer 
 
 typedef struct bigint_st* BIGINT;
 
 struct bigint_st {
	u8 value[MAXSIZE + 1];	/* represent the big number */
	int8_t signbit; 		/* 1 if positive, -1 if negative */
	u8 size;				/* size in bit */
 };

/* Inverse a string
 * Input: char* ;	Return: char*
 */
char* inv_str(const char *str);

//compute size in bits.
u8 compute_size(BIGINT rop);

/* Initialize a big integer before using 
 * 
 * Allocate a memory space for the variable x
 * 
 */
void BI_init(BIGINT *x);

/* Release a big integer after using */
void BI_clear(BIGINT *x);

/* Print out a big number to FILE */
void BI_print(FILE* out, u8 base, const BIGINT x); 

/* Assignment functions. 
 * 
 * Assuming that big integer variables were allocated a memory space 
 * 
 * Set the value of rop from op
 */
void BI_set(BIGINT rop, const BIGINT op);
void BI_set_ui(BIGINT rop, const u64 op);
void BI_set_si(BIGINT rop, const int64_t op);	// op is a signed 64-bit integer
int BI_set_str(BIGINT rop, const char* str, int base);	/*
														 * Set the value of rop from str in base base. 
														 * The base may vary from values 2 (binary), 10 (decimal) or 16 (hex)
														 * This function returns 0 if the entire string is a valid number in base base. Otherwise it returns â1.	
														 */
void BI_set_pow_2(BIGINT rop, u8 n);	// Set rop = 2^n

void BI_swap(BIGINT x, BIGINT y);		// swap the values x and y

/* Combined Initialization and Assignment Functions
 *  
 * Initialize rop with limb space and set the initial numeric value from op.
 */
void BI_init_set(BIGINT *rop, const BIGINT op);
void BI_init_set_ui(BIGINT *rop, u64 op);
void BI_init_set_si(BIGINT *rop, int64_t op);
int BI_init_set_str(BIGINT *rop, const char *str, int base);

/* Conversion functions */

u64 BI_get_ui(const BIGINT op);		/* Return the value of op as an 64-bit unsigned integer. 
									   If op is too big to fit an unsigned long then just the least significant bits that do fit are returned */

int64_t BI_get_si(const BIGINT op); /* If op fits into a signed long int return the value of op. 
									   Otherwise return the least significant part of op, with the same sign as op. */

char* BI_get_str(char *str, int base, const BIGINT op); /* Convert op to a string of digits in base base */

/* Random Number Functions */
void BI_randb(BIGINT rop, u8 n); 			/* Generate a uniformly distributed random integer in the range 0 to 2^nâ1, inclusive. */
void BI_randm(BIGINT rop, const BIGINT n); 	/* Generate a uniformly distributed random integer in the range 0 to n â 1, inclusive. */

/* Comparison Functions */

/* Compare op1 and op2. 
 * 
 * Return a positive value if op1 > op2, zero if op1 = op2, or a negative value if op1 < op2. 
 * 
 */
int BI_cmp(const BIGINT op1, const BIGINT op2);
int BI_cmp_si(const BIGINT op1, int64_t op2);
int BI_cmp_ui(const BIGINT op1, u64 op2);

/* Compare the absolute values of op1 and op2. 
 * Return a positive value if abs(op1) > abs(op2), zero if abs(op1) = abs(op2), or a negative value if abs(op1) < abs(op2).
 */
int BI_cmpabs(const BIGINT op1, const BIGINT op2);
int BI_cmpabs_ui(const BIGINT op1, u64 op2);

/* Return +1 if op > 0, 0 if op = 0, and -1 if op < 0. */
int BI_sgn(const BIGINT op);

/*
 * Logical and Bit Manipulation Functions
 */
void BI_and(BIGINT rop, const BIGINT op1, const BIGINT op2); /* Set rop to op1 bitwise-and op2. */

void BI_ior(BIGINT rop, const BIGINT op1, const BIGINT op2); /* Set rop to op1 bitwise inclusive-or op2. */

void BI_xor(BIGINT rop, const BIGINT op1, const BIGINT op2); /* Set rop to op1 bitwise exclusive-or op2. */

void BI_com(BIGINT rop, const BIGINT op); /* Set rop to the one's complement of op. */

u8 BI_Hammingweight(const BIGINT op); /* Count the number of 1 bits in the binary representation. */

u8 BI_hamdist(const BIGINT op1, const BIGINT op2); /* Return the hamming distance between the two operands, 
													* which is the number of bit positions where op1 and op2 have different bit values. 
													*/
													
/* Scan op, starting from bit starting_bit, towards more significant bits, until the first 0 or 1 bit (respectively) is found. 
 * 
 * Return the index of the found bit. 
 * 
 * If the bit at starting_bit is already what's sought, then starting_bit is returned.
 * 
 * If there's no bit found, then the largest possible u8 is returned.
 * 
 */
u8 BI_scan0(const BIGINT op, u8 starting_bit);
u8 BI_scan1(const BIGINT op, u8 starting_bit);

void BI_setbit(BIGINT rop, u8 bit_index); /* Set bit bit_index in rop. */

void BI_clrbit(BIGINT rop, u8 bit_index); /* Clear bit bit_index in rop. */

void BI_combit(BIGINT rop, u8 bit_index); /* Complement bit bit_index in rop. */

int BI_tstbit(const BIGINT op, u8 bit_index); /* Test bit bit_index in op and return 0 or 1 accordingly. */

/* Determine whether op is odd or even, respectively. 
 * Return non-zero if yes, zero if no. 
 */  
int BI_odd_p(const BIGINT op);
int BI_even_p(const BIGINT op);

/* Return the size of op measured in number of digits in the given base. 
 * The sign of op is ignored, just the absolute value is used. 
 * The result will be either exact or 1 too big. If base is a power of 2, the result is always exact. If op is zero the return value is always 1.
 */
u8 BI_sizeinbase(const BIGINT op, int base); 


/* 
 * Arithmetic Functions 
 */

/* Set rop = op1 + op2 */
void BI_add(BIGINT rop, const BIGINT op1, const BIGINT op2);
void BI_add_ui(BIGINT rop, const BIGINT op1, u64 op2);

/* Set rop = op1 - op2 */
void BI_sub(BIGINT rop, const BIGINT op1, const BIGINT op2);
void BI_sub_ui(BIGINT rop, const BIGINT op1, u64 op2);
void BI_ui_sub(BIGINT rop, u64 op1, const BIGINT op2);

/* Set rop = op1 x op2. */
void BI_mul(BIGINT rop, const BIGINT op1, const BIGINT op2);
void BI_mul_si(BIGINT rop, const BIGINT op1, int64_t op2);
void BI_mul_ui(BIGINT rop, const BIGINT op1, u64 op2);
void BI_mulm(BIGINT rop, const BIGINT op1, const BIGINT op2, const BIGINT m);

/* Set rop = rop + op1 x op2 */
void BI_addmul(BIGINT rop, const BIGINT op1, const BIGINT op2);
void BI_addmul_ui(BIGINT rop, const BIGINT op1, u64 op2);

/* Set rop to rop - op1 times op2 */
void BI_submul(BIGINT rop, const BIGINT op1, const BIGINT op2);
void BI_submul_ui(BIGINT rop, const BIGINT op1, u64 op2);

/* Set rop = op1 x 2 ^ op2. This operation can also be defined as a left shift by op2 bits */
void BI_shift(BIGINT rop, const BIGINT op1, u8 op2);

/* Set rop = op1 / op2. Division is undefined if the divisor is zero */
void BI_div(BIGINT rop, const BIGINT op1, const BIGINT op2);
void BI_div_si(BIGINT rop, const BIGINT op1, int64_t op2);
void BI_div_ui(BIGINT rop, const BIGINT op1, u64 op2);
int BI_divm(BIGINT rop, const BIGINT op1, const BIGINT op2, const BIGINT m);

/* Set rop to -op */
void BI_neg(BIGINT rop, const BIGINT op);

/* Set rop to the absolute value of op */
void BI_abs(BIGINT rop, const BIGINT op);


/* Modulo arithmetic functions */

/* Set rop = d mod n. The sign of the divisor is ignored; the result is always non-negative. */
void BI_mod(BIGINT rop, const BIGINT d, const BIGINT n);
u64 BI_mod_ui(BIGINT rop, const BIGINT d, u64 n);

/* Set rop = modular multiplicative inverse of op in modular mod.
 * Return 1 if it's success. Otherwise, return 0.
 */
int BI_inverse(BIGINT rop, const BIGINT op, const BIGINT mod);

/* Return non-zero if c is congruent to d modulo n */
int BI_congruent_p(const BIGINT c, const BIGINT d, const BIGINT n);
int BI_congruent_ui_p(const BIGINT c, u64 d, u64 n);

/* Set rop to (base raised to exp) modulo mod. */
void BI_powm(BIGINT rop, const BIGINT base, const BIGINT exp, const BIGINT mod);
void BI_powm_ui (BIGINT rop, const BIGINT base, u64 exp, const BIGINT mod);

/* Number theory algorithms. */

/* Eclip gcd(a, b) */
void BI_gcd(BIGINT rop, const BIGINT a, const BIGINT b);

/* Extended Eclip
 * Return [g, x, y] satisfy the equation g = ax + by.
 */
void BI_xgcd(BIGINT g, BIGINT x, BIGINT y, const BIGINT a, const BIGINT b);
