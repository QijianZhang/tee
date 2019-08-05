#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

void tee_indent(uint32_t n)
{
	while (n) {
		printf("|  ");
		n -= 1;
	}
}

char *tee_namecat(const char *dir, const char *name)
{
	size_t dir_len = strlen(dir);
	size_t name_len = strlen(name);
	size_t len = dir_len + name_len + 1;
	char *r = malloc(len + 1);
	strcpy(r, dir);
	r[dir_len] = '/';
	strcpy(r + dir_len + 1, name);
	r[len] = 0;
	return r;
}

int tee_dir(const char *dir_name, uint32_t indent)
{
	DIR *dp = opendir(dir_name);
	if (dp == NULL) {
		return 1;
	}

	tee_indent(indent);
	printf("|- %s\n", dir_name);

	indent += 1;
	struct dirent *e = NULL;
	while ((e = readdir(dp)) != NULL) {
		if (e->d_name[0] == '.') {
			continue;
		}
		if (e->d_type == DT_DIR) {
			char *name = tee_namecat(dir_name, e->d_name);
			tee_dir(name, indent);
			free(name);
		} else {
			tee_indent(indent);
			printf("|- %s\n", e->d_name);
		}
	}

	closedir(dp);
	return 0;
}

int main(int argc, char *argv[])
{
	const char *dir = ".";
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			dir = argv[i];
		} else {
			printf("tee -- print directory tree\n"
			       "Usage: tee [dir] [--help]\n"
			       "current directory is used if dir is omitted\n");
			return 0;
		}
	}

	tee_dir(dir, 0);
	return 0;
}

