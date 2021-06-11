#include <iostream>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "dbms.h"

int main()
{
    dbms::DBMS dbms;
    dbms.import_from_file("test2.txt");
    std::cin.get();
    return 0;
}