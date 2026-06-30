#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "librarian.h"
#include "structures.h"
#include "utility.h"

void librarian_list_all(const Book *book_list, int num_books) {
    int i;
    if (num_books == 0) {
        printf("  (catalogue is empty)\n");
        return;
    }
    for (i = 0; i < num_books; i++) {
        printf("  [%2d] %-40s  %s\n", i, book_list[i].author,
               book_list[i].title);
    }
}

void librarian_list_borrowed(const Book *book_list, int num_books) {
    int i, found = 0;
    for (i = 0; i < num_books; i++) {
        if (book_list[i].is_borrowed) {
            printf("  [%2d] %-40s  %s\n", i, book_list[i].author,
                   book_list[i].title);
            found++;
        }
    }
    if (!found)
        printf("  (no books currently borrowed)\n");
}

int librarian_do_add(Library *lib, const char *author, const char *title) {
    Book *b;
    if (lib->num_books >= lib->max_books) {
        printf("Error: catalogue is full (%d books).\n", lib->max_books);
        return LIB_ERR;
    }
    b = &lib->book_list[lib->num_books];
    memset(b, 0, sizeof(Book));
    strncpy(b->author, author, MAX_AUTHOR_LEN - 1);
    strncpy(b->title, title, MAX_TITLE_LEN - 1);
    b->is_borrowed = 0;
    lib->num_books++;
    return LIB_OK;
}

int librarian_do_remove(Library *lib, int idx) {
    int i;
    if (lib->num_books == 0) {
        printf("Error: catalogue is empty.\n");
        return LIB_ERR;
    }
    if (idx < 0 || idx >= lib->num_books) {
        printf("Error: invalid selection.\n");
        return LIB_ERR;
    }
    if (lib->book_list[idx].is_borrowed) {
        printf("Error: cannot remove a book that is currently borrowed.\n");
        return LIB_ERR;
    }
    /* Shift remaining entries down to fill the gap */
    for (i = idx; i < lib->num_books - 1; i++)
        lib->book_list[i] = lib->book_list[i + 1];
    lib->num_books--;
    return LIB_OK;
}

int librarian_add_book(Library *lib) {
    char author[MAX_AUTHOR_LEN];
    char title[MAX_TITLE_LEN];

    printf("  Author: ");
    fflush(stdout);
    if (fgets(author, (int)sizeof(author), stdin) == NULL)
        return LIB_ERR;
    util_trim_newline(author);

    printf("  Title:  ");
    fflush(stdout);
    if (fgets(title, (int)sizeof(title), stdin) == NULL)
        return LIB_ERR;
    util_trim_newline(title);

    if (librarian_do_add(lib, author, title) == LIB_OK) {
        printf("  Added: %s – %s\n", author, title);
        return LIB_OK;
    }
    return LIB_ERR;
}

int librarian_remove_book(Library *lib) {
    int idx;
    if (lib->num_books == 0) {
        printf("  (catalogue is empty)\n");
        return LIB_ERR;
    }
    librarian_list_all(lib->book_list, lib->num_books);
    idx = util_read_index("  Enter number to remove: ");
    return librarian_do_remove(lib, idx);
}

void librarian_cli(Library *lib) {
    int running = 1;

    while (running) {
        printf("\n--- Librarian Menu ---\n");
        printf("  1  List all books\n");
        printf("  2  List borrowed books\n");
        printf("  3  Add book\n");
        printf("  4  Remove book\n");
        printf("  5  Log out\n");
        printf("Choice: ");
        int choice = util_read_option();

        switch (choice) {
        case 1:
            printf("\n[All books – %d total]\n", lib->num_books);
            librarian_list_all(lib->book_list, lib->num_books);
            break;
        case 2:
            printf("\n[Borrowed books]\n");
            librarian_list_borrowed(lib->book_list, lib->num_books);
            break;
        case 3:
            printf("\n[Add book]\n");
            librarian_add_book(lib);
            break;
        case 4:
            printf("\n[Remove book]\n");
            librarian_remove_book(lib);
            break;
        case 5:
            running = 0;
            printf("Logged out.\n");
            break;
        default:
            printf("Invalid option.\n");
            break;
        }
    }
}
