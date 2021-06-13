#include "dbms.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace dbms
{
    parsing_error::parsing_error(const char *e, int line_count = 0) : runtime_error(e), m_line_count(line_count) {}
    tmp_file_exists::tmp_file_exists(const char *e) : runtime_error(e) {};

    // Constructors VehicleBase
    VehicleBase::VehicleBase(std::string const &company, std::string const &model)
        : m_company(company), m_model(model) {}
    VehicleBase::VehicleBase(std::string &&company, std::string &&model)
        : m_company(std::move(company)), m_model(std::move(model)) {}
    VehicleBase::VehicleBase(VehicleBase const &) = default;
    VehicleBase::VehicleBase(VehicleBase &&) = default;
    VehicleBase::~VehicleBase() = default;
    //
    //

    // Members VehicleBase
    //
    //
    //
    //

    // Constructors Vehicle
    Vehicle::Vehicle(std::string const &company, std::string const &model, std::vector<std::string> const &others = {}, std::string const &product_id = "")
        : VehicleBase(company, model), m_others(others), m_product_id(product_id) {}
    Vehicle::Vehicle(std::string &&company = "", std::string &&model = "", std::vector<std::string> &&others = {}, std::string &&product_id = "")
        : VehicleBase(std::move(company), std::move(model)), m_others(std::move(others)), m_product_id(std::move(product_id)) {}
    Vehicle::Vehicle(Vehicle const &vehicle)
        : VehicleBase(vehicle), m_others(vehicle.m_others), m_product_id(vehicle.m_product_id) {}
    Vehicle::Vehicle(Vehicle &&vehicle)
        : VehicleBase(std::move(vehicle)), m_others(std::move(vehicle.m_others)), m_product_id(std::move(vehicle.m_product_id)) {}
    Vehicle::~Vehicle() = default;
    //
    //

    // Members Vehicle
    int Vehicle::import_from_file(std::stringstream &is, int al_read)
    {
        int line_count = 0;
        const int mem_count = 4;
        int mem_loaded = 0;
        int other_attr_loaded = 0;
        bool tab_flag = 0;
        bool break_flag = 0;
        std::string cur_line;
        auto is_tab = [&]()
        {
            int spaces = 0;
            for (auto &it : cur_line)
            {
                if (it == '\t')
                {
                    cur_line = cur_line.substr(1);
                    return true;
                }
                if (it == ' ')
                    spaces++;
                else
                    break;
            }
            if (spaces >= 4)
            {
                cur_line = cur_line.substr(spaces);
                return true;
            }
            return false;
        };

        while (!is.eof())
        {
            cur_line.clear(); for (char c = is.get(); c != '\n' && c != EOF; c = is.get()) {
                cur_line.push_back(c);
            }
            if (cur_line.empty())
                break_flag = 1;
            else
                break_flag = 0;
            line_count++;

            if (!cur_line.empty())
            {
                if (*cur_line.cbegin() == ';' && *(cur_line.cend() - 1) == ';')
                    continue;
            }

            if (mem_loaded < mem_count)
            {
                if (tab_flag == 1)
                {
                    tab_flag = is_tab();
                    if (tab_flag != 1)
                        mem_loaded++;
                }
                else
                    tab_flag = is_tab();

                switch (mem_loaded)
                {
                case 0:
                    if (tab_flag | break_flag)
                        throw parsing_error("Missing Comany field", line_count + al_read);
                    m_company = cur_line;
                    mem_loaded++;
                    break;
                case 1:
                    if (tab_flag | break_flag)
                        throw parsing_error("Missing Model field", line_count + al_read);
                    m_model = cur_line;
                    mem_loaded++;
                    break;
                case 2:
                    if (tab_flag)
                    {
                        m_others.push_back(cur_line);
                        other_attr_loaded++;
                    }
                    break;
                case 3:
                    if (tab_flag | break_flag)
                        throw parsing_error("Missing Id field", line_count + al_read);
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

    void Vehicle::add_to_file_buff(std::stringstream &ofile)
    {
        const int mem_count = 4;
        int mem_written = 0;
        int other_attr_written = 0;

        if (mem_written < mem_count)
        {
            switch (mem_written)
            {
            case 0:
                ofile << m_company << '\t';
                mem_written++;
                break;
            case 1:
                ofile << m_model << '\t';
                mem_written++;
                break;
            case 2:
                for (auto const &iter : m_others)
                {
                    ofile << '\t' << iter << '\n';
                    other_attr_written++;
                }
                mem_written++;
                break;
            case 3:
                ofile << m_product_id << '\t';
                mem_written++;
                break;
            }
        }
    }
    //
    //
    //
    //

    //Constructors DBMS
    DBMS::DBMS() = default;
    DBMS::DBMS(std::string filepath) noexcept(false) : m_filepath(filepath) , m_temp_filepath("."+ filepath +".temp")
    {
        if(std::filesystem::exists(m_temp_filepath))
            throw dbms::tmp_file_exists("Tempfile already exists. Maybe program was not closed properly.");
        import_from_file(filepath);
        f_file = 1;
    }
    DBMS::~DBMS()
    {
        f_exit = 1;
        m_futures_void.~vector();
        sync();
        if(f_save & f_file)
        {
            std::rename(m_temp_filepath.c_str() , m_filepath.c_str());
        }
        std::cout << "Exited";
    }
    //
    //

    //Members DBMS
    void DBMS::add(Vehicle &&vehicle)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if(m_id_index.end() ==  m_id_index.find(vehicle.m_product_id))
        {
            throw bad_add("Vehicle with the same ID found. Stop insertion.");
        }
        auto ret = m_database_data.emplace(new Vehicle(std::move(vehicle)));
        m_id_index.emplace(ret.first->get()->m_product_id, *ret.first);
        m_company_index.emplace(ret.first->get()->m_company , *ret.first);
        m_model_index.emplace(ret.first->get()->m_model , *ret.first);
        m_color_index.emplace(ret.first->get()->m_others[Vehicle::COLOR] , *ret.first);
    }
    void DBMS::print_stock() const
    {
        int pos = 1;
        for (auto &it : m_database_data)
        {
            std::cout << "Entry " << pos << '\n'
                      << "\t"
                      << "Company Name: " << it->m_company << "\n"
                      << "\t"
                      << "Model Name: " << it->m_model << "\n"
                      << "\t"
                      << "Other Attributes: "
                      << "\n";
            for (auto const &it2 : it->m_others)
                std::cout << "\t\t" << it2 << "\n";
            std::cout << "\t"
                      << "Vehicle ID: " << it->m_product_id << "\n";
            pos++;
        }
    }
    int DBMS::import_from_file(std::istream &ifile)
    {
        int lines_read = 0;
        std::vector<Vehicle> tmp_vec;
        std::stringstream temp_ss;
        temp_ss << ifile.rdbuf();
        while (!temp_ss.eof())
        {
            Vehicle temp;
            lines_read += temp.import_from_file(temp_ss, lines_read);
            tmp_vec.push_back(std::move(temp));
        }
        for (auto &it : tmp_vec)
        {
            try
            {
                add(std::move(it));
            }
            catch(const bad_add& e)
            {
                throw e;
            }
        }
        return lines_read;
    }
    int DBMS::import_from_file(std::string filepath)
    {
        std::ifstream ifile(filepath);
        if (ifile.peek() == EOF)
            throw parsing_error("EmptyFile");
        return import_from_file(ifile);
    }

    void DBMS::write_to_buf()
    {
        while(!(f_buf_flush | f_exit)){};
        f_buf_ready = 0;
        for(auto const & iter : m_database_data)
            iter->add_to_file_buff(m_file_buf);
        f_buf_ready = 1;
    }

    void DBMS::write_buf_to_file(std::string filepath)
    {
        while (!(f_buf_ready | f_exit))
        {};
        {
            f_buf_flush = 1;
            std::ofstream ofile(filepath); 
            ofile << m_file_buf.rdbuf();
            ofile.flush();
            f_buf_flush = 0;
        }
    }
    void DBMS::write_buf_to_file()
    {
        write_buf_to_file(m_temp_filepath);
    }
    void DBMS::sync()
    {
    }
    
    void DBMS::remove_vehicle(Vehicle::PtrWk & ptr)
    {

    }


    //
    //

    //Static Members
    Vehicle DBMS::make_vehicle(std::string const &company, std::string const &model, std::vector<std::string> const &others, std::string const &product_id)
    {
        return Vehicle(company, model, others, product_id);
    }

    Vehicle DBMS::make_vehicle(std::string &&company, std::string &&model, std::vector<std::string> &&others, std::string &&product_id)
    {
        return Vehicle(std::move(company), std::move(model), std::move(others), std::move(product_id));
    }
}