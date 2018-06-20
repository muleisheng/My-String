#include "String.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef WIN32
#include <Windows.h>
#endif
#define FREE_POINTER(data) \
    if(data) \
        free(data); \
    data = nullptr

#define DELETE_POINTER(p) \
    { \
        if(p) \
            delete p; \
        p = NULL; \
    }

#define DELETE_ARRAY(arr) \
    { \
        if(arr) \
            delete [] arr; \
        arr = NULL; \
    }
#define MAX_LEN 1024*10
String::String(const char *str /*= NULL*/) throw (int)
    :m_data(nullptr)
{
    CloneData(str);
}

String::String(const String &str) throw(int)
     :m_data(nullptr)
{
    *this = str;
}
String::String (const string& str) throw(int){
    *this = str;
}
String::~String()
{
    FREE_POINTER(m_data);
}

void String::InitData(int nSize) throw(int){
    m_data = (char*)realloc(m_data,nSize);
    if(!m_data)
        throw MEM_REALLOC_FAILED;
}
void String::CloneData(const char *str) throw (int){
    if(!str){
        FREE_POINTER(m_data);
        return;
    }
    int src_len = strlen(str)+1;
    InitData(src_len);
    memset(m_data,0,sizeof(char)*src_len);
    memcpy(m_data,str,sizeof(char)*src_len);
}
String &String::operator = (const char *str) throw(int){
    CloneData(str);
    return *this;
}

String &String::operator = (const string & str) throw(int){
        CloneData(str.c_str());
        return *this;
}

String &String::operator = (const String & str) throw(int){
        CloneData(str.c_str());
        return *this;
}

bool String::IsEmpty() {
    if(!m_data || m_data[0] == '\0')
        return true;
    return false;
}

size_t String::Length(){
    if(IsEmpty())
        return 0;
    return strlen(m_data);
}

int String::Format(const char *str,...)  throw (int){
    char buf[MAX_LEN] = {0};
    va_list list;
    va_start(list,str);
    int ret = vsprintf(buf,str,list) ;
    if(ret >= 0)
        CloneData(buf);
    va_end(list);
    return ret;
}

int String::Compare(const char *str){
    return string(m_data).compare(str);
}
#ifdef WIN32
bool String::TransCode(int codePage){
    int wcsLen = MultiByteToWideChar(CP_ACP, NULL, m_data, strlen(m_data), NULL, 0);
    wchar_t* wszString = new wchar_t[wcsLen + 1];
    MultiByteToWideChar(CP_ACP, NULL, m_data, strlen(m_data), wszString, wcsLen);

    int nLen = WideCharToMultiByte(codePage,NULL,wszString,-1,NULL,NULL,NULL,NULL);  
    char *szBuffer = new char[nLen+1];  
    nLen = WideCharToMultiByte(codePage,NULL,wszString,-1,szBuffer,nLen,NULL,NULL);  
    szBuffer[nLen] = '\0';  
    DELETE_ARRAY(wszString);

    int nSize = nLen+1;

    m_data = (char*)realloc(m_data,nSize);
    if(!m_data){
        DELETE_ARRAY(szBuffer);
        throw MEM_REALLOC_FAILED;
    }
    memset(m_data,0,sizeof(char)*nSize);
    memcpy(m_data,szBuffer,sizeof(char)*nSize);
    DELETE_ARRAY(szBuffer);
    return true;
}
#endif

String String::operator + (const char *str) throw (int){
    String retStr(*this);
    retStr += str;
    return retStr;
}
String &String::operator += (const char *str) throw (int){
    if(!str || str[0] == '\0')
        return *this;
    int len = strlen(str);
    m_data = (char*)realloc(m_data,len+Length()+1);
    if(!m_data)
        throw MEM_REALLOC_FAILED;
    int total_size = len+Length()+1;
    memset(m_data+Length(),0,sizeof(char)*(len+1));
    memcpy(m_data+Length(),str,sizeof(char)*(len+1));
    return *this;
}

size_t String::Find(const char *str,size_t pos/* = 0*/){
    string srcStr(m_data);
    return srcStr.find(str,pos);
}

char &String::operator [](int nPos){
    return m_data[nPos];
}

char String::At(int nPos){
    return m_data[nPos];
}
void String::Trancate(size_t nPos){
    if(nPos >=  strlen(m_data))
        return;
    m_data[nPos] = '\0';
}

void String::Reverse(){
    String::iterator first = begin();
    String::iterator last = end();
    while ((first != last) && (first != --last)){
        swap(*first, *last);
        first++;
    }
}