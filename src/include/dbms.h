#ifndef DBMS_H

#define DBMS_H
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <sstream>
#include <future>
#include <mutex>

namespace dbms
{
    class parsing_error : public std::runtime_error
    {
    public:
        int m_line_count;
        parsing_error(const char[], int line_count);
    };
    //
    //
    //
    class VehicleBase
    {
    protected:
        friend class boost::serialization::access;

        std::string m_company;
        std::string m_model;

        VehicleBase(std::string const &company, std::string const &model);
        VehicleBase(std::string &&company, std::string &&model);
        VehicleBase(VehicleBase const &);
        VehicleBase(VehicleBase &&);

        template <class Archive>
        void serialize(Archive &, const unsigned int);

        virtual ~VehicleBase();
    };
    //
    //
    //
    class Vehicle : public VehicleBase
    {
    protected:
        using PtrSh = std::shared_ptr<Vehicle>;
        using PtrWk = std::weak_ptr<Vehicle>;

        friend class boost::serialization::access;
        friend class DBMS;
        friend class catalouge;

        enum
        {
            COLOR = 0,
            ENGINE,
            PURCHASE_PRICE
        };
        std::vector<std::string> m_others;
        std::string m_product_id;

        Vehicle(std::string const &company, std::string const &model, std::vector<std::string> const &others, std::string const &product_id);
        Vehicle(std::string &&company, std::string &&model, std::vector<std::string> &&others, std::string &&product_id);
        Vehicle &operator=(Vehicle const &);
        Vehicle &operator=(Vehicle &&) noexcept;
        int add_from_file(std::stringstream &, int);
        int add_to_file_buff(std::stringstream &);

    public:
        Vehicle(Vehicle const &);
        Vehicle(Vehicle &&);
        virtual ~Vehicle();

    private:
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version);
    };
    //
    //
    //
    class DBMS
    {
        using IndexStr = std::unordered_multimap<std::string, Vehicle::PtrWk>;
        using IndexIter = IndexStr::iterator;
        using ConstIndexIter = IndexStr::const_iterator;
        using IndexStrTwo = std::unordered_multimap<std::pair<std::string, std::string>, Vehicle::PtrWk>;

        friend class boost::serialization::access;
        std::unordered_set<Vehicle::PtrSh> m_database_data;
        std::stringstream m_file_buf;

        IndexStr m_company_index;
        IndexStr m_model_index;
        IndexStr m_id_index;
        IndexStr m_color_index;

        bool
            f_change = 0,
            f_kill = 0,
            f_buf_flush = 0;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version);

    public:
        static Vehicle make_vehicle(std::string const &company, std::string const &model, std::vector<std::string> const &others, std::string const &product_id);
        static Vehicle make_vehicle(std::string &&company, std::string &&model, std::vector<std::string> &&others, std::string &&product_id);
        DBMS();
        void add(Vehicle &&vehicle);
        // Filehandaling for tempfiles write, import, add.
        int add_from_file(std::istream &ifile);
        int add_from_file(std::string filepath);
        int export_to_file(std::ostream &ofile) const;
        int export_to_file(std::string filepath) const;

        void import_from_xml(std::string filepath);
        void export_to_xml(std::string filepath);

        void print_stock() const;

        DBMS(std::vector<Vehicle> const &vehicle_list);
        DBMS(std::vector<Vehicle> &&vehicle_list);
        DBMS(std::initializer_list<Vehicle> vehicle_list);
        DBMS(std::istream &ifile);
        ~DBMS();
    };
    //
    //
    //
    //templates
    template <class Archive>
    void VehicleBase::serialize(Archive &ar, unsigned const int version)
    {
        ar &BOOST_SERIALIZATION_NVP(m_company);
        ar &BOOST_SERIALIZATION_NVP(m_model);
    }

    template <class Archive>
    void Vehicle::serialize(Archive &ar, const unsigned int version)
    {
        ar &BOOST_SERIALIZATION_BASE_OBJECT_NVP(VehicleBase);
        ar &BOOST_SERIALIZATION_NVP(m_others);
        ar &BOOST_SERIALIZATION_NVP(m_product_id);
    }

    template <class Archive>
    void DBMS::serialize(Archive &ar, unsigned const int version)
    {
        ar &BOOST_SERIALIZATION_NVP(m_database_data);
    }

}

#endif