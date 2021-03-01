#include <perf.h>
#include <stdio.h>

#define BUF_SIZE (1u << 14)

const char flash[BUF_SIZE] = "this is a test buffer";

#define TEST(NAME, LINE, PRE, POST) \
	printf("benchmark: " #NAME ", size %u\n", BUF_SIZE);\
	printf("run, cycles\n");\
	for (size_t i  = 0; i < 100; i++)\
	{\
		PRE;\
		unsigned begin = perf_cycles();\
		LINE;\
		unsigned end = perf_cycles();\
		POST;\
		printf("%u, %u\n", i, end-begin);\
	}\
	printf("benchmark: memcpy_bandwidth_instructions, size %u\n", BUF_SIZE);\
	printf("run, instructions\n");\
	for (size_t i  = 0; i < 100; i++)\
	{\
		PRE;\
		unsigned begin = perf_instructions_retired();\
		LINE;\
		unsigned end = perf_instructions_retired();\
		POST;\
		printf("%u, %u\n", i, end-begin);\
	}


int main(void)
{
	char sram1[BUF_SIZE];
	char sram2[BUF_SIZE];

	// memcpy
	printf("sram locations: %p %p\n", sram1, sram2);
	printf("flash locations: %p\n", flash);
	printf("### RESULTS ###\n");
	TEST(memcpy_bandwidth_cycles, memcpy(sram1, sram2, sizeof(sram1)),,)
	TEST(manual_copy_bandwidth_cycles,
		((volatile char*)sram1)[i] = ((volatile char*)sram2)[i],,)

	unsigned sum = 0;
	TEST(read_bandwidth_instructions, unsigned val = ((volatile char*)sram1)[i];,, sum += val)
	printf("Ignore this value: %u\n", sum);

	TEST(write_bandwidth_instructions, ((volatile char*)sram1)[i] = 1;,, sum += ((volatile char*)sram1)[i])

	return 0;
}
