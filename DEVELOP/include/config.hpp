#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>

//Config for storaging all received data
struct Config
{
    // Maps for storing args and default values:
    std::shared_ptr<std::unordered_map<std::string, std::string>> string_values;
    std::shared_ptr<std::unordered_map<std::string, std::string>> default_values;

    //Vector for allowed vector types:
    std::shared_ptr<std::vector<std::string>> vector_types;

    //String values
    std::vector<unsigned char> ip;
    unsigned short port;
    std::string role;
    int i;
    std::string library;

    //CLI-Menu values
    std::string program_name;
    std::string type;

    //Structures for untyped vector
    struct VectorBase
    {
        virtual ~VectorBase() = default;
        virtual void clear() = 0;
    };
    
    template<typename T>
    struct TypedVector : VectorBase
    {
        std::vector<T> data;
        
        void clear() override
        {
            data.clear();
        }
    };

    std::unique_ptr<VectorBase> vect;

    Config() 
    {
        string_values = std::make_shared<std::unordered_map<std::string, std::string>>();
        default_values = std::make_shared<std::unordered_map<std::string, std::string>>();
        vector_types = std::make_shared<std::vector<std::string>>();
        
        *string_values = {
            {"-a", ""},
            {"-p", ""},
            {"-r", ""},
            {"-i", ""},
            {"-L", ""}
        };

        *default_values = {
            {"-a", "127.0.0.1"},
            {"-p", "5555"},
            {"-r", "Client"},
            {"-i", "0"},
            {"-L", "mylib"}
        };

        *vector_types = {
            "int", 
            "float",
            "double"
        };
    }

    void createTypedVector(const std::string& new_type)
    {
        if(new_type == "int")
        {
            vect = std::make_unique<TypedVector<int>>();
        }
        else if(new_type == "float")
        {
            vect = std::make_unique<TypedVector<float>>();
        }
        else if(new_type == "double")
        {
            vect = std::make_unique<TypedVector<double>>();
        }
        type = new_type;
    }
};

#endif