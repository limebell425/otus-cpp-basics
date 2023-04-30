#pragma once
#include <vector>
#include <string>
#include <optional>

class IStatistics 
{
public:
	virtual ~IStatistics()  = default;
	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char *name() const = 0;
};

class Min : public IStatistics 
{
public:
	Min() = default;
	void update(double next) override;
	double eval() const override;
	const char *name() const override;

private:
	std::optional<double> m_min;
};

class Max : public IStatistics 
{
public:
	Max() = default;
	void update(double next) override;
	double eval() const override;
	const char *name() const override;

private:
	std::optional<double> m_max;
};

class Mean : public IStatistics 
{
public:
	Mean();
	void update(double next) override;
	double eval() const override;
	const char *name() const override;

private:
	double sum;
	int data_size;
};

class Std : public IStatistics 
{
public:
	Std();
	void update(double next) override;
	double eval() const override;
	const char *name() const override;

private:
	double sum;
	std::vector<double> data;
	
};

class Percentile : public IStatistics 
{
public:
	Percentile(int persent);
	void update(double next) override;
	double eval() const override;
	const char *name() const override;

private:
	int persent;
	std::vector<double> data;
	std::string name_;
};