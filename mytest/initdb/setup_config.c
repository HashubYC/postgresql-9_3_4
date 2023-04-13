#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define _(x) (x)
#define MAXPGPATH		1024


const char *pg_data="/data/postgres";
const char *progname="my progname";
static int	n_connections = 10;
static int	n_buffers = 50;
static int	BLCKSZ = 4096;
static char *conf_file="/data/postgres/postgresql.conf";

static char **readfile(const char *path);
static void writefile(char *path, char **lines);
static void set_null_conf(void);
static void setup_config(void);
static char backend_exec[MAXPGPATH];
static const char *boot_options = "-F";
#define SYSTEMQUOTE ""
#define DEVNULL "/dev/null"



static char **
readfile(const char *path)
{
	FILE	   *infile;
	int			maxlength = 1,
				linelen = 0;
	int			nlines = 0;
	int			n;
	char	  **result;
	char	   *buffer;
	int			c;

	if ((infile = fopen(path, "r")) == NULL)
	{
		fprintf(stderr, _("%s: could not open file \"%s\" for reading: %s\n"),
				progname, path, strerror(errno));
	}

	/* pass over the file twice - the first time to size the result */

	while ((c = fgetc(infile)) != EOF)
	{
		linelen++;
		if (c == '\n')
		{
			nlines++;
			if (linelen > maxlength)
				maxlength = linelen;
			linelen = 0;
		}
	}

	/* handle last line without a terminating newline (yuck) */
	if (linelen)
		nlines++;
	if (linelen > maxlength)
		maxlength = linelen;

	/* set up the result and the line buffer */
	result = (char **) malloc((nlines + 1) * sizeof(char *));
	buffer = (char *) malloc(maxlength + 1);

	/* now reprocess the file and store the lines */
	rewind(infile);
	n = 0;
	while (fgets(buffer, maxlength + 1, infile) != NULL && n < nlines)
		result[n++] = strdup(buffer);

	fclose(infile);
	free(buffer);
	result[n] = NULL;

	return result;
}

static void
writefile(char *path, char **lines)
{
	FILE	   *out_file;
	char	  **line;

	if ((out_file = fopen(path, "w")) == NULL)
	{
		fprintf(stderr, _("%s: could not open file \"%s\" for writing: %s\n"),
				progname, path, strerror(errno));
	}
	for (line = lines; *line != NULL; line++)
	{
		if (fputs(*line, out_file) < 0)
		{
			fprintf(stderr, _("%s: could not write file \"%s\": %s\n"),
					progname, path, strerror(errno));
		}
		free(*line);
	}
	if (fclose(out_file))
	{
		fprintf(stderr, _("%s: could not write file \"%s\": %s\n"),
				progname, path, strerror(errno));
	}
}


static void
set_null_conf(void)
{
	FILE	   *conf_file;
	char	   *path;

	path = malloc(strlen(pg_data) + 17);
	sprintf(path, "%s/postgresql.conf", pg_data);
	conf_file = fopen(path, "w");
	if (conf_file == NULL)
	{
		fprintf(stderr, _("%s: could not open file \"%s\" for writing: %s\n"),
				"progname", path, strerror(errno));
	}
	if (fclose(conf_file))
	{
		fprintf(stderr, _("%s: could not write file \"%s\": %s\n"),
				"progname", path, strerror(errno));
	}
	free(path);
}


static char **
replace_token(char **lines, const char *token, const char *replacement)
{
	int			numlines = 1;
	int			i;
	char	  **result;
	int			toklen,
				replen,
				diff;

	for (i = 0; lines[i]; i++)
		numlines++;

	result = (char **) malloc(numlines * sizeof(char *));

	toklen = strlen(token);
	replen = strlen(replacement);
	diff = replen - toklen;

	for (i = 0; i < numlines; i++)
	{
		char	   *where;
		char	   *newline;
		int			pre;

		/* just copy pointer if NULL or no change needed */
		if (lines[i] == NULL || (where = strstr(lines[i], token)) == NULL)
		{
			result[i] = lines[i];
			continue;
		}

		/* if we get here a change is needed - set up new line */

		newline = (char *) malloc(strlen(lines[i]) + diff + 1);

		pre = where - lines[i];

		strncpy(newline, lines[i], pre);

		strcpy(newline + pre, replacement);

		strcpy(newline + pre + replen, lines[i] + pre + toklen);

		result[i] = newline;
	}

	return result;
}







static void
setup_config(void)
{

	char	  **conflines;
	char		repltok[MAXPGPATH];
	char		path[MAXPGPATH];
	const char *default_timezone;

	fputs(_("creating configuration files ... "), stdout);
	fflush(stdout);

	/* postgresql.conf */

	conflines = readfile(conf_file);

	snprintf(repltok, sizeof(repltok), "max_connections = %d", 10);
	conflines = replace_token(conflines, "#max_connections = 100", repltok);

	if ((n_buffers * (BLCKSZ / 1024)) % 1024 == 0)
		snprintf(repltok, sizeof(repltok), "shared_buffers = %dMB",
				 (n_buffers * (BLCKSZ / 1024)) / 1024);
	else
		snprintf(repltok, sizeof(repltok), "shared_buffers = %dkB",
				 n_buffers * (BLCKSZ / 1024));
	conflines = replace_token(conflines, "#shared_buffers = 32MB", repltok);
	snprintf(path, sizeof(path), "%s/postgresql.conf", pg_data);

	writefile(path, conflines);
	chmod(path, S_IRUSR | S_IWUSR);

    free(conflines);
}


void main() {
    set_null_conf();
    setup_config();
}


