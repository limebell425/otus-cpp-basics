#include "statistics.h"
#include <limits>
#include <cmath>
#include <vector>


Min::Min() 
    : m_min{ std::numeric_limits<double>::max() }
{}

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

Max::Max()
    : m_max{ -std::numeric_limits<double>::max() }
{}

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
    return sum/data_size;
}

const char *Mean::name() const
{
    return "mean";
}

Std::Std(const std::vector<double> &data_) 
    : sum{0}
    , data{data_}
{}

void Std::update(double next)
{
    sum += next; 
}

double Std::eval() const 
{
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

Percentile::Percentile(int persent_, const std::vector<double> &data_)
    : persent{persent_}
    , data{data_}
{}

void Percentile::update(double)
{

}

double Percentile::eval() const 
{
    if (persent == 100) {
        return data.back();
    }
    double ind = persent / 100.0 * (data.size() - 1);
    double fraction = 0;
    return (data[ind] + modf(ind, &fraction) * (data[ind + 1] - data[ind]));
}

const char *Percentile::name() const
{
    return "percentile";
}