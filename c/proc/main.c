/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <perf.h>
#include <stdint.h>
#include <inttypes.h>

#define TEST(NAME, FUNC, ...) \
void test_##NAME(void); \
void test_##NAME(void) \
{ \
  printf("### RESULTS ###\n"); \
  printf("benchmark: " #NAME "_proc_cycles\n"); \
  printf("run, cycles\n"); \
  for(size_t i = 0; i < 1000; ++i) {\
    unsigned cycles1 = perf_cycles(); \
    FUNC(__VA_ARGS__); \
    unsigned cycles2 = perf_cycles(); \
    printf("%u, %u\n", i, cycles2 - cycles1); \
  }\
  printf("benchmark: " #NAME "_proc_instructions\n"); \
  printf("run, instructions\n"); \
  for(size_t i = 0; i < 1000; ++i) {\
    unsigned instr1 = perf_instructions_retired(); \
    FUNC(__VA_ARGS__); \
    unsigned instr2 = perf_instructions_retired(); \
    printf("%u, %u\n", i, instr2 - instr1); \
  }\
}

void __attribute__((noinline, optimize(0))) test_void(void);
int __attribute__((noinline, optimize(0))) test_ret_(void);
int __attribute__((noinline, optimize(0))) test_1(int);
int __attribute__((noinline, optimize(0))) test_2(int,int);
int __attribute__((noinline, optimize(0))) test_3(int,int,int);
int __attribute__((noinline, optimize(0))) test_4(int,int,int,int);
int __attribute__((noinline, optimize(0))) test_5(int,int,int,int,int);
int __attribute__((noinline, optimize(0))) test_6(int,int,int,int,int,int);
int __attribute__((noinline, optimize(0))) test_7(int,int,int,int,int,int,int);
int __attribute__((noinline, optimize(0))) test_8(int,int,int,int,int,int,int,int);
int __attribute__((noinline, optimize(0))) test_9(int,int,int,int,int,int,int,int,int);
int __attribute__((noinline, optimize(0))) test_10(int,int,int,int,int,int,int,int,int,int);

TEST(plain, test_void)
TEST(ret, test_ret_)
TEST(arg1, test_1, 1)
TEST(arg2, test_2, 1,2)
TEST(arg3, test_3, 1,2,3)
TEST(arg4, test_4, 1,2,3,4)
TEST(arg5, test_5, 1,2,3,4,5)
TEST(arg6, test_6, 1,2,3,4,5,6)
TEST(arg7, test_7, 1,2,3,4,5,6,7)
TEST(arg8, test_8, 1,2,3,4,5,6,7,8)
TEST(arg9, test_9, 1,2,3,4,5,6,7,8,9)
TEST(arg10, test_10, 1,2,3,4,5,6,7,8,9,10)

int main(void) {
  test_plain();
  test_ret();
  test_arg1();
  test_arg2();
  test_arg3();
  test_arg4();
  test_arg5();
  test_arg6();
  test_arg7();
  test_arg8();
  test_arg9();
  test_arg10();
  return 0;
}

void __attribute__((noinline, optimize(0))) test_void(void)
{
	return;
}

int __attribute__((noinline, optimize(0))) test_ret_(void)
{
	return 1;
}

int __attribute__((noinline, optimize(0))) test_1(int a)
{
	return a;
}

int __attribute__((noinline, optimize(0))) test_2(int a, int b)
{
	(void)a;
	return b;
}

int __attribute__((noinline, optimize(0))) test_3(int a, int b, int c)
{
	(void)a;
	(void)b;
	return c;
}

int __attribute__((noinline, optimize(0))) test_4(int a, int b, int c, int d)
{
	(void)a;
	(void)b;
	(void)c;
	return d;
}

int __attribute__((noinline, optimize(0))) test_5(int a, int b, int c, int d, int e)
{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	return e;
}

int __attribute__((noinline, optimize(0))) test_6(int a, int b, int c, int d, int e, int f)
{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	return f;
}

int __attribute__((noinline, optimize(0))) test_7(int a, int b, int c, int d, int e, int f, int g)
{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	return g;
}

int __attribute__((noinline, optimize(0))) test_8(int a, int b, int c, int d, int e, int f, int g, int h)
{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	(void)g;
	return h;
}

int __attribute__((noinline, optimize(0))) test_9(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	(void)g;
	(void)h;
	return i;
}

int __attribute__((noinline, optimize(0))) test_10(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j)
{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	(void)g;
	(void)h;
	(void)i;
	return j;
}
