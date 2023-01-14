#include <iostream>
#include <vector>
#include <string>
using namespace std;
//Struktura wyrazu ciągu
struct Ai
{
    Ai* next;//następnik
    int number;//wartość własna
    //ze względu na znaczną ilość komplikacji z obliczeniami zmiennoprzecinkowymi
    //dlatego przechowuję liczbę jako int i tylko przy wpisywaniu do wyników
    //dzielę przez 100
    double GetDoubleValue()
    {
        int dd = number;
        return dd/100;
    }
};