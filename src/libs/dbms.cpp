#include "dbms.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace dbms
{
    parsing_error::parsing_error(char *e , int line_count = 0) : runtime_error(e) , m_line_count(line_count) {}

    // Constructors VehicleBase
    VehicleBase::VehicleBase(std::string const &company, std::string const &model, std::vector<std::string> const &others)
        : m_company(company), m_model(model), m_others(others) {}
    VehicleBase::VehicleBase(std::string &&company, std::string &&model, std::vector<std::string> &&others)
        : m_company(std::move(company)), m_model(std::move(model)), m_others(std::move(others)) {}
    VehicleBase::VehicleBase(VehicleBase const &) = default;
    VehicleBase::VehicleBase(VehicleBase &&) = default;
    VehicleBase::~VehicleBase() = default;

    // Members VehicleBase




    // Constructors Vehicle
    Vehicle::Vehicle(std::string const &company = "", std::string const &model = "", std::vector<std::string> const &others = {}, std::string const &product_id = "")
        : VehicleBase(company, model, others), m_product_id(product_id) {}
    Vehicle::Vehicle(std::string &&company = "", std::string &&model = "", std::vector<std::string> &&others = {}, std::string &&product_id = "")
        : VehicleBase(std::move(company), std::move(model), std::move(others)), m_product_id(std::move(product_id)) {}
    Vehicle::Vehicle(Vehicle const &vehicle) = default;
    Vehicle::Vehicle(Vehicle &&vehicle) = default;
    Vehicle::~Vehicle() = default;


    // Members Vehicle
    int Vehicle::add_from_file(std::istream &is)
    {
        int line_count = 0;
        while (!is.eof())
        {
            const int mem_count = 4;
            int mem_loaded = 0;
            int other_attr_loaded = 0;
            bool tab_flag = 0;
            bool break_flag = 0;
            std::string cur_line;

            for (char c = is.get(); c != '\n'; c = is.get())
            {
                cur_line.push_back(c);
            }
            if (cur_line[0] == '\t')
                tab_flag = 1;
            else
                tab_flag = 0;
            if(cur_line.empty())
                break_flag = 1;
            else
                break_flag = 0;
            line_count++;

            if (mem_loaded < mem_count)
            {
                switch (mem_loaded)
                {
                case 0:
                    if(tab_flag | break_flag)
                        throw parsing_error("Missing Comany field" , line_count);
                    m_company = cur_line;
                    mem_loaded++;
                    break;
                case 1:
                    if(tab_flag | break_flag)
                        throw parsing_error("Missing Comany field" , line_count);
                    m_model = cur_line;
                    mem_loaded++;
                    break;
                case 2:
                    if (tab_flag)
                    {
                        m_others[other_attr_loaded] = cur_line.substr(1);
                        other_attr_loaded++;
                    }
                    else
                        mem_loaded++;
                    break;
                case 3:
                    m_product_id = cur_line;
                    mem_loaded++;
                    break;
                }
            }
            if (break_flag)
                return line_count;
        }
        if (m_company.empty() || m_model.empty())
            throw parsing_error("File Does not contain critical information such as Company and Model");
        return line_count;
    }




    //Constructors DBMS
    DBMS::DBMS(std::set<Vehicle> const &vehicle_list)
        : m_database(vehicle_list) {}
    DBMS::DBMS(std::set<Vehicle> &&vehicle_list)
        : m_database(std::move(vehicle_list)){};
    DBMS::DBMS() = default;
    DBMS::~DBMS() = default;


    //Members DBMS
    Vehicle DBMS::make_vehicle(std::string const &company, std::string const &model, std::vector<std::string> const &others, std::string const &product_id = "NULL")
    {
        return Vehicle(company, model, others, product_id);
    }

    Vehicle DBMS::make_vehicle(std::string &&company, std::string &&model, std::vector<std::string> &&others, std::string &&product_id = "NULL")
    {
        return Vehicle(std::move(company), std::move(model), std::move(others), std::move(product_id));
    }

    int DBMS::add_from_file(std::istream & is)
    {
        int linecount
    }
}