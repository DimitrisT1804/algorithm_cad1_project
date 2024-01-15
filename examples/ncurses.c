#include <ncurses.h>

int main() {
    // Initialize ncurses
    initscr();
    cbreak();  // Line buffering disabled
    keypad(stdscr, TRUE);  // Enable special keys

    // Set up colors
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);

    // Print a simple menu
    printw("Select an option:\n");
    printw("1. Option 1\n");
    printw("2. Option 2\n");
    printw("3. Option 3\n");

    int choice;
    int highlight = 1;

    // Display the menu and handle user input
    while (1) {
        // Highlight the selected option
        for (int i = 1; i <= 3; i++) {
            if (i == highlight) {
                attron(COLOR_PAIR(i));
                printw("=> ");
            } else {
                attroff(COLOR_PAIR(i));
                printw("   ");
            }
            printw("Option %d\n", i);
        }

        // Get user input
        choice = getch();

        // Process user input
        switch (choice) {
            case KEY_UP:
                highlight--;
                if (highlight < 1) {
                    highlight = 3;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight > 3) {
                    highlight = 1;
                }
                break;
            case 10:  // Enter key
                // User pressed Enter, perform action based on the selected option
                clear();  // Clear the screen
                printw("You selected Option %d\n", highlight);
                refresh();
                getch();  // Wait for user input
                clear();  // Clear the screen for the next iteration
                break;
        }
    }

    // End ncurses
    endwin();

    return 0;
}
