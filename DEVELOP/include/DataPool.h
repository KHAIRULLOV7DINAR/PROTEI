#ifndef DATAPOOL_H
#define DATAPOOL_H

#include <string>
#include <vector>
#include <deque>
#include <memory>

//Классы для хранения векторов
class BaseVector
{
public:
    virtual const std::string get_type() const = 0;

    virtual void print() const = 0;
};

template <typename T>
class TypedVector: public BaseVector
{
public:
    const int SIZE = 4;
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
    
    const std::vector<std::string>& get_allowed_types();

    void insert(std::unique_ptr<BaseVector> vec);
    std::unique_ptr<BaseVector> first(); 

    void print_vectors() const;

private:
    std::deque<std::unique_ptr<BaseVector>> pool_;
    std::vector<std::string> allowed_types_;
};

#endif