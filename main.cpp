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
#include <random>
#include <chrono>
#include <ctime> 
#include "DataStructures.h"

#define tableSize 20
#define howManyNumbers 1000
#define minAlbum 99
#define maxAlbum 99990

using namespace std;


Ai* numbers[tableSize];

int hashKey(double key, int size)
{
    int hashedKey = (key-minAlbum)/((maxAlbum-minAlbum)/size);
    if(hashedKey>=size)
    {
        cout<<"OVERFLOW!"<<key<<"->"<<hashedKey<<endl;
        hashedKey=size;
    }
    
    return hashedKey;
}
void ClearTable()
{
    for(int i=0; i<tableSize;i++)
    {
        numbers[i]=NULL;
    }
}
Ai* StudentPointer(int aNumber, bool& canAdd)
{
    Ai* p = numbers[hashKey(aNumber,tableSize)];

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
Ai* NewRandomEntry()
{
    Ai* newRandomEntry = new Ai;
    int randomNumber = rand()+rand()+rand()+rand();
    newRandomEntry->number = (double)(minAlbum+(randomNumber%(maxAlbum-minAlbum)));
    newRandomEntry->next = NULL;
    return newRandomEntry;
}
void FillTable(int howMany)
{
    for(int i =0; i<howMany; i++)
    {
        Ai* newEntry = NewRandomEntry();

        bool canAdd = true;
        Ai* p = StudentPointer(newEntry->number,canAdd);
        
        if(canAdd)
        {
            if(!p)
            {
                numbers[hashKey(newEntry->number,tableSize)] = newEntry;
            }
            else
            {
                p->next = newEntry;
            }
        }
        
    }
}
void WriteOutTable()
{
    for(int i=0; i<tableSize;i++)
    {
        cout<<"Cell "<<i<<endl<<"["<<endl;
        Ai* p = numbers[i];
        while(p)
        {
            double k = p->number;
            cout<<k<<endl;
            p = p->next;
        }
        cout<<"]"<<endl;
    }
}
void GetSmallestNumber()
{
    double ss = 0;
    Ai* p = numbers[0];
    for(int i=0; i<tableSize;i++)
    {
        p = numbers[i];
        if(p)
        { 
            break;
        }
    }
    ss = p->number;
    while(p)
    {
        if((p->number)<ss)
        {
            ss=p->number;
        }
        p = p->next;
    }
    cout<<"Smallest"<<ss<<endl;
    
}
int main()
{
    cout<<"START"<<rand()<<"|"<<rand()<<endl;
    ClearTable();
    cout<<"Cleared"<<endl;
    FillTable(howManyNumbers);
    cout<<"Added all"<<endl;
    WriteOutTable();
    cout<<endl;
    GetSmallestNumber();
    //clearing memory
    for(int i=0;i<tableSize;i++)
    {
        Ai* p = numbers[i];
        while(p = numbers[i])
        {
            numbers[i] = p->next;
            p->number=0;
            delete p;
        }
    }
    
    //double s = GetSmallestNumber();
   // cout<<"Smallest number: "<<s<<endl;
    cout<<"End"<<endl;
}