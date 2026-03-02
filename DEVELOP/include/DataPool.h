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
    virtual std::string get_type() const = 0;
    virtual void print() const = 0;
    virtual bool check_w() const = 0;
};

template <typename T>
class TypedVector: public BaseVector
{
public:
    const int SIZE = 4;
    explicit TypedVector(std::string type, std::vector<T> vect);
    std::string get_type() const override;
    void print() const override;
    bool check_w() const override;

    const std::vector<T>& get_data() const;

private:
    std::string type_;
    std::vector<T> data_;
};

//DataPool
class DataPool
{
public:
    DataPool() : allowed_types_{
        "int", 
        "float",
        "double"
    }{};
    void insert(std::unique_ptr<BaseVector> vec);
    std::unique_ptr<BaseVector> first(); 
    const std::vector<std::string>& get_allowed_types();

private:
    std::deque<std::unique_ptr<BaseVector>> pool_;
    std::vector<std::string> allowed_types_;
};

#endif