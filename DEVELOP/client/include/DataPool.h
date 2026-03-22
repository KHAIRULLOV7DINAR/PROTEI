#pragma once

#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <array>


//Классы для хранения векторов
class BaseVector
{
public:
    virtual ~BaseVector() = default; 
    virtual const std::string get_type() const = 0;

    virtual void print() const = 0;
};

template <typename T>
class TypedVector: public BaseVector
{
public:
    constexpr static size_t SIZE = 4;
    explicit TypedVector(std::string type, std::vector<T> vect) : type_(type), data_(vect){};

    const std::string get_type() const override;
    const std::vector<T>& get_data() const;

    void print() const override;

private:
    std::string type_;
    std::vector<T> data_;
};

//DataPool
class DataPool
{
/*
Инвариант класса:
все хранимые вектора имеют размерность = 4;
w-компонента векторов не равно 0;
типы данных векторов соответствуют int/float/double.
*/
public:
    DataPool() : allowed_types_{
        "int", 
        "float",
        "double"
    }{};
    
    const std::array<std::string, 3>& get_allowed_types();

    void insert(std::unique_ptr<BaseVector> vec);
    std::unique_ptr<BaseVector> first(); 

    void print_vectors() const;

private:
    std::deque<std::unique_ptr<BaseVector>> pool_;
    std::array<std::string, 3> allowed_types_;
};