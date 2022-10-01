#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int is_file_exist_stats(const char *path)
{
	struct stat stats;

	stat(path, &stats);

	// Check for file existence
	if (stats.st_mode & F_OK)
		return 1;

	return 0;
}
