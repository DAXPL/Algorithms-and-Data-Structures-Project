#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct Student
{
    Student* next;
    int number;
    string name;
    string surname;
};