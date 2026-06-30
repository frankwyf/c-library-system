#ifndef LIB_LIBRARY_H
#define LIB_LIBRARY_H

#include "structures.h"
#include <stdio.h>

/*
 * Initialise library from a book data file.
 * Allocates book_list, opens and reads the file, initialises the user.
 * Returns LIB_OK on success; prints to stderr and returns LIB_ERR on failure.
 */
int library_init(const char *book_file, Library *lib);

/*
 * Read book records from an open file pointer into book_list.
 * book_list must have capacity for at least max_books entries.
 *
 * File format — each record:
 *   <author line>
 *   <title line>
 *   <blank line>
 *
 * Returns the number of books successfully read, or LIB_ERR on hard error.
 */
int library_read_books(FILE *fp, Book *book_list, int max_books);

/*
 * Free all heap memory owned by lib and zero the pointer fields.
 * Safe to call with a NULL pointer.
 */
void library_free(Library *lib);

/*
 * Run the interactive top-level CLI loop until the user chooses Exit.
 */
void library_cli(const char *book_file);

#endif /* LIB_LIBRARY_H */
