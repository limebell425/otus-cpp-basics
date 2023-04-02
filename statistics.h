#pragma once
#include <vector>
class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;
};

class Min : public IStatistics {
public:
	Min();
	void update(double next) override;
	double eval() const override;
	const char * name() const override;
private:
	double m_min;
};

class Max : public IStatistics {
public:
	Max();
	void update(double next) override;
	double eval() const override;
	const char * name() const override;
private:
	double m_max;
};

class Mean : public IStatistics {
public:
	Mean();
	void update(double next) override;
	double eval() const override;
	const char * name() const override;
private:
	double sum;
	int data_size;
};

class Std : public IStatistics {
public:
	Std();
	void update(double next) override;
	double eval() const override;
	const char * name() const override;
	size_t get_seq_size();
private:
	std::vector<double>  data;
	double sum;
};
