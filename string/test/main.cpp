#include "mystring.cpp" 
int main()
{
    A a("aaa");
    A b("ccc");
    A c = a;
    cout<<"A "<<a;
    cout<<"B "<<b;
    cout<<"C = A =  "<<c;
    A d;
    d= a+b;
    cout<<"D = aaa + ccc = "<<d;
    A e("aaaacccc");
    A f("aaaa");
    A g;
    g= e-f;
    cout<<"G = aaaacccc - aaaa = "<<g;
    cout<<"A 与 B 不相等？"<<endl;
    if(a!=b)
        cout<<"不相等"<<endl;
    else
        cout<<"相等"<<endl;
    char  t = d[3];
    cout<<"D 的第四个元素： "<<t<<endl;
    A h("aaaaaaaaaaaaacccccccczzzzzzzzz");
    cout<<h<<"转为大写";
    h.transforms();
    cout<<h;
    A test("edcba");
    A ::Iterator it = NULL;
    it = test.begin();
    while(it != test.end()) {
        cout<<*it<<endl;
        ++it;
    }
    cout<<"test: "<<test<<endl;
    it = test.begin();
    it++;
    it++;
    test.Erase(test.begin(),it);
    cout<<"test: "<<test<<endl;
    return 0;
}
