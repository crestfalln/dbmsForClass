#include <map>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>

class Vehicle //Protected Class Never to be used outside class DBMS or class Catalouge
{
protected:
//Friends 
    friend class Catalouge;
    friend class DBMS;

//Some useful aliases used extensively throughout the program
    using Company = std::string;
    using Model = std::string;
    using Identifier = std::pair<Company, Model>;

//Main identifing Vars 
    Company m_company;
    Model m_model;
    Identifier m_identity;
    std::string m_product_id = "null";

//Operators
    Vehicle &operator=(Vehicle const &) = default;
    Vehicle &operator=(Vehicle &&) = default;

//Serialization-deserialization functions 
    std::string string_read(std::ifstream &ifile) // Deserialization helper function
    {
        size_t size;
        std::string str;
        ifile.read((char *)&size, sizeof(size_t));
        str.resize(size);
        ifile.read(str.data(), size);
        return str;
    }
    void string_write(std::ofstream &ofile, std::string const &str) // Serialization helper function
    {
        size_t size = str.size();
        ofile.write((char *)&size, sizeof(size_t));
        ofile.write(str.data(), size);
    }
    std::ofstream &dump_to_disk(std::ofstream &ofile) //Deserailization - Archive write
    {
        string_write(ofile , m_company);
        string_write(ofile , m_model);
        string_write(ofile , m_product_id);
        return ofile;
    }

//Constructors
    Vehicle() = delete;
    Vehicle(Vehicle const &) = default;
    Vehicle(Vehicle &&) = default;
    explicit Vehicle(std::string const &company, std::string const &model, std::string const &product_id = "null") 
        : m_company(company), m_model(model), m_identity({m_model, m_company}), m_product_id(product_id){};
    explicit Vehicle(Identifier const &model_id) 
        : m_company(model_id.first), m_model(model_id.second), m_identity(model_id){};
    explicit Vehicle(std::ifstream &ifile) noexcept //Deserialization - Archive read 
        : m_company(std::move(string_read(ifile))) , m_model(std::move(string_read(ifile))) ,  m_identity({m_company , m_model}) , m_product_id(std::move(string_read(ifile))) {};

public: //Public destructor because protected did not work with shared-pointer(some one look into it)
    ~Vehicle() = default;
};

class Catalouge
{
protected:
    friend class DBMS;
    using Company = Vehicle::Company;
    using Model = Vehicle::Model;
    using Identifier = Vehicle::Identifier;
    using IdentityHash = std::map<Identifier, Identifier>;
    std::map<Company, IdentityHash> m_catalouge;
    void print_catalouge() const
    {
        for (auto const &it : m_catalouge)
        {
            std::cout << it.first << ":\n";
            for (auto const &it2 : it.second)
                std::cout << '\t' << it2.second.second << '\n';
        }
        std::cout.flush();
    }
    void view_cataloge_by_company(Company const &comp) const
    {
        auto const &find_ret = m_catalouge.find(comp);
        if (find_ret == m_catalouge.cend())
        {
            std::cout << "Company not in catalouge";
            return;
        }
        std::cout << find_ret->first << '\n';
        for (auto const &it : find_ret->second)
        {
            std::cout << '\t' << it.second.second << '\n';
        }
        std::cout.flush();
        return;
    }
    int add_to_catalouge(Identifier const &identity)
    {
        auto const &m_cat_find_ret = m_catalouge.find(identity.first);
        if (m_cat_find_ret == m_catalouge.cend())
        {
            m_catalouge.insert({identity.first, {{identity, identity}}});
            return 0;
        }
        auto const &m_cat_model_find_ret = m_cat_find_ret->second.find(identity);
        if (m_cat_model_find_ret == m_cat_find_ret->second.cend())
        {
            m_cat_find_ret->second.insert({identity, identity});
            return 0;
        }
        return 1;
    }

    Catalouge(std::map<Company, IdentityHash> &&cat) noexcept
    {
        m_catalouge = std::move(cat);
        cat.~map();
    };
    explicit Catalouge(std::map<Company, IdentityHash> const &cat) noexcept
    {
        m_catalouge = cat;
    };
    explicit Catalouge(std::initializer_list<Identifier> cat_list) noexcept
    {
        for (auto &it : cat_list)
            add_to_catalouge(it);
    };
    Catalouge() = default;
    Catalouge(Catalouge const &) = delete;
    Catalouge(Catalouge &&) = delete;
    Catalouge &operator=(Catalouge const &) = delete;
    ~Catalouge() = default;
};
class DBMS
{
    using Company = Vehicle::Company;
    using Model = Vehicle::Model;
    using Identifier = Vehicle::Identifier;
    using IdentityHash = Catalouge::IdentityHash;
    using VehiclePtr = std::shared_ptr<Vehicle>;
    using IndexStr = std::map<std::string, VehiclePtr>;
    Catalouge m_cat;
    std::map<Identifier, std::vector<VehiclePtr>> m_stock;
    IndexStr color_index;

public:
    void print_catalouge()
    {
        m_cat.print_catalouge();
    }
    int add_to_stock(Company const &company, Model const &model, std::string const &product_id = "null")
    {
        Identifier identity = {company, model};
        m_cat.add_to_catalouge(identity);
        auto m_stock_ins_pos = m_stock.insert({identity, {}}).first;
        m_stock_ins_pos->second.emplace_back(new Vehicle(company, model, product_id));
        return 0;
    }
    int add_to_stock(Vehicle const &vehicle)
    {
        add_to_stock(vehicle.m_company, vehicle.m_model, vehicle.m_product_id);
        return 0;
    }
    int amt_in_stock(Company const &company, Model const &model)
    {
        auto const &m_stock_find_ret = m_stock.find({company, model});
        return m_stock_find_ret->second.size();
    }
    size_t total_vehicles_in_stock()
    {
        size_t total_vehicles = 0;
        for (auto const &iter_m_stock : m_stock)
        {
            for (auto const &iter_m_stock_in_vec : iter_m_stock.second)
                total_vehicles++;
        }
        return total_vehicles;
    }
    std::ofstream &dump_to_disk(std::ofstream &ofile)
    {

        size_t total_vehicles = total_vehicles_in_stock();
        ofile.write((char *)&total_vehicles, sizeof(size_t));

        for (auto const &iter_m_stock : m_stock)
        {
            for (auto const &iter_m_stock_in_vec : iter_m_stock.second)
                iter_m_stock_in_vec->dump_to_disk(ofile);
        }
        return ofile;
    }
    DBMS(std::ifstream &ifile) : DBMS()
    {
        if (ifile.peek() == std::ifstream::traits_type::eof())
            return;
        size_t total_vehicles;
        ifile.read((char *)&total_vehicles, sizeof(size_t));
        for (int file_vehicle_iter = 0; file_vehicle_iter < total_vehicles; file_vehicle_iter++)
            add_to_stock(Vehicle(ifile));
    }
    DBMS() = default;
};

int main()
{
    std::ifstream ifile("datin.0", std::ios_base::binary);
    DBMS dbms(ifile);
    ifile.close();
    /*
    dbms.add_to_stock("Honda" , "Civic");
    dbms.add_to_stock("Honda" , "Civic");
    dbms.add_to_stock("Tesla" , "Model X");
    dbms.add_to_stock("Tesla" , "Model X");
    dbms.add_to_stock("Tesla" , "Model y");
    dbms.add_to_stock("Tesla" , "Model z");
    dbms.add_to_stock("Tesla" , "Model q");
    */
    dbms.print_catalouge();
    std::cout << dbms.amt_in_stock("Tesla", "Model X");
    std::ofstream ofile("datout.0", std::ios_base::binary);
    dbms.dump_to_disk(ofile);
}