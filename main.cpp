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
#include <string>
#include <random>
#include <fstream>
#include <Windows.h>
#include "DataStructures.h"

#define tableSize 20
#define maxNumers 10000
#define measurements 25
#define minRandom 0.99
#define maxRandom 999.99

using namespace std;

Ai* numbers[tableSize];
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<unsigned int> dis(minRandom*100, maxRandom*100);

int hashKey(double key, int size)
{
    int hashedKey = (key-minRandom)/((maxRandom-minRandom)/size);
    if(hashedKey>=size)
    {
        cout<<"OVERFLOW!"<<key<<"->"<<hashedKey<<endl;
        hashedKey=size-1;
    }
    
    return hashedKey;
}

void ClearTable()
{
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
}

Ai* FindElement(int aNumber, bool& canAdd)
{
    Ai* p = numbers[hashKey(aNumber,tableSize)];

    canAdd = true;
    if(p)
    {
        while (true)
        {
            if(p->number == aNumber)
            {
                canAdd = false;
                return NULL;
                cout<<"Wpis juz istnieje"<<endl;
            }
            if(!p->next)
            {
                return p;//doszlismy do końca listy jednokierunkowej
            }
            p = p->next;//jeszcze nie ma końca listy
        }
    }

    //cout<<"Cant find number! Cell empty?"<<endl;
    return NULL;             
}

Ai* NewRandomEntry()
{
    Ai* newRandomEntry = new Ai;
    newRandomEntry->number = (double)dis(gen)/100.0;
    newRandomEntry->next = NULL;
    return newRandomEntry;
}

void FillTable(int howMany)
{
    for(int i =0; i<howMany; i++)
    {
        bool canAdd = true;
        Ai* newEntry = NewRandomEntry();
        Ai* p = FindElement(newEntry->number,canAdd);
        
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

double GetSmallestNumber(int& comparisons)
{
    double ss = 0;
    comparisons = 0;
    Ai* p = nullptr;

    /*
    Szukam pierwszej komórki która nie jest pusta - to w niej będą najmniejsze
    wartości
    */
    for(int i=0; i<tableSize;i++)
    {
        comparisons++;
        p = numbers[i];
        if(p)
        { 
            break;
        }
    }

    /*
    Niech pierwsza liczba będzie na razie najmniejsza.
    Potem iteruje po całej zawartości link listy szukając coraz to mniejszych
    wartości ciągu ai.
    Od razu też przesuwam wskaźnik - nie ma sensu sprawdzać go dwa razy
    */
    ss = p->number;
    p = p->next;
    while(p)
    {
        if((p->number)<ss)
        {
            ss=p->number;
        }
        p = p->next;
        comparisons++;
    }
    //cout<<"Smallest number: "<<ss<<" found in: "<<comparisons<<" comparisons"<<endl;
    return ss;
    
}

double GetLargestNumber(int& comparisons)
{
    double max = 0;
    comparisons = 0;
    Ai* p = nullptr;

    /*
    Szukam ostatniej komórki która nie jest pusta - to w niej będą największe
    wartości
    */
    for(int i=tableSize-1; i>=0;i--)
    {
        comparisons++;
        p = numbers[i];
        if(p)
        { 
            break;
        }
    }

    /*
    Niech pierwsza liczba będzie na razie największa.
    Potem iteruje po całej zawartości link listy szukając coraz to większych
    wartości ciągu ai.
    Od razu też przesuwam wskaźnik - nie ma sensu sprawdzać go dwa razy
    */
    max = p->number;
    p = p->next;
    while(p)
    {
        if((p->number)>max)
        {
            max=p->number;
        }
        p = p->next;
        comparisons++;
    }
    //cout<<"Largest number: "<<max<<" found in: "<<comparisons<<" comparisons"<<endl;
    return max;
    
}

void DoStuff(fstream& file,int& size, int&compS, int& compL)
{
    //srand((int) time(0));
    ClearTable();
    FillTable(size);
    //WriteOutTable();
    //cout<<endl;

    double smallest,largest;
    smallest = GetSmallestNumber(compS);
    largest = GetLargestNumber(compL);
    ClearTable();
}

int main()
{
    cout<<"START"<<endl;

    fstream outputFile;
    outputFile.open("data.csv", ios::out);
    if(!outputFile)
    {
        cout<<"Failed to create file!"<<endl;
    }

    for(int i=10;i<=maxNumers;i+=50)
    {
        cout<<i<<" z "<<maxNumers;
        double compSmMed =0;
        double compLgMed =0;
        for(int j=0;j<measurements;j++)
        {
            cout<<".";
            int compSm, compLg;
            DoStuff(outputFile,i,compSm,compLg);

            compSmMed+=compSm;
            compLgMed+=compLg;
        }

        //wyznaczanie średniej z sumy porównań
        compSmMed/=measurements;
        compLgMed/=measurements;

        if(outputFile)
        {
            outputFile<<i<<","<<compSmMed<<","<<compLgMed<<endl;
        }  
         
        cout<<endl;    
    }


    if(outputFile)
    {
        outputFile.close();
    }
    cout<<"End"<<endl;
}