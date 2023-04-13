#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define gettext(x) (x)

static void
write_version_file(char *extrapath)
{   
    FILE *version_file;
    char *path;
    const char *pg_data="/data/postgres";
    
    path = malloc(strlen(pg_data) + 12);
    sprintf(path, "%s/PG_VERSION", pg_data);

    version_file = fopen(path, "w");

    if (fprintf(version_file, "%s\n", "13") < 0 || fclose(version_file))
	{
		fprintf(stderr, "%s: could not write file \"%s\": %s\n",
				"my progname", path, strerror(errno));
		// exit_nicely();
	}


	free(path);
}

int main () {
    write_version_file(NULL);
}