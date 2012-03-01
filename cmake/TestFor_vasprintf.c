#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void format(char** out, char* format, ...)
{
	va_list args;
	
	va_start(args, format);
	vasprintf(out, format, args);
	va_end(args);
}

int main(int argc, const char* argv)
{
	char* ret;
	
	format(&ret, "Tasker test GNU lib C vasprintf function\n");
	
	printf(ret);
	
	free(ret);
	
	return 0;
}
