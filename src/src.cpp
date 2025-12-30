#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <string>
#include <conio.h>
#include "book.h"
#include "algo.h"
#include "reader.h"
#include "date.h"
using namespace std;

/* I used to never understand the point of enums, lol */
// User menu options
enum UserMenu
{
    User_Search = 1,  // Search for books
    User_List,        // List all books
    User_Popularity,  // Popularity ranking
    User_Add,         // Add a book
    User_CheckOut,    // Borrow a book
    User_ReturnBook,  // Return a book
    User_CheckRecord, // View borrowing records
    User_Logout,      // Log out
    User_Exit         // Exit
};

// Guest menu options
enum GuestMenu
{
    Guest_Search = 1,    // Search for books
    Guest_List,          // List all books
    Guest_Popularity,    // Popularity ranking
    Guest_Login,         // Log in
    Guest_CreateAccount, // Create account
    Guest_Exit           // Exit
};

// Clear the screen display
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Wait for the user to press Enter before switching screens
void waitforUser()
{
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

/* Main Menus */
void WelcomingMessage();  // Welcome message
void MainMenu_Loggedin(); // Main menu for logged-in users
void MainMenu_Guest();    // Main menu for guests (not logged in)

/* Functions */
void addBook();                                     // Add a book
void searchBook();                                  // Search for books
void checkoutBook();                                // Borrow a book
void returnBook();                                  // Return a book
void showRecord();                                  // Show borrowing records
void listBooks();                                   // List all books
void showRanking();                                 // Show popularity ranking
void loginAccount();                                // Log in to account
void loginPassword(string &password, Reader &user); // Enter password
void createAccount();                               // Create an account
void createPassword(string &password);              // Set password
void logoutAccount();                               // Log out

/* Date System */
void randomDay(); // Randomly generate today's date
void newDay();    // Advance date by one day
int countDay = 0; // Operation counter to track day progression

/* System Data */
vector<Book> Collection; // Library collection
vector<Reader> Readers;  // List of readers
Date today(2025, 0, 0);  // Today's date

/* User System */
Reader guest;          // Default guest user
Reader *User = &guest; // Pointer to the current user

int main()
{
    // Initialize today's date
    srand(time(0));
    randomDay();

    int action;
    while (1)
    {
        // Display main menu based on user status
        WelcomingMessage();
        if (User->isGuest())
            MainMenu_Guest();
        else
            MainMenu_Loggedin();

        cin >> action;

        if (User->isGuest())
        {
            // Guest actions
            switch (action)
            {
            case Guest_Search:
                searchBook();
                break;
            case Guest_List:
                listBooks();
                break;
            case Guest_Popularity:
                showRanking();
                break;
            case Guest_Login:
                loginAccount();
                break;
            case Guest_CreateAccount:
                createAccount();
                break;
            case Guest_Exit:
                return 0;
            default:
                cout << "Invalid option.\n"
                     << "Please try again.\n";
                break;
            }
        }
        else
        {
            // Logged-in user actions
            switch (action)
            {
            case User_Add:
                addBook();
                break;
            case User_Search:
                searchBook();
                break;
            case User_CheckOut:
                checkoutBook();
                break;
            case User_ReturnBook:
                returnBook();
                break;
            case User_List:
                listBooks();
                break;
            case User_Popularity:
                showRanking();
                break;
            case User_CheckRecord:
                showRecord();
                break;
            case User_Logout:
                logoutAccount();
                break;
            case User_Exit:
                return 0;
            default:
                cout << "Invalid option.\n"
                     << "Please try again.\n";
                break;
            }
        }

        waitforUser(); // Wait for user
        clearScreen(); // Clear the output

        countDay++;
        if (countDay == 3)
        {
            newDay(); // Advance date by one day every three operations
            countDay = 0;
        }
    }
}

// Welcome message and today's date
void WelcomingMessage()
{
    today.printDate();
    User->helloReader();
}

// Main menu for logged-in users
void MainMenu_Loggedin()
{
    cout << "Library Management System\n"
         << User_Search << ". Search for a book\n"
         << User_List << ". List all books\n"
         << User_Popularity << ". Popularity ranking list\n"
         << User_Add << ". Add a new book\n"
         << User_CheckOut << ". Check out a book\n"
         << User_ReturnBook << ". Return a book\n"
         << User_CheckRecord << ". View borrowing history\n"
         << User_Logout << ". Log out\n"
         << User_Exit << ". Exit\n"
         << "Enter option: ";
}

// Main menu for guest users
void MainMenu_Guest()
{
    cout << "Library Management System\n"
         << Guest_Search << ". Search for a book\n"
         << Guest_List << ". List all books\n"
         << Guest_Popularity << ". Popularity ranking list\n"
         << Guest_Login << ". Log in account\n"
         << Guest_CreateAccount << ". Create account\n"
         << Guest_Exit << ". Exit\n"
         << "Enter option: ";
}

// Add a new book
void addBook()
{
    string title, author;
    int year;
    cout << "Enter title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);
    cout << "Enter publish year: ";
    cin >> year;

    cout << "Thank you for your donation! ^ ^\n";
    // Increase donation count
    // Every three books donated can remove one violation record
    User->increaseAdded();
    if (User->donated() >= 3 && User->records() > 0)
        User->decreaseViolation();

    Book book(title, author, year);
    vector<Book>::iterator it = func::find(Collection, book);
    if (it != Collection.end())
    {
        // If book already exists, increase the number of copies
        it->increaseCopies();
        return;
    }
    Collection.push_back(book); // Add new book to collection
    func::sort(Collection);     // Re-sort collection
}

// Search for books
void searchBook()
{
    int option;
    cout << "Searching by [1. title]    [2. Author]    [3. Publish year]\n"
         << "Enter option: ";
    cin >> option;
    cin.ignore();

    bool found = false;
    if (option == 1)
    {
        string input;
        cout << "Enter keyword: ";
        getline(cin, input);
        for (auto x : Collection)
        {
            if (x.sameTitle(input))
            {
                x.printBook();
                found = true;
            }
        }
    }
    else if (option == 2)
    {
        string input;
        cout << "Enter keyword: ";
        getline(cin, input);
        for (auto x : Collection)
        {
            if (x.sameAuthor(input))
            {
                x.printBook();
                found = true;
            }
        }
    }
    else
    {
        int input;
        cout << "Enter keyword: ";
        cin >> input;
        for (auto x : Collection)
        {
            if (x.sameYear(input))
            {
                x.printBook();
                found = true;
            }
        }
    }
    if (!found)
        cout << "No matching result.\n";
}

// Borrow a book
void checkoutBook()
{
    if (User->isSuspended())
    {
        // Prohibit suspended users from borrowing books
        cout << "Your account has been suspended due to repeated infractions.\n";
        return;
    }
    if (User->isReading())
    {
        // Prohibit users from borrowing multiple books at once
        cout << "Please return the book before you borrow a new one.\n";
        return;
    }

    string title, author;
    int year;
    cout << "Enter title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter the author: ";
    getline(cin, author);
    cout << "Enter the year of publish: ";
    cin >> year;
    Book book(title, author, year);

    vector<Book>::iterator it = func::find(Collection, book);
    if (it == Collection.end())
        cout << "Sorry, the book does not exist.\n";
    else if (it->checkedout())
        cout << "Sorry, the book is currently checked out.\n";
    else
    {
        it->decreaseCopies();               // Decrease number of available copies
        it->increaseLent();                 // Increase the count of times lent
        Date returnDate = today.addDay(14); // Set the return date (14 days from now)
        User->borrowBook(book, returnDate); // Record borrowing
        cout << "Book checked out successfully.\n";
        cout << "Please return the book before " << returnDate << endl;
    }
}

// Return a book
void returnBook()
{
    string title, author;
    int year;
    cout << "Enter title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter the author: ";
    getline(cin, author);
    cout << "Enter the year of publish: ";
    cin >> year;
    Book book(title, author, year);

    vector<Book>::iterator it = func::find(Collection, book);
    if (it == Collection.end())
    {
        cout << "Sorry, the book does not exist in the collection.\n";
        return;
    }
    it->increaseCopies(); // Increment copies upon return
    User->returnBook();   // Mark as returned in user records
    cout << "Book returned successfully.\n";

    if (User->isLate(today))
    {
        // Overdue return: increase violation count
        User->increaseViolation();
        cout << "\nYour returned item is overdue.\n"
             << "Your account has recorded " << User->records() << " violations.\n"
             << "If your overdue returns exceed 10 times, your account will be permanently suspended.\n"
             << "Each set of three donated books grants the removal of one violation record.\n";
    }
}

// Show borrowing records
void showRecord()
{
    User->printRecords();
}

// List all books in collection
void listBooks()
{
    cout << "\n<======= Collections =======>\n";
    for (auto book : Collection)
        book.printBook();
    cout << "<===========================>\n";
}

// Show popularity ranking
void showRanking()
{
    // Sort by popularity first
    // Then list, then sort back to original order
    func::sortPopularity(Collection);
    listBooks();
    func::sort(Collection);
}

// Log in to an account
void loginAccount()
{
    string ID;
    cout << "Enter user ID: ";
    cin >> ID;
    auto it = Readers.begin();
    for (; it != Readers.end(); ++it)
    {
        if (it->sameID(ID))
            break;
    }
    if (it == Readers.end())
    {
        cout << "User does not exist.\n";
        return;
    }

    string password;
    loginPassword(password, *it); // Password input process
}

// Enter password (hidden display) (pretty cool, right?)
void loginPassword(string &password, Reader &user)
{
    while (true)
    {
        char temp[30] = {0}, ch;
        cout << "Enter password: ";
        int i = 0;
        while (i < 29)
        {
            ch = getch(); // getch() does not echo the input character
            if (ch == '\r')
                break;
            if (ch == 8) // Backspace logic
            {
                if (i > 0)
                {
                    i--;
                    temp[i] = '\0';
                    cout << "\b \b";
                }
            }
            else
            {
                temp[i++] = ch;
                putchar('*');
            }
        }
        temp[i] = '\0';
        password = temp;
        cout << endl;

        if (user.samePassword(password))
        {
            cout << "Login successful.\n";
            user.helloReader();
            User = &user;
            break;
        }
        else
        {
            cout << "Wrong password. Please try again.\n";
        }
    }
}

// Create an account
void createAccount()
{
    string ID;
    cout << "Enter user ID: ";
    cin >> ID;
    for (auto r : Readers)
    {
        if (r.sameID(ID))
        {
            cout << "User already exists.\n";
            return;
        }
    }

    string password;
    createPassword(password); // Set password

    Reader newReader(ID, password);
    Readers.push_back(newReader);

    cout << "\nAccount successfully created.\n";
}

// Set password (with double input confirmation)
void createPassword(string &password)
{
    while (true)
    {
        // First password entry
        char temp[30] = {0}, ch;
        cout << "Enter password(less than 30 characters): ";
        int i = 0;
        while (i < 29)
        {
            ch = getch();
            if (ch == '\r')
                break;
            if (ch == 8)
            {
                if (i > 0)
                {
                    i--;
                    temp[i]
