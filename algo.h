/* 搜尋和排序函式 */
#pragma once
#include <iostream>
#include <vector>
#include "book.h"
using namespace std;

/* 比較兩本書，使用 operator<，用於排序 */
bool cmp(Book &b1, Book &b2)
{
    return b1 < b2;
}

class func
{
public:
    static vector<Book>::iterator find(vector<Book> &v, Book b); // 在 Book 向量中尋找指定 Book，回傳 iterator
    static vector<int>::iterator find(vector<int> &v, int i);    // 在 int 向量中尋找指定 int，回傳 iterator
    static void sort(vector<Book> &v);                           // 以 operator< 對 Book 向量做快速排序
    static void sortPopularity(vector<Book> &v);                 // 以人氣高到低對 Book 向量做合併排序

private:
    static void sort_step(vector<Book> &v, int left_bound, int right_bound);          // 快速排序遞迴步驟
    static void mergePopularity(vector<Book> &v1, vector<Book> &v2, vector<Book> &v); // 快速排序遞迴步驟
};

/* quick sort */
void func::sort(vector<Book> &v)
{
    if (v.size() <= 1)
        return;
    sort_step(v, 0, v.size() - 1);
}

/* quick sort遞迴 */
void func::sort_step(vector<Book> &v, int left_bound, int right_bound)
{
    if (left_bound >= right_bound)
        return;

    Book pivot = v[right_bound];
    int i = left_bound - 1;
    for (int j = left_bound; j < right_bound; j++)
    {
        if (cmp(v[j], pivot))
        {
            i++;
            swap(v[i], v[j]);
        }
    }
    swap(v[i + 1], v[right_bound]);
    int pivot_index = i + 1;

    sort_step(v, left_bound, pivot_index - 1);
    sort_step(v, pivot_index + 1, right_bound);
}

/* 在 Book 向量中尋找指定的 Book，回傳 iterator */
vector<Book>::iterator func::find(vector<Book> &v, Book b)
{
    vector<Book>::iterator it;
    for (it = v.begin(); it != v.end(); it++)
    {
        if (*it == b)
            return it;
    }
    return v.end();
}

/* 在 int 向量中尋找指定的 int，回傳 iterator */
vector<int>::iterator func::find(vector<int> &v, int i)
{
    vector<int>::iterator it;
    for (it = v.begin(); it != v.end(); it++)
    {
        if (*it == i)
            return it;
    }
    return v.end();
}

/* 以人氣高到低對 Book 向量做merge sort */
void func::sortPopularity(vector<Book> &v)
{
    if (v.size() <= 1)
        return;

    vector<Book> v1, v2;
    for (int i = 0; i < v.size() / 2; i++)
        v1.push_back(v[i]);
    for (int i = v.size() / 2; i < v.size(); i++)
        v2.push_back(v[i]);
    v.clear();

    sortPopularity(v1);
    sortPopularity(v2);
    mergePopularity(v1, v2, v);
}

/* merge sort合併 */
void func::mergePopularity(vector<Book> &v1, vector<Book> &v2, vector<Book> &v)
{
    vector<Book>::iterator it1 = v1.begin(), it2 = v2.begin();
    while (it1 != v1.end() && it2 != v2.end())
    {
        if (*it1 > *it2)
            v.push_back(*it1++);
        else
            v.push_back(*it2++);
    }

    while (it1 != v1.end())
        v.push_back(*it1++);
    while (it2 != v2.end())
        v.push_back(*it2++);
}