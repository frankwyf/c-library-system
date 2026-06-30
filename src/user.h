#ifndef LIB_USER_H
#define LIB_USER_H

#include "structures.h"

/* ---- Non-interactive (unit-testable) ---- */

/* Print catalogue entries where is_borrowed == 0. */
void user_list_available(const Book *book_list, int num_books);

/* Print books currently in the user's borrowed list. */
void user_list_borrowed(const User *user);

/*
 * Borrow the catalogue entry at index book_idx.
 * Returns LIB_ERR if: borrow limit reached, book_idx invalid, or already
 * borrowed.
 */
int user_do_borrow(User *user, Book *book_list, int num_books, int max_borrowed,
                   int book_idx);

/*
 * Return the book at borrow-list index borrow_idx.
 * Marks the book as available and compacts the borrowed array.
 * Returns LIB_ERR if: nothing borrowed, or borrow_idx is out of range.
 */
int user_do_return(User *user, int borrow_idx);

/* ---- Interactive wrappers ---- */

/* List available books, prompt for catalogue index, call user_do_borrow(). */
int user_borrow_book(User *user, Book *book_list, int num_books,
                     int max_borrowed);

/* List borrowed books, prompt for borrow-list index, call user_do_return(). */
int user_return_book(User *user);

/* Interactive menu loop for a user session. */
void user_cli(Library *lib);

#endif /* LIB_USER_H */
