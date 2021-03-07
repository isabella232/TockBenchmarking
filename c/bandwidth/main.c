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
	printf("benchmark: " #NAME ", size %u\n", BUF_SIZE);\
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
	volatile unsigned sram1[BUF_SIZE/4];
	volatile unsigned sram2[BUF_SIZE/4];

	// memcpy
	printf("sram locations: %p %p\n", sram1, sram2);
	printf("flash locations: %p\n", flash);
	printf("### RESULTS ###\n");
	TEST(memcpy_bandwidth_cycles, memcpy((char*)sram1, (char*)sram2, sizeof(sram1)),,)
	TEST(manual_copy_bandwidth_cycles,
		for (size_t j = 0; j < sizeof(sram1)/4; ++j) sram1[j] = sram2[j];,
		,
	)

	unsigned sum = 0;
	TEST(read_bandwidth_instructions,
		for (size_t j = 0; j < sizeof(sram1)/4; ++j) val = sram1[j];,
		volatile unsigned val;,
		for (size_t j = 0; j < sizeof(sram1)/4; ++j) sum += sram1[j] + val;
	)
	printf("Ignore this value: %u\n", sum);

	TEST(write_bandwidth_instructions,
		for (size_t j = 0; j < sizeof(sram1)/4; ++j) sram1[j] = val;,
		volatile unsigned val = 0xFFFF0000 + i,
		for (size_t j = 0; j < sizeof(sram1)/4; ++j) sum += sram1[j];
	)
	printf("Ignore this value: %u\n", sum);

	return 0;
}
