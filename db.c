/*

Copyright 2013 Carl D Hamann

This file is part of db.

db is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

db is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with db.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <db.h>

/** TODO
add flag to append to a particular key
add arg to delete a particular entry
*/
static const char *options = "kdf:";
static const char *default_filename = ".db";
/** TODO
pull out usage message
*/

int
db_try(const char *trace, int code)
{
	if(code != 0)
	{
		fprintf(stderr, "%s: %s\n", trace, db_strerror(code));
	}

	return code == 0;
}

int
try(const char *trace, int code)
{
	if(code != 0)
	{
		perror(trace);
	}

	return code == 0;
}

int
set_db_file(char const **dbfile, char **allocated, int sz_alloc)
{
	int ret = 0;
	struct stat sbstat;
	char *homedir;

	if(*dbfile == NULL)
	{
		ret = stat(default_filename, &sbstat);
		if(ret == -1)
		{
			if(errno == ENOENT)
			{
				homedir = getenv("HOME");

				ret = snprintf(NULL, 0, "%s/%s", homedir, default_filename);

				if((ret + 1) > sz_alloc)
				{
					try("realloc", realloc(*allocated, ret + 1) == NULL);
				}

				snprintf(*allocated, ret + 1, "%s/%s", homedir, default_filename);
				*dbfile = *allocated;
				ret = 0;
			}
			else
			{
				try("stat", -1);
			}
		}
		else
		{
			*dbfile = default_filename;
		}
	}

	return ret;
}

int
main(int argc, char *argv[])
{
	DB* db;
	DBT key;
	DBT value;
	DBT retval;
	DBC *cursor;
	int ret = -1;
	int dumpkeys = 0;
	int delete = 0;
	const char *dbfile = NULL;
	char *buildstring = NULL;
	char *keystring = NULL;
	char *valstring = NULL;

	memset(&key, 0, sizeof(key));
	memset(&value, 0, sizeof(value));
	memset(&retval, 0, sizeof(retval));

	ret = getopt(argc, argv, options);

	while(ret != -1)
	{
		switch(ret)
		{
		case 'k':
			dumpkeys = 1;
			break;
		case 'd':
			delete = 1;
			break;
		case 'f':
			dbfile = optarg;
			break;
		}

		ret = getopt(argc, argv, options);
	}

	buildstring = malloc(32);
	ret = set_db_file(&dbfile, &buildstring, 32);

	db_try("create", db_create(&db, NULL, 0));
	db_try("open", db->open(db, NULL, dbfile, NULL, DB_HASH, DB_CREATE, 0));

	if(buildstring != NULL)
	{
		free(buildstring);
	}

	switch(argc - optind)
	{
	case 2:
		valstring = argv[optind + 1];
		value.data = valstring;
		value.size = strlen(valstring) + 1;
	case 1:
		keystring = argv[optind];
		key.data = keystring;
		key.size = strlen(keystring) + 1;
	}

	if(dumpkeys == 1 && delete == 0)
	{
		db_try("cursor", db->cursor(db, NULL, &cursor, 0));

		while((ret = cursor->get(cursor, &key, &retval, DB_NEXT)) == 0)
		{
			fprintf(stdout, "%s\n", (char *)key.data);
		}

		db_try("cursor close", cursor->close(cursor));
	}
	else if(dumpkeys == 0 && delete == 1 && keystring != NULL)
	{
/** TODO
add cursor to handle deleting specific entry when multiple
*/
		db_try("delete", db->del(db, NULL, &key, 0));
	}
	else if(dumpkeys == 0 && delete == 0 && keystring != NULL && valstring != NULL)
	{
/** TODO
change DB_NOOVERWRITE so we can handle multiple keys
*/
		db_try("put", db->put(db, NULL, &key, &value, DB_NOOVERWRITE));
	}
	else if(dumpkeys == 0 && delete == 0 && keystring != NULL && valstring == NULL)
	{
/** TODO
add cursor to handle multiple entries
*/
		db_try("get", db->get(db, NULL, &key, &retval, 0));
		fprintf(stdout, "%s\n", (char *)retval.data);
	}
	else
	{
		fprintf(stderr, "Usage: %s <key> [<value>]\n", argv[0]);
	}

	db_try("close", db->close(db, 0));

	return 0;
}
