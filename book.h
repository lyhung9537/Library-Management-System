/* 書籍資料 */
#pragma once
#include <iostream>
using namespace std;

class Book
{
    string title;  // 書名
    string author; // 作者
    int year;      // 出版年份
    int copies;    // 可借閱本數
    int lent;      // 借出次數(人氣)

public:
    Book() {}
    Book(string a, string b, int c) : title(a), author(b), year(c), copies(1), lent(0) {}
    bool operator<(const Book &other) const;  // 書本排序（書名、作者、年份）
    bool operator>(const Book &other) const;  // 書本人氣排序（借出次數、書名、作者、年份）
    bool operator==(const Book &other) const; // 書本相等判斷（書名、作者、年份）

    /* 判斷書名、作者、出版年份是否相同 */
    bool sameTitle(const string &title) const;
    bool sameAuthor(const string &author) const;
    bool sameYear(const int year) const;

    bool checkedout() const; // 是否已全數借出
    void printBook() const;  // 完整輸出書籍資訊
    void printBook2() const; // 簡化輸出書籍資訊
    void increaseCopies();   // 增加可藉本數(歸還或捐贈)
    void decreaseCopies();   // 減少可藉本數(借出)
    void increaseLent();     // 增加出借次數
};

/* 書本排序（書名、作者、年份）*/
bool Book::operator<(const Book &other) const
{
    // 先比書名，再比作者，最後比年份
    if (this->title != other.title)
        return this->title < other.title;
    if (this->author != other.author)
        return this->author < other.author;
    return this->year < other.year;
}

/* 書本人氣排序（借出次數多的排前面，若相同則比書名、作者、年份） */
bool Book::operator>(const Book &other) const
{
    // 先比借出次數（人氣）（降序），再比書名、作者、年份（升序）
    if (this->lent != other.lent)
        return this->lent > other.lent;
    if (this->title != other.title)
        return this->title < other.title;
    if (this->author != other.author)
        return this->author < other.author;
    return this->year < other.year;
}

/* 判斷是否為同本書（書名、作者、年份都相同） */
bool Book::operator==(const Book &other) const
{
    if (this->title != other.title)
        return false;
    if (this->author != other.author)
        return false;
    if (this->year != other.year)
        return false;
    return true;
}

// 書名是否相同
bool Book::sameTitle(const string &title) const
{
    return this->title == title;
}

// 作者是否相同
bool Book::sameAuthor(const string &author) const
{
    return this->author == author;
}

// 年份是否相同
bool Book::sameYear(const int year) const
{
    return this->year == year;
}

// 是否已全數借出
bool Book::checkedout() const
{
    return this->copies == 0;
}

// 完整輸出書籍資訊
void Book::printBook() const
{
    cout << "Title: " << this->title
         << "\nAuthor: " << this->author
         << "\nPublish year: " << this->year
         << "\nTimes borrowed: " << this->lent
         << "\nCopies available: " << this->copies << endl
         << endl;
}

// 簡化輸出書籍資訊
void Book::printBook2() const
{
    cout << "Title: " << this->title
         << "\nAuthor: " << this->author
         << "\nPublish year: " << this->year;
}

// 增加可藉本數(歸還或捐贈)
void Book::increaseCopies()
{
    this->copies++;
}

// 減少可藉本數(借出)
void Book::decreaseCopies()
{
    this->copies--;
}

// 增加出借次數
void Book::increaseLent()
{
    this->lent++;
}