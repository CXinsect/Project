#include "mystring.h"

A::A()
{
    m_len = 0;
    m_p = new char [m_len + 1];
    strcpy(m_p,"");
}

A::A(const char * str)
{
    m_len = strlen(str);
    m_p = new char [m_len + 1];
    strcpy(m_p,str);
}
A::A (A & a)
{
    m_len = a.m_len;
    m_p = new char [m_len + 1];
    strcpy(m_p,a.m_p);
}
A::~A()
{
    if(m_p  != NULL)
    {
        delete [] m_p;
        m_len = 0;
    }
}
A & A ::operator= (A & a)
{
    cout<<"main ll "<<a.m_p<<a.m_len<<endl;
    char tmp [64];
    strcpy(tmp,a.m_p);
    if(a.m_len == 0)
    {
        cout<<"input error"<<endl;
    }
    int t = a.m_len;
    if(m_p != NULL)
    {
        delete [] m_p;
        m_len = 0;
    }
    m_len = t;
    m_p = new char [m_len +1];
    strcpy(this->m_p,tmp);
    return *this;
}
char  A :: operator[] (int index)
{
    return m_p[index];
}
bool A:: operator == (A & a)
{
    if(m_len == a.m_len)
    {
        for(int i = 0;i<a.m_len;i++)
        {
            if(m_p[i] != a.m_p[i])
                return false;
        }
        return  true;
    }
    else 
        return false;
}
bool A ::operator != (A & a)
{
    return !(*this == a);
}
A & A ::operator + (A & a)
{
    m_len = a.m_len +m_len;
    char * t = m_p;
    m_p = new char [m_len + 1];
    strcpy(m_p,t);
    strcat(m_p,a.m_p);
    delete [] t;
    return *this;
}
A & A ::operator -(A & a)
{
    if(a.m_len > m_len)
    {
        cout<<"wrong element"<<endl;
    }
    else
    {
        char * p = NULL;
        char * tmp = new char [64];
        while((p = strstr(m_p,a.m_p)) != NULL)
        {
             *p = '\0';
             strcpy(tmp,p+strlen(a.m_p));
             strcat(m_p,tmp);
        }
        delete [] tmp;
        m_p[strlen(m_p) + 1] = '\0';
        m_len = strlen(m_p);
    }
   

    return *this;
}
ostream &  operator<< (ostream & out,A & a)
{
    out<<a.m_p<<endl;
    return out;
}
istream & operator>> (istream & in,A & a)
{
    in>>a.m_p;
    return in;
}
A & A ::transforms ()
{
    for(int i = 0;i<m_len;i++)
    {
        if(m_p[i] >= 'a'  && m_p[i] <= 'z')
            m_p[i] -= 32;
        else
        {
            cout<<"input error"<<endl;
            break;
        }
    }
    return *this;
}
bool A::Iterator :: operator != (const A::Iterator & it) {
    return !(ptr == it.ptr);
}
A::Iterator & A::Iterator::operator++() {
    ptr++;
    return *this;
}
A::Iterator A::Iterator::operator++(int) {
    A::Iterator tmp = *this;
    ptr++;
    return tmp;
}
char & A::Iterator::operator*() {
    return *ptr;
}
A::Iterator  A::begin() {
    return Iterator(m_p);
}
A::Iterator A::end() {
    return Iterator(m_p + strlen(m_p));
}
A::Iterator A::Erase(A::Iterator start,A::Iterator end) {
    return NULL;
}
