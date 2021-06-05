#include "dbms.h"

namespace dbms
{

// Constructors VehicleBase
    VehicleBase::VehicleBase(){}
    VehicleBase::VehicleBase(std::string const & company, std::string const & model, std::vector<std::string> const & others) 
        : m_company(company) , m_model(model) , m_others(others) {}
    VehicleBase::VehicleBase(std::string && company, std::string && model, std::vector<std::string> && others )
        : m_company(std::move(company)) , m_model(std::move(model)) , m_others(std::move(others)) {}
    VehicleBase::VehicleBase(VehicleBase const &) = default;
    VehicleBase::VehicleBase(VehicleBase &&) = default;
    VehicleBase::~VehicleBase() = default;

// Members VehicleBase

    Vehicle::Vehicle(){}
    Vehicle::Vehicle(std::string const & company, std::string const & model, std::vector<std::string> const & others, std::string const & product_id) 
        : VehicleBase(company , model , others) , m_product_id(product_id){}
    Vehicle::Vehicle(std::string && company, std::string && model, std::vector<std::string> && others , std::string && product_id)
        : VehicleBase(std::move(company) , std::move(model) , std::move(others)) , m_product_id(std::move(product_id)){}
    Vehicle::Vehicle(Vehicle const &) = default;
    Vehicle::Vehicle(Vehicle &&) = default;
    Vehicle::~Vehicle() = default;


    DBMS::DBMS(std::vector<Vehicle> const & vehicle_list)
        :m_database(vehicle_list) {}
    DBMS::~DBMS() = default;

}