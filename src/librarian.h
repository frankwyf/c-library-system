#ifndef LIB_LIBRARIAN_H
#define LIB_LIBRARIAN_H

#include "structures.h"

/* ---- Non-interactive (unit-testable) ---- */

/* Print all catalogue entries as "  [idx] author  title". */
void librarian_list_all(const Book *book_list, int num_books);

/* Print only entries where is_borrowed == 1. */
void librarian_list_borrowed(const Book *book_list, int num_books);

/*
 * Add a book with the given author and title.
 * Returns LIB_OK on success, LIB_ERR if the catalogue is full.
 */
int librarian_do_add(Library *lib, const char *author, const char *title);

/*
 * Remove the book at catalogue index idx.
 * Returns LIB_ERR if idx is out of range or the book is currently borrowed.
 */
int librarian_do_remove(Library *lib, int idx);

/* ---- Interactive wrappers ---- */

/* Prompt for author/title, then call librarian_do_add(). */
int librarian_add_book(Library *lib);

/* List catalogue, prompt for index, then call librarian_do_remove(). */
int librarian_remove_book(Library *lib);

/* Interactive menu loop for a librarian session. */
void librarian_cli(Library *lib);

#endif /* LIB_LIBRARIAN_H */
