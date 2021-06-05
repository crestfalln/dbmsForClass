#include "dbms.h"

namespace dbms
{

// Constructors VehicleBase
    VehicleBase::VehicleBase(){}
    VehicleBase::VehicleBase(std::string const & company, std::string const & model, std::vector<std::string> const & others) 
        : m_company(company) , m_model(model) , m_others(others) {}
    VehicleBase::VehicleBase(std::string && company, std::string && model, std::vector<std::string> && others )
        : m_company(company) , m_model(model) , m_others(others) {}
    VehicleBase::VehicleBase(VehicleBase const &) = default;
    VehicleBase::VehicleBase(VehicleBase &&) = default;

// Members VehicleBase
    template <class Archive>
    void VehicleBase::serialize(Archive & ar , unsigned const int version)
    {
        ar & BOOST_SERIALIZATION_NVP(m_company);
        ar & BOOST_SERIALIZATION_NVP(m_model);
        ar & BOOST_SERIALIZATION_NVP(m_others);
    }

    Vehicle::Vehicle(){}
    Vehicle::Vehicle(std::string const & company, std::string const & model, std::vector<std::string> const & others, std::string const & product_id) 
        : VehicleBase(company , model , others) , m_product_id(product_id){}
    Vehicle::Vehicle(std::string && company, std::string && model, std::vector<std::string> && others , std::string && product_id)
        : VehicleBase(company , model , others) , m_product_id(product_id){}
    Vehicle::Vehicle(Vehicle const &) = default;
    Vehicle::Vehicle(Vehicle &&) = default;

    template <class Archive>
    void Vehicle::serialize(Archive &ar, const unsigned int version) 
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP<VehicleBase>(*this);
        ar & BOOST_SERIALIZATION_NVP(m_product_id);
    }
}