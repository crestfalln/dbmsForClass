#ifndef DBMS_H
#define DBMS_H

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
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
    class tmp_file_exists : public std::runtime_error
    {
        public:
        tmp_file_exists();
        tmp_file_exists(const char *);
    };
    //
    //
    //

    class VehicleBase
    {
    protected:

        std::string m_company;
        std::string m_model;

        VehicleBase(std::string const &company, std::string const &model);
        VehicleBase(std::string &&company, std::string &&model);
        VehicleBase(VehicleBase const &);
        VehicleBase(VehicleBase &&);


        virtual ~VehicleBase();
    };
    //
    //
    //
    class Vehicle : public VehicleBase
    {
    protected:
        //Useful aliases
        using PtrSh = std::shared_ptr<Vehicle>;
        using PtrWk = std::weak_ptr<Vehicle>;

        friend class DBMS;

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
        int import_from_file(std::stringstream &, int);
        void add_to_file_buff(std::stringstream &);

    public:
        Vehicle(Vehicle const &);
        Vehicle(Vehicle &&);
        virtual ~Vehicle();

    private:
    };
    //
    //
    //
    class DBMS
    {
        //Useful Aliases
        using IndexStr = std::unordered_multimap<std::string, Vehicle::PtrWk>;
        using IndexIter = IndexStr::iterator;
        using ConstIndexIter = IndexStr::const_iterator;
        using IndexStrTwo = std::unordered_multimap<std::pair<std::string, std::string>, Vehicle::PtrWk>;
        using DataSet = std::unordered_set<Vehicle::PtrSh>;
        using DataSetIter = DataSet::iterator;
        using ConstDataSetIter = DataSet::const_iterator;


        DataSet m_database_data; //All Data uses shared pointers
        std::stringstream m_file_buf; //Acts as buffer for things to be written to file
        std::string m_filepath;
        std::string m_temp_filepath;
        std::fstream m_temp_file;
        std::vector<std::future<void>> m_futures_void;

        //Indexes for fast searching - unindexed search could work too - someone look into it
        IndexStr m_company_index;
        IndexStr m_model_index;
        IndexStr m_id_index;
        IndexStr m_color_index;

        //Flags for synchronising async calls; better ways should be interrogated
        bool
            f_exit = 0,
            f_alter = 0,
            f_save = 1,
            f_file = 0,
            f_buf_ready = 0,
            f_buf_flush = 0;


    public:
        static Vehicle make_vehicle(std::string const &company, std::string const &model, std::vector<std::string> const &others, std::string const &product_id);
        static Vehicle make_vehicle(std::string &&company, std::string &&model, std::vector<std::string> &&others, std::string &&product_id);
        DBMS();
        void add(Vehicle &&vehicle);

        // Filehandaling for tempfiles write, import, add.
        int import_from_file(std::istream &ifile);
        int import_from_file(std::string filepath);
        void write_to_buf(std::stringstream& outStream) ;
        void write_to_buf() ;
        void write_buf_to_file(std::string filepath);
        void write_buf_to_file();
        void sync();

        //Uses boost serailization; will be removed soon
        void import_from_xml(std::string filepath);
        void export_to_xml(std::string filepath);

        void print_stock() const;
        void alter_vehicle(DataSetIter);
        void alter_vehicle(IndexIter);
        void alter_vehicle(Vehicle::PtrSh &);
        void alter_vehicle(Vehicle::PtrWk &);
        void remove_vehicle(DataSetIter);
        void remove_vehicle(DataSetIter begin , DataSetIter end);
        void remove_vehicle(IndexIter);
        void remove_vehicle(Vehicle::PtrSh &);
        void remove_vehicle(Vehicle::PtrWk &);

        //Various Constructors
        DBMS(std::vector<Vehicle> const &vehicle_list);
        DBMS(std::vector<Vehicle> &&vehicle_list);
        DBMS(std::initializer_list<Vehicle> vehicle_list);
        DBMS(std::string filepath);
        ~DBMS();
    };
    //
    //
    //
}

#endif