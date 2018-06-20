#ifndef _STRING_H_
#define _STRING_H_
#include <string>
using namespace std;

#define MEM_REALLOC_FAILED  0x01
class String{
public:
    String(const char *str = nullptr) throw (int);
    String(const String &str) throw(int);
    String (const string& str) throw(int);
    ~String();

public:
    class iterator{
    public:
        iterator(String &str,int nPos):m_str(str),m_nPos(nPos){};
        iterator &operator++(){
            ++m_nPos;
            return *this;
        };
        iterator &operator ++(int){
            m_nPos ++; 
            return *this;
        };
        iterator &operator --(){
            --m_nPos;
            return *this;
        };
        iterator &operator --(int){
            m_nPos -- ;
            return *this;
        };
        char &operator*(){return m_str.GetChar(m_nPos);};
        bool operator != (iterator itor){return m_nPos != itor.Position();};
        int Position() const {return m_nPos;};
    private:
        String &m_str;
        int m_nPos;
    };
public:
   
    String &operator = (const char *str) throw(int);
    String &operator = (const string & str) throw(int);
    String &operator = (const String & str) throw(int);

    String &operator += (const char *str) throw (int);
    template<typename T>
    String &operator += (const T &str) throw (int){
        *this += str.c_str();
        return *this;
    }

    String operator + (const char *str) throw (int);
    template<typename T>
    String operator + (const T & str) throw(int){
        String retStr(*this);
        retStr += str.c_str();
        return retStr;
    };
    
    char &operator [](int nPos);
    char At(int nPos);
public:
    const char * c_str() const {return m_data;};
    char &GetChar(int nPos) {return m_data[nPos];};
    bool IsEmpty() ;
    size_t Length();
    int Compare(const char *str);
    void Trancate(size_t nPos);
    void Reverse();
    /**used like std::string*/
    size_t Find(const char *str,size_t pos = 0);
    template<typename T>
    size_t Find(const char *str,size_t pos = 0){
        string srcStr(m_data);
        return srcStr.find(str.c_str(),pos);
    };
public:
#ifdef WIN32
    bool TransCode(int codePage);
#endif
public: //for iterator
    iterator begin(){return iterator(*this,0);};
    iterator end(){return iterator(*this,Length());};
public:
    int Format(const char *str,...) throw (int);
private:
    void CloneData(const char *str) throw(int);
    void InitData(int nSize) throw(int);
private:
    char *m_data;
};
#endif