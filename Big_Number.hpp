#pragma once

typedef unsigned char DATA_TYPE;

struct Big_Number {
	DATA_TYPE *a;
	int len, capacity;
	static const int PAS = 512;

	Big_Number();
	Big_Number(int);
	~Big_Number();

	void operator +=(const Big_Number &);
	void operator =(const Big_Number &);

	void increase_capacity();
	std::string to_string();
	void print();

};

#include "Big_Number.cpp"
