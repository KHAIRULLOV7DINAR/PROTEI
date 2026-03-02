#include <vector>
#include <iostream>
#include <algorithm>

#include "../include/DataPool.h"

//Вектор
template <typename T>
TypedVector<T>::TypedVector(std::string type, std::vector<T> vect)
{
    type_ = type;
    data_ = vect;
}

template <typename T>
std::string TypedVector<T>::get_type() const
{
    return type_;
}

template <typename T>
void TypedVector<T>::print() const
{
    int size = data_.size();
    for (int i = 0; i < size - 1; i++)
    {
        std::cout << data_[i] << "; ";
    }
    std::cout << data_[size - 1];
}

template <typename T>
bool TypedVector<T>::check_w() const
{
    return data_[data_.size() - 1] == T(0);
}

template <typename T>
const std::vector<T>& TypedVector<T>::get_data() const
{
    return data_;
}

template class TypedVector<int>;
template class TypedVector<float>;
template class TypedVector<double>;

//DataPool
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

const std::vector<std::string>& DataPool::get_allowed_types()
{
    return allowed_types_;
}