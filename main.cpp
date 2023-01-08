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

#define bufforSize 4096
#define maxNumers 1000
#define measurements 100
#define minRandom 0.99
#define maxRandom 999.99

using namespace std;

char fBuf[4096];

int tableSize = 10;
Ai** numbers;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<unsigned int> dis(minRandom*100, maxRandom*100);

int hashKey(double key, int size)
{
    int hashedKey = round((key-minRandom)/((maxRandom-minRandom)/size));//round down
    
    if(hashedKey>=size)
    {
        hashedKey=size-1;
    }
    
    return hashedKey;
}

void PrepareTable()
{
    for(int i=0;i<tableSize;i++)
    {
        numbers[i]=NULL;
    }
}

void ClearTable()
{
    for(int i=0;i<tableSize;i++)
    {   
        //Ai* p = numbers[i];
        Ai* p = NULL;
        while(p = numbers[i])
        {
            numbers[i] = p->next;
            delete p; 
        }   
    }
}

Ai* FindElement(int aNumber)
{
    Ai* p = numbers[hashKey(aNumber,tableSize)];

    if(p)
    {
        while (true)
        {
            if(!p->next)
            {
                return p;//doszlismy do końca listy jednokierunkowej
            }
            p = p->next;//jeszcze nie ma końca listy
        }
    }

    //komórka jest pusta
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
        Ai* newEntry = NewRandomEntry();
        Ai* p = FindElement(newEntry->number);

        if(p)
        {
            p->next = newEntry;  
        }
        else
        {
            numbers[hashKey(newEntry->number,tableSize)] = newEntry;
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
    outputFile.rdbuf()->pubsetbuf(fBuf,bufforSize);
    outputFile.open("data.csv", ios::out);
    if(!outputFile)
    {
        cout<<"Failed to create file!"<<endl;
        return 0;
    }

    for(int z=1;z<=5;z++)//pomiary dla różnych rozmiarów tablicy hashującej
    {
        tableSize = 10*z;
        numbers = new Ai*[tableSize];
        PrepareTable();

        //outputFile<<tableSize<<" dlugosc tablicy, sr por min, sr por max, sr"<<endl;
        cout<<"Seria "<<z<<" dla rozmiaru tablicy: "<<tableSize<<endl;
        for(int i=10;i<=maxNumers;i+=10)//pomiary dla różnych długości ciągu a
        {
            cout<<i<<" z "<<maxNumers;
            double compSmMed =0;
            double compLgMed =0;
            double compMed =0;
            for(int j=0;j<measurements;j++)//powtórzenia dla uśrednienia
            {
                cout<<".";
                int compSm =0;
                int compLg =0;
                DoStuff(outputFile,i,compSm,compLg);

                compSmMed+=compSm;
                compLgMed+=compLg;
            }

            //wyznaczanie średniej z sumy porównań
            compMed += compSmMed + compLgMed;
            compSmMed/=measurements;
            compLgMed/=measurements;
            compMed/=2*measurements;

            if(outputFile)
            {
                //rozmiar tablicy, długość ciągu, średnia porównań
                outputFile<<tableSize<<", "<<i<<", "<<compMed<<endl;
            }  
            
            cout<<endl;    
        }
    }
   


    if(outputFile)
    {
        outputFile.close();
    }
    delete[] numbers;
    cout<<"End"<<endl;
}