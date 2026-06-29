#ifndef LIB_STRUCTURES_H
#define LIB_STRUCTURES_H

/* String buffer sizes */
#define MAX_TITLE_LEN    64
#define MAX_AUTHOR_LEN   64
#define MAX_USERNAME_LEN 32

/* Capacity defaults — override at compile time with -DMAX_BOOKS=N */
#ifndef MAX_BOOKS
#  define MAX_BOOKS    64
#endif

#ifndef MAX_BORROWED
#  define MAX_BORROWED  4
#endif

/* Return codes */
#define LIB_OK    0
#define LIB_ERR  (-1)

/*
 * Book — a single catalogue entry.
 * is_borrowed: 0 = available, 1 = currently borrowed.
 */
typedef struct {
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    int  is_borrowed;
} Book;

/*
 * User — an active borrowing session.
 * borrowed[] holds pointers into the library's book_list array,
 * so mutations are reflected in the catalogue immediately.
 */
typedef struct {
    char  username[MAX_USERNAME_LEN];
    Book *borrowed[MAX_BORROWED];
    int   num_borrowed;
} User;

/*
 * Library — top-level state container.
 * book_list is heap-allocated by library_init() and freed by library_free().
 */
typedef struct {
    Book *book_list;    /* heap-allocated array of Book    */
    int   num_books;    /* current catalogue size           */
    int   max_books;    /* allocated capacity               */
    User  the_user;     /* active user session              */
    int   max_borrowed; /* borrow limit per user            */
} Library;

#endif /* LIB_STRUCTURES_H */
