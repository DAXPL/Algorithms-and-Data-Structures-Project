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

#define bufforSize 4096//bufor zapisu
#define maxNumers 10000//największa długość ciągu ai
#define measurements 500//ilość pomiarów uśredniających
#define tables 10//ilość rozwarzanych rozmiarów tablic
#define minRandom 0.99
#define maxRandom 999.99

using namespace std;

char fBuf[bufforSize];

int tableSize = 10;
Ai** numbers;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<unsigned int> dis(minRandom*100, maxRandom*100);

//Funkcja hashująca podany klucz, dla zadanego rozmiaru tablicy
int hashKey(double key, int size)
{
    int hashedKey = round((key-minRandom)/((maxRandom-minRandom)/size));//round down
    
    if(hashedKey>=size)
    {
        hashedKey=size-1;
    }
    
    return hashedKey;
}

//Przygotowanie obszaru w pamięci dla tablicy
void PrepareTable()
{
    for(int i=0;i<tableSize;i++)
    {
        numbers[i]=NULL;
    }
}

//Zwalnianie pamięci tablicy
void ClearTable()
{
    for(int i=0;i<tableSize;i++)
    {   
        //Czyszczenie listy jednokierunkowej
        Ai* p = NULL;
        while(p = numbers[i])
        {
            numbers[i] = p->next;
            delete p; 
        } 
        numbers[i]=NULL;  
    }
}

//Wyszukanie ostatniego elementu w liście jednokierunkowej
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

//Generowanie losowego elementu ciągu
Ai* NewRandomEntry()
{
    Ai* newRandomEntry = new Ai;
    //losowanie liczby w zakresie 99..99999 i dopasowanie do wymaganego zakresu
    newRandomEntry->number = (double)dis(gen)/100.0;
    newRandomEntry->next = NULL;
    return newRandomEntry;
}

//Wypełnianie tablicy losowym ciągiem długości howMany
void FillTable(int howMany)
{
    for(int i =0; i<howMany; i++)
    {
        Ai* newEntry = NewRandomEntry();
        Ai* p = FindElement(newEntry->number);

        if(p)
        {
            //Dopisanie elementu na koniec tablicy
            p->next = newEntry;  
        }
        else
        {
            //Wpisanie elementu do pustej komórki
            numbers[hashKey(newEntry->number,tableSize)] = newEntry;
        }
        
    }
}

//Wypisywanie zawartości tabeli do standardowego wyjścia
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
    cout<<endl;
}

//Funkcja wyszukująca najmniejszy element w tablicy
double GetSmallestNumber(int& comparisons)
{
    //Przygotowanie do wyszukiwania
    double ss = 0; //Najmniejsza liczba
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

    //zabezpieczenie na wypadek pustej listy
    if(p==NULL)
    {
        cout<<"List is empty!"<<endl;
        return -1;
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

//Funkcja wyszukująca największy element w tablicy
double GetLargestNumber(int& comparisons)
{
    //Przygotowanie do wyszukiwania
    double max = 0; //największa liczba
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

    //zabezpieczenie na wypadek pustej listy
    if(p==NULL)
    {
        cout<<"List is empty!"<<endl;
        return -1;
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

//Jedna seria pomiarów dla zadanego rozmiaru tablicy i ciągu
void SeriesOfMeasurements(fstream& file,int& size, int&compS, int& compL)
{
    ClearTable();
    
    FillTable(size);
    //WriteOutTable();

    double smallest,largest;
    smallest = GetSmallestNumber(compS);
    largest = GetLargestNumber(compL);
    ClearTable();//???
}

//Główna pętla programu
int main()
{
    cout<<"START"<<endl;

    //Przygotowanie pliku na przyjęcie danych
    fstream outputFile; 
    outputFile.rdbuf()->pubsetbuf(fBuf,bufforSize);
    outputFile.open("data.csv", ios::out);
    if(!outputFile)
    {
        cout<<"Failed to create file!"<<endl;
        return 0;
    }
    outputFile<<"Rozmiar tablicy\tDlugosc ciagu\tOperacje"<<endl;
    //Seria pomiarów
    for(int z=1;z<=tables;z++)//pomiary dla różnych rozmiarów tablicy hashującej
    {
        tableSize = 10*z;
        numbers = new Ai*[tableSize];
        PrepareTable();

        cout<<"Seria "<<z<<" dla rozmiaru tablicy: "<<tableSize<<endl;
        for(int i=10;i<=maxNumers;i*=10)//pomiary dla różnych długości ciągu a
        {
            //cout<<i<<" z "<<maxNumers;
            double compSmMed =0; //Średnia porównań dla najmniejszego elementu
            double compLgMed =0; //Średnia porównań dla największego elementu
            double compMed =0;

            for(int j=0;j<measurements;j++)//powtórzenia dla uśrednienia
            {
                //porównania dla tej seii pomiarów
                int compSm =0; 
                int compLg =0;

                //Wykonanie serii pomiarów
                SeriesOfMeasurements(outputFile,i,compSm,compLg);

                //Zsumowanie liczby operacji
                compSmMed+=compSm;
                compLgMed+=compLg;
            }

            //wyznaczanie średniej liczby operacji
            compMed += compSmMed + compLgMed;
            compSmMed/=measurements;
            compLgMed/=measurements;
            compMed/=2*measurements;

            if(outputFile)
            {
                //rozmiar tablicy, długość ciągu, średnia porównań
                outputFile<<tableSize<<"\t"<<i<<"\t"<<compMed<<endl;
            }  
            //cout<<endl;    
        }
    }

    //Kończenie i zwalnianie pamięci
    if(outputFile)
    {
        outputFile.close();
    }
    ClearTable();
    delete[] numbers;
    cout<<"End"<<endl;
}