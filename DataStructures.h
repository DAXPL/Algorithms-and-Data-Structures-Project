#include <iostream>
#include <vector>
#include <string>
using namespace std;
//Struktura wyrazu ciągu
struct Ai
{
    Ai* next;//następnik
    double number;//wartość własna
};