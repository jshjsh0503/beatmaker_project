#include <stdio.h>

#define EX1
//#define EX2
//#define EX3
//#define EX4
//전처리기를 이용한 선택적 컴파일러

int main(void)
{
#if defined(EX1) || defined(EX2) || defined(EX4)
	const int a = 7;
#endif
#ifdef EX1
	int *p = &a;

	++*p;
	printf("*p=%d, a=%d\n", *p, a);
#endif
#ifdef EX2
	const int *p = &a;

	*p = 3;
#endif
#ifdef EX3
	int a = 7;
	int *const p = &a;
	int b = 4;

	p = &b;
	*p = 3;
	printf("*p=%d, a=%d\n", *p, a);
#endif
#ifdef EX4
	const int *const p = &a;
	int b = 4;

	p = &b;
	*p = 3;
#endif
}
