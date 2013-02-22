##NAME

db - command line key value store

##SYNOPSIS

db [-f file] key [value]

db [-f file] -k

db [-f file] -d key

##DESCRIPTION

Insert and retrieve strings from a Berkeley DB on the command line.
If *value* is specified, it is stored in the database at *key*;
otherwise, the value already stored at *key* is printed to stdout.
In the alternate forms, all keys can be dumped to stdout, or a given
record can be deleted.

##OPTIONS

-f file
:	Use *file* instead of the default database.

-k
:	Dump all keys to stdout.

-d
:	Delete *key* from the database instead of retrieving its value.

##ENVIRONMENT

HOME
:	If a database file is not specified, and the default database
file is not found in the current directory, the *HOME* directory
is searched

##FILES
.db
:	Default database file

~/.db
:	Fallback default database file

##AUTHOR

Written by Carl D Hamann (laindir).

##BUGS

* May continue to do things it shouldn't after an error
* A case could probably be made to use a BTREE instead of a HASH
* Shouldn't print anything to stdout when key not found, but does
* Should allow error messages to be suppressed
* Should give a useful exit code

##COPYRIGHT

Copyright 2013 Carl D Hamann
Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
