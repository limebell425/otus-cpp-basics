#include "statistics.h"
#include <limits>
#include <cmath>

Min::Min(): m_min{std::numeric_limits<double>::max()} {
}

void Min::update(double next)
{
    if (next < m_min) {
    m_min = next;
    }
}

double Min::eval() const 
{
    return m_min;
}

const char *Min::name() const
{
    return "min";
}

Max::Max(): m_max{-std::numeric_limits<double>::max()} {
}

void Max::update(double next)
{
    if (next > m_max) {
        m_max = next;
    }  
}

double Max::eval() const 
{
    return m_max;
}


const char *Max::name() const
{
    return "max";
}

Mean::Mean() : sum{0}, data_size(0) {
}

void Mean::update(double next)
{
    sum += next;
    ++data_size;        
}

double Mean::eval() const 
{
    return sum/data_size;
}

const char *Mean::name() const
{
    return "mean";
}

Std::Std()
{
    sum = 0;
    data.reserve(100);
}

void Std::update(double next)
{
    data.push_back(next);
    sum += next; 
}

double Std::eval() const 
{
    if (data.size() < 2){
        return 0;
    }
    double mean = sum/data.size();
    double sum_of_dev = 0;
    for (const auto i : data){
        sum_of_dev += std::pow((i - mean), 2);
    }
    return std::sqrt(sum_of_dev/(data.size()-1));          
}

const char *Std::name() const
{
    return "std";
}

size_t Std::get_seq_size()
{
    return data.size();    
}