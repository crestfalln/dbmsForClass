#include <iostream>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "dbms.h"

int main()
{
    dbms::DBMS dbms;
    std::ofstream outFile("out.xml");
    boost::archive::xml_oarchive out(outFile);
    out << BOOST_SERIALIZATION_NVP(dbms);
}