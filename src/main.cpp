#include <iostream>
#include <fstream>
#include "dbms.h"

int main()
{
    dbms::DBMS dbms;
    dbms.import_from_file("test2.txt");
    std::cin.get();
    return 0;
}