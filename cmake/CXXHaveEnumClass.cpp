#include <cstdio>

enum class EnumClass : int
{
	A = 0,
	B,
};

int main(int argc, const char** argv)
{
	printf("%d\n", EnumClass::A);
	return 0;
}
