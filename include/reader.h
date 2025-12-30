/* 讀者資料 */
#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include "book.h"
#include "date.h"
using namespace std;

class Reader
{
private:
    string ID;                        // 讀者帳號
    string password;                  // 密碼
    vector<pair<Book, int>> borrowed; // 借閱過的書籍及每本書的借閱次數
    int added;                        // 捐贈書籍數量
    int violation;                    // 違規次數，超過 10 次會被停權
    Date returnDate;                  // 應還書日期
    bool returned = true;             // 是否已還書

public:
    Reader() : ID("Guest") {}
    Reader(string id, string pw) : ID(id), password(pw), added(0), violation(0) {}

    int records() const; // 取得違規次數
    int donated() const; // 取得捐贈書籍數量

    void printReader() const;                        // 輸出讀者資訊
    void printRecords() const;                       // 輸出借閱紀錄
    void helloReader() const;                        // 歡迎語
    void borrowBook(const Book &b, const Date &day); // 借書
    void returnBook();                               // 還書
    void increaseViolation();                        // 增加違規次數
    void decreaseViolation();                        // 減少違規次數
    void increaseAdded();                            // 增加捐贈書籍數量

    bool isSuspended() const;                        // 是否被停權
    bool isGuest() const;                            // 是否為訪客
    bool sameID(const string &ID) const;             // 帳號是否相同
    bool samePassword(const string &password) const; // 密碼是否相同
    bool isLate(const Date &day) const;              // 是否逾期
    bool isReading() const;                          // 是否有正在借閱的書
};

// 取得違規次數
int Reader::records() const
{
    return this->violation;
}

// 取得捐贈書籍數量
int Reader::donated() const
{
    return this->added;
}

// 輸出讀者資訊
void Reader::printReader() const
{
    cout << "Welcome to your personal study.\n"
         << "ID: " << this->ID
         << "\nBooks borrowed: " << this->borrowed.size()
         << "\nViolations: " << this->violation << endl
         << endl;
}

// 輸出借閱紀錄
void Reader::printRecords() const
{
    for (auto r : borrowed)
    {
        r.first.printBook2();
        cout << "\nTime borrowed by you: " << r.second << endl;
    }
}

// 歡迎語
void Reader::helloReader() const
{
    cout << "Hello, " << this->ID << ".\n";
}

// 借書，若已借過則次數+1，否則新增紀錄，並設定還書日期
void Reader::borrowBook(const Book &b, const Date &day)
{
    bool read = false;
    for (auto &r : borrowed)
    {
        if (r.first == b)
        {
            r.second++;
            read = true;
        }
    }
    if (!read)
        borrowed.push_back(make_pair(b, 1));

    this->returnDate = day;
    this->returned = false;
}

// 還書，將狀態設為已還
void Reader::returnBook()
{
    this->returned = true;
}

// 增加違規次數
void Reader::increaseViolation()
{
    this->violation++;
}

// 減少違規次數
void Reader::decreaseViolation()
{
    this->violation--;
}

// 增加捐贈書籍數量
void Reader::increaseAdded()
{
    this->added++;
}

// 是否被停權（違規次數大於10）
bool Reader::isSuspended() const
{
    return this->violation > 10;
}

// 是否為訪客
bool Reader::isGuest() const
{
    return this->ID == "Guest";
}

// 帳號是否相同
bool Reader::sameID(const string &ID) const
{
    return this->ID == ID;
}

// 密碼是否相同
bool Reader::samePassword(const string &password) const
{
    return this->password == password;
}

// 是否逾期（今天大於應還日期）
bool Reader::isLate(const Date &today) const
{
    return today > this->returnDate;
}

// 是否有正在借閱的書（尚未還書）
bool Reader::isReading() const
{
    return this->returned == false;
}