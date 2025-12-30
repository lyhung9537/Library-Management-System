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

/* �H�e�����z�� enum �s�b���N�q�z�ڰڰ� */
// �ϥΪ̿��ﶵ
enum UserMenu
{
    User_Search = 1,  // �j�M���y
    User_List,        // �C�X�Ҧ����y
    User_Popularity,  // �H��Ʀ�]
    User_Add,         // �s�W���y
    User_CheckOut,    // �ɮ�
    User_ReturnBook,  // �ٮ�
    User_CheckRecord, // �d�ݭɾ\����
    User_Logout,      // �n�X
    User_Exit         // ���}
};

// �X�ȿ��ﶵ
enum GuestMenu
{
    Guest_Search = 1,    // �j�M���y
    Guest_List,          // �C�X�Ҧ����y
    Guest_Popularity,    // �H��Ʀ�]
    Guest_Login,         // �n�J
    Guest_CreateAccount, // �Ыرb��
    Guest_Exit           // ���}
};

// �M�����
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ���ݨϥΪ̫��U Enter �A�����e��
void waitforUser()
{
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

/* �D��� */
void WelcomingMessage();  // �w��y
void MainMenu_Loggedin(); // �n�J�᪺�D���
void MainMenu_Guest();    // �X��(���n�J)�D���

/* �\�� */
void addBook();                                     // �s�W���y
void searchBook();                                  // �j�M���y
void checkoutBook();                                // �ɮ�
void returnBook();                                  // �ٮ�
void showRecord();                                  // ��ܭɾ\����
void listBooks();                                   // �C�X�Ҧ����y
void showRanking();                                 // ��ܤH��Ʀ�]
void loginAccount();                                // �n�J�b��
void loginPassword(string &password, Reader &user); // ��J�K�X
void createAccount();                               // �Ыرb��
void createPassword(string &password);              // �]�w�K�X
void logoutAccount();                               // �n�X

/* ����t�� */
void randomDay(); // �H�����ͤ��Ѥ��
void newDay();    // ����[�@��
int countDay = 0; // �p��Ѽ�

/* �t�θ�� */
vector<Book> Collection; // �]��
vector<Reader> Readers;  // Ū�̲M��
Date today(2025, 0, 0);  // ���Ѫ����

/* �ϥΪ̨t�� */
Reader guest;          // �w�]�X��
Reader *User = &guest; // ���e�ϥΪ̫���

int main()
{
    // ��l�Ƥ��Ѥ��
    srand(time(0));
    randomDay();

    int action;
    while (1)
    {
        // �̷Ө�����ܥD���
        WelcomingMessage();
        if (User->isGuest())
            MainMenu_Guest();
        else
            MainMenu_Loggedin();

        cin >> action;

        if (User->isGuest())
        {
            // �X��
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
            // �w�n�J
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

        waitforUser(); // ���ݨϥΪ�
        clearScreen(); // �M�ſ�X

        countDay++;
        if (countDay == 3)
        {
            newDay(); // �C�T���ާ@���+1
            countDay = 0;
        }
    }
}

// �w��T���P������
void WelcomingMessage()
{
    today.printDate();
    User->helloReader();
}

// �w�n�J�D���
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

// �X�ȥD���
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

// �s�W���y
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
    // �W�[���ؼƶq
    // �C���T���i�����@���H�W����
    User->increaseAdded();
    if (User->donated() >= 3 && User->records() > 0)
        User->decreaseViolation();

    Book book(title, author, year);
    vector<Book>::iterator it = func::find(Collection, book);
    if (it != Collection.end())
    {
        // �w�����ѫh�W�[����
        it->increaseCopies();
        return;
    }
    Collection.push_back(book); // �s�ѥ[�J����
    func::sort(Collection);     // ���s�Ƨ�
}

// �j�M���y
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

// �ɮ�
void checkoutBook()
{
    if (User->isSuspended())
    {
        // �T��Q���v�Τ�ɮ�
        cout << "Your account has been suspended due to repeated infractions.\n";
        return;
    }
    if (User->isReading())
    {
        // �T��Τ�@���ɦh����
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
        it->decreaseCopies();               // ��֥i�ɥ���
        it->increaseLent();                 // �W�[�ɥX����
        Date returnDate = today.addDay(14); // �]�w�ٮѤ��
        User->borrowBook(book, returnDate); // �O���ɮ�
        cout << "Book checked out successfully.\n";
        cout << "Please return the book before " << returnDate << endl;
    }
}

// �ٮ�
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
    it->increaseCopies(); // �ٮѼW�[����
    User->returnBook();   // �]�w�w�ٮ�
    cout << "Book returned successfully.\n";

    if (User->isLate(today))
    {
        // �O���k�١A�W�[�H�W����
        User->increaseViolation();
        cout << "\nYour returned item is overdue.\n"
             << "Your account has recorded " << User->records() << " violations.\n"
             << "If your overdue returns exceed 10 times, your account will be permanently suspended.\n"
             << "Each set of three donated books grants the removal of one violation record.\n";
    }
}

// ��ܭɾ\����
void showRecord()
{
    User->printRecords();
}

// �C�X�]��
void listBooks()
{
    cout << "\n<======= Collections =======>\n";
    for (auto book : Collection)
        book.printBook();
    cout << "<===========================>\n";
}

// ��ܤH��Ʀ�]
void showRanking()
{
    // ���̤H��Ƨ�
    // �A�ƧǦ^�쥻����
    func::sortPopularity(Collection);
    listBooks();
    func::sort(Collection);
}

// �n�J�b��
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
    loginPassword(password, *it); // ��J�K�X
}

// ��J�K�X�]������ܡ^�]�W�Ū���a�^
void loginPassword(string &password, Reader &user)
{
    while (true)
    {
        char temp[30] = {0}, ch;
        cout << "Enter password: ";
        int i = 0;
        while (i < 29)
        {
            ch = getch(); // getch()���|�N��J�����e���
            if (ch == '\r')
                break;
            if (ch == 8)
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

// �Ыرb��
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
    createPassword(password); // �]�w�K�X

    Reader newReader(ID, password);
    Readers.push_back(newReader);

    cout << "\nAccount successfully created.\n";
}

// �]�w�K�X�]�⦸��J�T�{�^
void createPassword(string &password)
{
    while (true)
    {
        // �Ĥ@����J�K�X
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

        // �T�{�K�X
        char temp2[30] = {0};
        cout << "\nEnter password again: ";
        int j = 0;
        while (j < 29)
        {
            ch = getch();
            if (ch == '\r')
                break;
            if (ch == 8)
            {
                if (j > 0)
                {
                    j--;
                    temp2[j] = '\0';
                    cout << "\b \b";
                }
            }
            else
            {
                temp2[j++] = ch;
                putchar('*');
            }
        }
        temp2[j] = '\0';

        // ����⦸�K�X
        string s1 = temp;
        string s2 = temp2;
        if (s1 == s2)
        {
            password = temp;
            break;
        }
        else
            cout << "Passwords do not match. Please try again.\n";
    }
}

// �n�X�b��
void logoutAccount()
{
    char confirm;
    cout << "Are you sure you want to log out? [y/n]: ";
    cin.ignore();
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {
        User = &guest;
        cout << "Logout successful.\n";
        return;
    }
    else if (confirm == 'n' || confirm == 'N')
    {
        cout << "Returning to Main Menu.\n";
        return;
    }
    else
    {
        cout << "Invalid input. Please enter 'y' or 'n'.\n";
    }
}

// �H�����ͤ��Ѥ��
void randomDay()
{
    int year = 2025;
    int month = rand() % 12 + 1;
    Date temp(year, month, 1);
    int days = temp.daysofthemonth();
    int day = rand() % days + 1;
    today = Date(year, month, day);
}

// ����[�@��
void newDay()
{
    today = today.addDay(1);
}
