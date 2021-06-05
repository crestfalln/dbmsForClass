#include <iostream>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "dbms.h"

int main()
{
    dbms::DBMS dbms({dbms::Vehicle("Tesla" , "Model X" , {"Red"} , "0x123"),
    dbms::Vehicle("Tesla" , "Model Y" , {"Red"} , "0x123"),
    dbms::Vehicle("Tesla" , "Model Z" , {"Blue"} , "0x123"),
    dbms::Vehicle("Tesla" , "Model Q" , {"Green"} , "0x123"),
    dbms::Vehicle("Tesla" , "Model R" , {"Blue"} , "0x123"),
    dbms::Vehicle("Tesla" , "Model L" , {"Red"} , "0x123")
    });
    std::ofstream outFile("out.xml");
    boost::archive::xml_oarchive out(outFile);
    out << BOOST_SERIALIZATION_NVP(dbms);
}