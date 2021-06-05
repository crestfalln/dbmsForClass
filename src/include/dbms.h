#ifndef DBMS_H

#define DBMS_H
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>

#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>

namespace dbms
{
    class VehicleBase
    {
    protected:
        friend class boost::serialization::access;
        friend class DBMS;
        friend class catalouge;

        std::string m_company;
        std::string m_model;
        enum {
            COLOR,
            ENGINE
        };
        std::vector<std::string> m_others;

        VehicleBase();
        VehicleBase(std::string const & , std::string const & , std::vector<std::string> const & );
        VehicleBase(std::string &&, std::string &&, std::vector<std::string> && );
        VehicleBase(VehicleBase const &);
        VehicleBase(VehicleBase &&);

        template <class Archive>
        void serialize(Archive &, const unsigned int);

        virtual ~VehicleBase();
    };
    class Vehicle : public VehicleBase
    {
    protected:
        std::string m_product_id;

        Vehicle();
        Vehicle(std::string const & , std::string const & , std::vector<std::string> const & , std::string const &);
        Vehicle(std::string &&, std::string &&, std::vector<std::string> && , std::string &&);
        Vehicle(Vehicle const &);
        Vehicle(Vehicle &&);

        template <class Archive>
        void serialize(Archive &ar, const unsigned int version) ;

        virtual ~Vehicle();
    };
}

#endif