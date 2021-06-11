#include <iostream>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "dbms.h"

int main()
{
    dbms::DBMS dbms;
    dbms.add_from_file("test.txt");
    dbms.print_stock();
    std::cin.get();
    return 0;
}