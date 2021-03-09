#include <perf.h>
#include <stdio.h>

#define BUF_SIZE (1u << 14)

#define TEST(NAME, LINE, PRE, POST, SIZE) \
{\
	printf("benchmark: " #NAME ", size %u\n", SIZE);\
	printf("run, cycles\n");\
	const size_t SIZE_ = SIZE;\
	for (size_t i  = 0; i < 100; i++)\
	{\
		PRE;\
		unsigned begin = perf_cycles();\
		LINE;\
		unsigned end = perf_cycles();\
		POST;\
		printf("%u, %u\n", i, end-begin);\
	}\
	printf("benchmark: " #NAME ", size %u\n", SIZE);\
	printf("run, instructions\n");\
	for (size_t i  = 0; i < 100; i++)\
	{\
		PRE;\
		unsigned begin = perf_instructions_retired();\
		LINE;\
		unsigned end = perf_instructions_retired();\
		POST;\
		printf("%u, %u\n", i, end-begin);\
	}\
}

#define TEST_SET(NAME, LINE, PRE, POST) \
	TEST(NAME, LINE, PRE, POST, 1 << 5) \
	TEST(NAME, LINE, PRE, POST, 1 << 6) \
	TEST(NAME, LINE, PRE, POST, 1 << 7) \
	TEST(NAME, LINE, PRE, POST, 1 << 8) \
	TEST(NAME, LINE, PRE, POST, 1 << 9) \
	TEST(NAME, LINE, PRE, POST, 1 << 10) \
	TEST(NAME, LINE, PRE, POST, 1 << 11) \
	TEST(NAME, LINE, PRE, POST, 1 << 12) \
	TEST(NAME, LINE, PRE, POST, 1 << 13) \
	TEST(NAME, LINE, PRE, POST, 1 << 14)

unsigned sram1[BUF_SIZE/4];
unsigned sram2[BUF_SIZE/4];

int main(void)
{
	// memcpy
	printf("### RESULTS ###\n");
	for (size_t j = 0; j < sizeof(sram1)/4; ++j) sram2[j] = perf_cycles();
	TEST_SET(memcpy_bandwidth_cycles, memcpy(sram1, sram2, SIZE_),,)
	TEST_SET(manual_copy_bandwidth_cycles,
		for (size_t j = 0; j < SIZE_/4; ++j) sram1[j] = sram2[j];,
		,
	)

	for (size_t j = 0; j < sizeof(sram1)/4; ++j) sram1[j] = perf_cycles();
	unsigned sum = 0;
	TEST_SET(read_bandwidth_instructions,
		for (size_t j = 0; j < SIZE_/4; ++j) sum += sram1[j];
		,
		,
	)

	for (size_t j = 0; j < sizeof(sram1)/4; ++j) sram1[j] = perf_cycles();
	TEST_SET(write_bandwidth_instructions,
		for (size_t j = 0; j < SIZE_/4; ++j) sram1[j] = val;,
		unsigned val = 0xFFFF0000 + i,
		for (size_t j = 0; j < SIZE_/4; ++j) sum += sram1[j];
	)
	printf("Ignore this value: %u\n", sum);

	return 0;
}
