#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "user.h"
#include "utility.h"

void user_list_available(const Book *book_list, int num_books) {
    int i, found = 0;
    for (i = 0; i < num_books; i++) {
        if (!book_list[i].is_borrowed) {
            printf("  [%2d] %-40s  %s\n", i, book_list[i].author,
                   book_list[i].title);
            found++;
        }
    }
    if (!found)
        printf("  (no books available)\n");
}

void user_list_borrowed(const User *user) {
    int i;
    if (user->num_borrowed == 0) {
        printf("  (you have not borrowed any books)\n");
        return;
    }
    for (i = 0; i < user->num_borrowed; i++) {
        printf("  [%d] %-40s  %s\n", i, user->borrowed[i]->author,
               user->borrowed[i]->title);
    }
}

int user_do_borrow(User *user, Book *book_list, int num_books, int max_borrowed,
                   int book_idx) {
    if (user->num_borrowed >= max_borrowed) {
        printf("Error: return a book before borrowing another.\n");
        return LIB_ERR;
    }
    if (book_idx < 0 || book_idx >= num_books) {
        printf("Error: invalid selection.\n");
        return LIB_ERR;
    }
    if (book_list[book_idx].is_borrowed) {
        printf("Error: that book is not available.\n");
        return LIB_ERR;
    }
    book_list[book_idx].is_borrowed    = 1;
    user->borrowed[user->num_borrowed] = &book_list[book_idx];
    user->num_borrowed++;
    return LIB_OK;
}

int user_do_return(User *user, int borrow_idx) {
    int i;
    if (user->num_borrowed == 0) {
        printf("Error: you have not borrowed any books.\n");
        return LIB_ERR;
    }
    if (borrow_idx < 0 || borrow_idx >= user->num_borrowed) {
        printf("Error: invalid selection.\n");
        return LIB_ERR;
    }
    user->borrowed[borrow_idx]->is_borrowed = 0;

    /* Compact the borrowed array */
    for (i = borrow_idx; i < user->num_borrowed - 1; i++)
        user->borrowed[i] = user->borrowed[i + 1];
    user->borrowed[user->num_borrowed - 1] = NULL;
    user->num_borrowed--;
    return LIB_OK;
}

int user_borrow_book(User *user, Book *book_list, int num_books,
                     int max_borrowed) {
    int idx;
    user_list_available(book_list, num_books);
    idx = util_read_index("  Enter book number to borrow: ");
    if (idx < 0) {
        printf("Error: invalid input.\n");
        return LIB_ERR;
    }
    if (user_do_borrow(user, book_list, num_books, max_borrowed, idx) ==
        LIB_OK) {
        printf("  Borrowed: %s – %s\n", book_list[idx].author,
               book_list[idx].title);
        return LIB_OK;
    }
    return LIB_ERR;
}

int user_return_book(User *user) {
    int idx;
    user_list_borrowed(user);
    idx = util_read_index("  Enter book number to return: ");
    if (idx < 0) {
        printf("Error: invalid input.\n");
        return LIB_ERR;
    }
    if (idx >= user->num_borrowed) {
        printf("Error: invalid input.\n");
        return LIB_ERR;
    }

    /* Cache the name before user_do_return clears the slot. */
    const char *author = user->borrowed[idx]->author;
    const char *title  = user->borrowed[idx]->title;
    if (user_do_return(user, idx) == LIB_OK) {
        printf("  Returned: %s – %s\n", author, title);
        return LIB_OK;
    }
    return LIB_ERR;
}

void user_cli(Library *lib) {
    int running = 1;

    while (running) {
        printf("\n--- User Menu ---\n");
        printf("  1  Available books\n");
        printf("  2  Borrow a book\n");
        printf("  3  Return a book\n");
        printf("  4  My borrowed books\n");
        printf("  5  Log out\n");
        printf("Choice: ");
        int choice = util_read_option();

        switch (choice) {
        case 1:
            printf("\n[Available books]\n");
            user_list_available(lib->book_list, lib->num_books);
            break;
        case 2:
            printf("\n[Borrow a book]\n");
            user_borrow_book(&lib->the_user, lib->book_list, lib->num_books,
                             lib->max_borrowed);
            break;
        case 3:
            printf("\n[Return a book]\n");
            user_return_book(&lib->the_user);
            break;
        case 4:
            printf("\n[My borrowed books]\n");
            user_list_borrowed(&lib->the_user);
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
