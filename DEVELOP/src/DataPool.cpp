#include <vector>
#include <iostream>
#include <algorithm>

#include "../include/DataPool.h"

//Вектор
template <typename T>
const std::string TypedVector<T>::get_type() const
{
    return type_;
}

template <typename T>
const std::vector<T>& TypedVector<T>::get_data() const
{
    return data_;
}

template <typename T>
void TypedVector<T>::print() const
{
    if(data_.size() == 0)
    {
        throw std::logic_error("No values in vector!");
    }

    std::cout << type_ << ": ";
    for (int i = 0; i < SIZE; i++)
    {
        if(i > 0)
        {
            std::cout << "; ";
        }
        std::cout << data_[i];
    }
    std::cout << std::endl;
}

template class TypedVector<int>;
template class TypedVector<float>;
template class TypedVector<double>;

//DataPool
const std::vector<std::string>& DataPool::get_allowed_types()
{
    return allowed_types_;
}

void DataPool::insert(std::unique_ptr<BaseVector> vec)
{
    if (!vec)
    {
        throw std::invalid_argument("Cannot insert null vector!");
    }

    pool_.push_back(std::move(vec));
}

std::unique_ptr<BaseVector> DataPool::first()
{
    if (pool_.empty())
    {
        throw std::logic_error("DataPool is empty!");
    }
    
    auto first = std::move(pool_.front());
    pool_.pop_front();
    return first;
}

void DataPool::print_vectors() const
{
    if(pool_.size() == 0)
    {
        std::cout << "\nNo vectors in data pool!\n\n";

        return;
    }

    std::cout << "\nVectors in pool:" << std::endl;
    for(const auto& p_vec : pool_)
    {
        p_vec->print();
    }
    std::cout << std::endl;
}