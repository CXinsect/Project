#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
class A
{
    friend ostream &  operator<< (ostream & out,A & a);
    friend istream & operator>> (istream & in,A & a);
public:
    A();
    A(const char * str);
    A(A & a);
    ~A();
public:
    A & operator= (A & a);
    char operator[] (int index);
    bool operator == (A & a);
    bool operator != (A & a);
    A & operator +(A & a);
    A & operator - (A & a);
    A & transforms ();
    int get_length()
    {
        return m_len;
    }
    char * C_str() {
        return m_p;
    }

class Iterator {
public:
    Iterator( char * _ptr) {
        ptr = _ptr;
    }
    bool operator != (const Iterator & it);
    //前置加加
    Iterator & operator ++();
    //后置加加
    Iterator  operator ++(int);
    char &  operator * ();
private:
    char * ptr;
};
Iterator  begin();
Iterator  end();
Iterator Erase(Iterator start,Iterator end);

private:
    char * m_p;
    int m_len;

};
