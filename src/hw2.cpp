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

/* 以前都不理解 enum 存在的意義哇啊啊啊 */
// 使用者選單選項
enum UserMenu
{
    User_Search = 1,  // 搜尋書籍
    User_List,        // 列出所有書籍
    User_Popularity,  // 人氣排行榜
    User_Add,         // 新增書籍
    User_CheckOut,    // 借書
    User_ReturnBook,  // 還書
    User_CheckRecord, // 查看借閱紀錄
    User_Logout,      // 登出
    User_Exit         // 離開
};

// 訪客選單選項
enum GuestMenu
{
    Guest_Search = 1,    // 搜尋書籍
    Guest_List,          // 列出所有書籍
    Guest_Popularity,    // 人氣排行榜
    Guest_Login,         // 登入
    Guest_CreateAccount, // 創建帳號
    Guest_Exit           // 離開
};

// 清空顯示
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 等待使用者按下 Enter 再切換畫面
void waitforUser()
{
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

/* 主選單 */
void WelcomingMessage();  // 歡迎語
void MainMenu_Loggedin(); // 登入後的主選單
void MainMenu_Guest();    // 訪客(未登入)主選單

/* 功能 */
void addBook();                                     // 新增書籍
void searchBook();                                  // 搜尋書籍
void checkoutBook();                                // 借書
void returnBook();                                  // 還書
void showRecord();                                  // 顯示借閱紀錄
void listBooks();                                   // 列出所有書籍
void showRanking();                                 // 顯示人氣排行榜
void loginAccount();                                // 登入帳號
void loginPassword(string &password, Reader &user); // 輸入密碼
void createAccount();                               // 創建帳號
void createPassword(string &password);              // 設定密碼
void logoutAccount();                               // 登出

/* 日期系統 */
void randomDay(); // 隨機產生今天日期
void newDay();    // 日期加一天
int countDay = 0; // 計算天數

/* 系統資料 */
vector<Book> Collection; // 館藏
vector<Reader> Readers;  // 讀者清單
Date today(2025, 0, 0);  // 今天的日期

/* 使用者系統 */
Reader guest;          // 預設訪客
Reader *User = &guest; // 當前使用者指標

int main()
{
    // 初始化今天日期
    srand(time(0));
    randomDay();

    int action;
    while (1)
    {
        // 依照身分顯示主選單
        WelcomingMessage();
        if (User->isGuest())
            MainMenu_Guest();
        else
            MainMenu_Loggedin();

        cin >> action;

        if (User->isGuest())
        {
            // 訪客
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
            // 已登入
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

        waitforUser(); // 等待使用者
        clearScreen(); // 清空輸出

        countDay++;
        if (countDay == 3)
        {
            newDay(); // 每三次操作日期+1
            countDay = 0;
        }
    }
}

// 歡迎訊息與今日日期
void WelcomingMessage()
{
    today.printDate();
    User->helloReader();
}

// 已登入主選單
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

// 訪客主選單
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

// 新增書籍
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
    // 增加捐贈數量
    // 每捐三本可消除一次違規紀錄
    User->increaseAdded();
    if (User->donated() >= 3 && User->records() > 0)
        User->decreaseViolation();

    Book book(title, author, year);
    vector<Book>::iterator it = func::find(Collection, book);
    if (it != Collection.end())
    {
        // 已有此書則增加本數
        it->increaseCopies();
        return;
    }
    Collection.push_back(book); // 新書加入收藏
    func::sort(Collection);     // 重新排序
}

// 搜尋書籍
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

// 借書
void checkoutBook()
{
    if (User->isSuspended())
    {
        // 禁止被停權用戶借書
        cout << "Your account has been suspended due to repeated infractions.\n";
        return;
    }
    if (User->isReading())
    {
        // 禁止用戶一次借多本書
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
        it->decreaseCopies();               // 減少可借本數
        it->increaseLent();                 // 增加借出次數
        Date returnDate = today.addDay(14); // 設定還書日期
        User->borrowBook(book, returnDate); // 記錄借書
        cout << "Book checked out successfully.\n";
        cout << "Please return the book before " << returnDate << endl;
    }
}

// 還書
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
    it->increaseCopies(); // 還書增加本數
    User->returnBook();   // 設定已還書
    cout << "Book returned successfully.\n";

    if (User->isLate(today))
    {
        // 逾期歸還，增加違規次數
        User->increaseViolation();
        cout << "\nYour returned item is overdue.\n"
             << "Your account has recorded " << User->records() << " violations.\n"
             << "If your overdue returns exceed 10 times, your account will be permanently suspended.\n"
             << "Each set of three donated books grants the removal of one violation record.\n";
    }
}

// 顯示借閱紀錄
void showRecord()
{
    User->printRecords();
}

// 列出館藏
void listBooks()
{
    cout << "\n<======= Collections =======>\n";
    for (auto book : Collection)
        book.printBook();
    cout << "<===========================>\n";
}

// 顯示人氣排行榜
void showRanking()
{
    // 先依人氣排序
    // 再排序回原本順序
    func::sortPopularity(Collection);
    listBooks();
    func::sort(Collection);
}

// 登入帳號
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
    loginPassword(password, *it); // 輸入密碼
}

// 輸入密碼（隱藏顯示）（超酷的對吧）
void loginPassword(string &password, Reader &user)
{
    while (true)
    {
        char temp[30] = {0}, ch;
        cout << "Enter password: ";
        int i = 0;
        while (i < 29)
        {
            ch = getch(); // getch()不會將輸入的內容顯示
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

// 創建帳號
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
    createPassword(password); // 設定密碼

    Reader newReader(ID, password);
    Readers.push_back(newReader);

    cout << "\nAccount successfully created.\n";
}

// 設定密碼（兩次輸入確認）
void createPassword(string &password)
{
    while (true)
    {
        // 第一次輸入密碼
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

        // 確認密碼
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

        // 比較兩次密碼
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

// 登出帳號
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

// 隨機產生今天日期
void randomDay()
{
    int year = 2025;
    int month = rand() % 12 + 1;
    Date temp(year, month, 1);
    int days = temp.daysofthemonth();
    int day = rand() % days + 1;
    today = Date(year, month, day);
}

// 日期加一天
void newDay()
{
    today = today.addDay(1);
}