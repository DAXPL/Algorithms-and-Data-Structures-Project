#include <iostream>
#include <vector>
#include <string>
using namespace std;
//Struktura wyrazu ciągu
struct Ai
{
    Ai* next;//następnik
    int number;//wartość własna

    double GetDoubleValue()
    {
        int dd = number;
        return dd/100;
    }
};