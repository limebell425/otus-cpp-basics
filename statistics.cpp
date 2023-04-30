#include "statistics.h"
#include <limits>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <exception>


void Min::update(double next)
{
    if (!m_min || next < *m_min) {
        m_min = next;
    }
}

double Min::eval() const 
{
    if (!m_min){
        throw std::exception("can't get min of empty sequence");
    }
    return *m_min;
}

const char *Min::name() const
{
    return "min";
}

void Max::update(double next)
{
    if (!m_max || next > *m_max) {
        m_max = next;
    }  
}

double Max::eval() const 
{
    if (!m_max){
        throw std::exception("can't get max of empty sequence");
    }
    return *m_max;
}


const char *Max::name() const
{
    return "max";
}

Mean::Mean() 
    : sum{0}
    , data_size{0} 
{}

void Mean::update(double next)
{
    sum += next;
    ++data_size;        
}

double Mean::eval() const 
{
    if (data_size == 0){
        throw std::exception("can't get mean of empty sequence");
    }
    return sum/data_size;
}

const char *Mean::name() const
{
    return "mean";
}

Std::Std() 
    : sum{0}
{}

void Std::update(double next)
{
    sum += next;
    data.push_back(next);
}

double Std::eval() const 
{
    if (data.empty()){
        throw std::exception("can't get mean of empty sequence");
    }
    double mean = sum/data.size();
    double sum_of_dev = 0;
    for (double i : data) {
        sum_of_dev += std::pow((i - mean), 2);
    }
    return std::sqrt(sum_of_dev/(data.size()));          
}

const char *Std::name() const
{
    return "std";
}

Percentile::Percentile(int persent_)
    : persent{persent_}
    , name_{"percentile" + std::to_string(persent_)}
{}

void Percentile::update(double next)
{
    data.insert(std::lower_bound(data.begin(), data.end(), next), next);
}

double Percentile::eval() const 
{
    if (data.empty()) {
        throw std::exception("can't get percentile of empty sequence");
    }
    if ((persent == 100) || (data.size() ==  1)) {
        return data.back();
    }
    double ind = persent / 100.0 * (data.size() - 1);
    double fraction = 0;
    return (data[ind] + modf(ind, &fraction) * (data[ind + 1] - data[ind]));
}

const char *Percentile::name() const
{
    return name_.c_str();
}