#include <perf.h>
#include <stdio.h>

#define BUF_SIZE (1u << 14)

#define TEST(NAME, LINE, PRE, POST, SIZE) \
{\
	printf("benchmark: " #NAME "_cycles, size %u\n", SIZE);\
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
	printf("benchmark: " #NAME "_instructions, size %u\n", SIZE);\
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
	TEST(NAME, LINE, PRE, POST, 1024) \
	TEST(NAME, LINE, PRE, POST, 2048) \
	TEST(NAME, LINE, PRE, POST, 3072) \
	TEST(NAME, LINE, PRE, POST, 4096) \
	TEST(NAME, LINE, PRE, POST, 5120) \
	TEST(NAME, LINE, PRE, POST, 6144) \
	TEST(NAME, LINE, PRE, POST, 7168) \
	TEST(NAME, LINE, PRE, POST, 8192) \
	TEST(NAME, LINE, PRE, POST, 9216) \
	TEST(NAME, LINE, PRE, POST, 10240) \
	TEST(NAME, LINE, PRE, POST, 11264) \
	TEST(NAME, LINE, PRE, POST, 12288) \
	TEST(NAME, LINE, PRE, POST, 13312) \
	TEST(NAME, LINE, PRE, POST, 14336) \
	TEST(NAME, LINE, PRE, POST, 15360) \
	TEST(NAME, LINE, PRE, POST, 16384)

unsigned sram1[BUF_SIZE/4];
unsigned sram2[BUF_SIZE/4];

int main(void)
{
	// memcpy
	printf("### RESULTS ###\n");
	for (size_t j = 0; j < sizeof(sram1)/4; ++j) sram2[j] = perf_cycles();
	TEST_SET(memcpy_bandwidth, memcpy(sram1, sram2, SIZE_),,)
	TEST_SET(manual_copy_bandwidth,
		for (size_t j = 0; j < SIZE_/4; ++j) sram1[j] = sram2[j];,
		,
	)

	for (size_t j = 0; j < sizeof(sram1)/4; ++j) sram1[j] = perf_cycles();
	unsigned sum = 0;
	TEST_SET(read_bandwidth,
		for (size_t j = 0; j < SIZE_/4; ++j) sum += sram1[j];
		,
		,
	)

	for (size_t j = 0; j < sizeof(sram1)/4; ++j) sram1[j] = perf_cycles();
	TEST_SET(write_bandwidth,
		for (size_t j = 0; j < SIZE_/4; ++j) sram1[j] = val;,
		unsigned val = 0xFFFF0000 + i,
		for (size_t j = 0; j < SIZE_/4; ++j) sum += sram1[j];
	)
	printf("Ignore this value: %u\n", sum);

	return 0;
}
