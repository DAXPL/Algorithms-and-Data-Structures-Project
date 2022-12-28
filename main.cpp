/*
Autor:  Miłosz Klim
Temat:  wyszukanie najmniejszego/największego elementu ciągu {ai} 
        zapisanego w tablicy z haszowaniem, jeżeli 0.99 ≤ ai ≤999.99
Proponowane rozwiązanie:
        bucket hashing (porcjowanie)
Źródła:
        https://eduinf.waw.pl/inf/alg/001_search/0099a.php
Dodatkowe:
        https://www.youtube.com/watch?v=2_3fR-k-LzI
        https://eduinf.waw.pl/inf/alg/001_search/0067e.php (czym jest hashowanie)
*/
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "student.h"
#define tableSize 20
#define howManyStudents 1000

using namespace std;


Student* students[tableSize];

int hashKey(int key, int size)
{
    return key%size;
}
void ClearTable()
{
    for(int i=0; i<tableSize;i++)
    {
        students[i]=NULL;
    }
}
Student* StudentPointer(int aNumber, bool& canAdd)
{
    Student* p = students[hashKey(aNumber,tableSize)];

    canAdd = true;
    if(p)
    {
        while (true)
        {
            if(p->number == aNumber)
            {
                //Ten student jest już w tablicy
                //TO-DO - przeładować == ? (porównanie imie, nazwisko i numer albumu)
                canAdd = false;
                return NULL;
                cout<<"Wpis juz istnieje"<<endl;
            }
            if(!p->next)
            {
                return p;//doszlismy do końca tablicy jednokierunkowej - p to element do którego dopiszemy
            }
            p = p->next;//jeszcze nie ma końca tablicy
        }
    }

    cout<<"Cant find student! Cell empty?"<<endl;
    return NULL;             
}
Student* NewRandomStudent()
{
    Student* newRandomStudent= new Student;
            newRandomStudent->name = "default name";
            newRandomStudent->surname = "default surname";
            newRandomStudent->number = rand()%999999+100000;
            newRandomStudent->next = NULL;
    return newRandomStudent;
}
void FillTable(int howMany)
{
    for(int i =0; i<howMany; i++)
    {
        Student* newStudent = NewRandomStudent();
        
        bool canAdd = true;
        Student* p = StudentPointer(newStudent->number,canAdd);
        
        if(canAdd)
        {
            if(!p)
            {
                students[hashKey(newStudent->number,tableSize)] = newStudent;
            }
            else
            {
                p->next = newStudent;
            }
        }
        
    }
}
void WriteOutTable()
{
    for(int i=0; i<tableSize;i++)
    {
        cout<<"Cell "<<i<<endl<<"["<<endl;
        Student* p = students[i];
        while(p)
        {
            cout<<p->name<<" "<<p->surname<<" "<<p->number<<endl;
            p = p->next;
        }
        cout<<"]"<<endl;
    }
}
int main()
{
    cout<<"START"<<endl;
    ClearTable();
    cout<<"Cleared"<<endl;
    FillTable(howManyStudents);
    cout<<"Added all"<<endl;
    WriteOutTable();
    cout<<endl;
    
    //clearing memory
    for(int i=0;i<tableSize;i++)
    {
        Student* p = students[i];
        while(p = students[i])
        {
            students[i] = p->next;
            p->name="";
            p->surname="";
            p->number=0;
            delete p;
        }
    }
    cout<<"End"<<endl;
}