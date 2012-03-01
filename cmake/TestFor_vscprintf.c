#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void format(char** out, char* format, ...)
{
	int len;
	char* ret;
	va_list args;
	
	va_start(args, format);
	
	len = _vscprintf(format, args);
	ret = malloc(len + 1);
	
	_vsnprintf(ret, len, format, args);
	
	va_end(args);
	ret[len] = 0;
	
	*out = ret;
}

int main(int argc, const char* argv)
{
	char* ret;
	
	format(&ret, "Tasker test MSVC _vscprintf, _vsnprintf functions\n");
	
	printf("%s", ret);
	
	free(ret);
	
	return 0;
}