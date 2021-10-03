#pragma once

//#include "Big_Number.hpp"
#include <cstdlib>
#include <cstring>

Big_Number::Big_Number() {
	a = new(std::nothrow) DATA_TYPE[capacity = PAS];
	std::memset(a, 0, PAS);

	len = 1;
}

Big_Number::Big_Number(int x) {
	a = new(std::nothrow) DATA_TYPE[capacity = PAS];
	std::memset(a, 0, PAS);

	len = 0;
	do {
		// ? Optimization
		a[len++] = x % 10;
		x /= 10;
	} while (x);
}

Big_Number::~Big_Number() {
	delete[] a;
}

inline void Big_Number::operator += (const Big_Number &other) {
	DATA_TYPE carry = 0;

	len = std::max(len, other.len);
	for (int i = 0; i != len; i++) {
		a[i] += other.a[i] + carry;
		a[i] -= (carry = a[i] > (DATA_TYPE)9) * (DATA_TYPE)10;
	}

	if (len + carry >= capacity)
		increase_capacity();
	a[len] = carry;
	len += carry;
}

inline void Big_Number::increase_capacity() {
	a = (DATA_TYPE *) realloc(a, (capacity + PAS));
	memset(a + capacity, 0, PAS);
	capacity += PAS;
}

inline void Big_Number::print() {
	for (DATA_TYPE *it = a + len - 1; it != a; it--)
		std::cout << (int)*it;
	std::cout << (int)*a;
}

inline std::string Big_Number::to_string() {
	std::string ans;

	for (DATA_TYPE *it = a + len - 1; it != a; it--)
		ans += (int)*it + '0';
	ans += (int)*a + '0';

	return ans;
}
