#include "Common.hpp"

void	printErrorAndExit(const char *fmt, ...) {

	int		errno_save;
	va_list	ap;

	// any system or library call can set errno, so we need to save it now
	errno_save = errno;

	// print out the fmt+args to standard output
	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	fflush(stdout);

	// print out error message if errno was set
	if (errno_save != 0) {
		fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
		fprintf(stdout, "\n");
		fflush(stdout);
	}

	va_end(ap);
	exit(EXIT_FAILURE);
}

char	*bin2hex(const unsigned char *input, size_t len) {

	char		*result;
	const char	*hexits = "0123456789ABCDEF";

	if (input == NULL || len <= 0) {
		return NULL;
	}

	// 3 chars for every byte, plus a null terminator
	int		resultlength = (len * 3) + 1;

	result = (char *)malloc(resultlength);
	bzero(result, resultlength);

	for (int i = 0; i < len; i++) {
		result[i * 3] = hexits[input[i] >> 4];
		result[(i * 3) + 1] = hexits[input[i] & 0x0F];
		result[(i * 3) + 2] = ' ';
	}

	return result;
}
