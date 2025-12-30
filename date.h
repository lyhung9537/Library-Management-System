/* 日期系統 */
#pragma once
#include <iostream>
#include <vector>
#include "algo.h"
using namespace std;

class Date
{
private:
    int year, month, day;

public:
    Date() {}
    Date(int a, int b, int c) : year(a), month(b), day(c) {}

    Date addDay(int day);     // 增加天數，回傳新日期
    Date addMonth(int month); // 增加月數，回傳新日期

    bool operator>(const Date &other) const; // 比較日期先後

    void printDate() const;        // 輸出日期
    int daysofthemonth() const;    // 回傳當月天數
    bool leapyear(int year) const; // 判斷潤平年

    friend ostream &operator<<(ostream &os, const Date &date);
};

/* 輸出運算子重載，酷東西 */
ostream &operator<<(ostream &os, const Date &date)
{
    os << date.year << " / " << date.month << " / " << date.day;
    return os;
}

/* 增加天數，需要時會進位 */
Date Date::addDay(int day)
{
    int y = this->year, m = this->month, d = this->day + day;
    while (true)
    {
        int days = Date(y, m, 1).daysofthemonth();
        if (d > days)
        {
            d -= days;
            m++;
            if (m > 12)
            {
                m = 1;
                y++;
            }
        }
        else
            break;
    }
    return Date(y, m, d);
}

/* 增加月數，需要時會進位 */
Date Date::addMonth(int month)
{
    int y = this->year;
    int m = this->month + month;
    int d = this->day;
    while (m > 12)
    {
        m -= 12;
        y++;
    }

    int days = Date(y, m, 1).daysofthemonth();
    if (d > days)
    {
        d -= days;
        m++;
    }
    return Date(y, m, d);
}

/* 比較日期先後 */
bool Date::operator>(const Date &other) const
{
    if (this->year != other.year)
        return this->year > other.year;
    if (this->month != other.month)
        return this->month > other.month;
    return this->day > other.day;
}

/* 輸出日期 */
void Date::printDate() const
{
    cout << "Date: " << this->year << " / " << this->month << " / " << this->day << "\n";
}

/* 依年份及月份，取得當月天數 */
int Date::daysofthemonth() const
{
    vector<int> month31 = {1, 3, 5, 7, 8, 10, 12};
    vector<int> month30 = {4, 6, 9, 11};
    if (func::find(month30, this->month) != month30.end())
        return 30;
    else if (func::find(month31, this->month) != month31.end())
        return 31;
    else
        return (leapyear(this->year)) ? 29 : 28;
}

/* 潤平年判斷 */
bool Date::leapyear(int year) const
{
    if (year % 400 == 0)
        return true;
    else if (year % 100 == 0)
        return false;
    else if (year % 4 == 0)
        return true;
    else
        return false;
}
