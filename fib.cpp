#include <iostream>
#include "my_time"
#include "Big_Number.hpp"

#define PRINT_LAST_NUMBER false
#define PRINT_ALL_NUMBERS false

#if PRINT_ALL_NUMBERS
	#include <fstream>
#endif

int main(int cargs, char *vargs[]) {
	if (cargs < 2) {
		std::cout << "Usage: fib <count>\n";
		std::cout << "   count == 0 => all numbers in 1 second\n";
		exit(1);
	}

	#if PRINT_ALL_NUMBERS
		std::ofstream fout("data/nums.out");
	#endif

	int count = atoi(vargs[1]);
	Big_Number a(1), b(0), tmp(0);

	if (count) { // Calculate 'count' fibonnaci numbers
		My_time my_time;
		for (int i = 1; i < count; i++) {
			tmp = a;
			a += b;
			b = tmp;

			#if PRINT_ALL_NUMBERS
				fout << a.to_string() << '\n';
			#endif
		}

		#if PRINT_ALL_NUMBERS
			std::cout << "\n\n";
		#endif
		std::cout << "Elapsed time: " << my_time.elapsed_time() << '\n';
		std::cout << "Last digit count: " << a.len << '\n';

		#if PRINT_LAST_NUMBER
			std::cout << count << "th fibonacci number:\n";
			a.print();
		#endif
	}
	else { // Calculate fibonnaci numbers for a number of seconds
		double wanted_time = 1.0;

		if (cargs >= 2) {
			wanted_time = atof(vargs[2]);

			if (wanted_time == 0.0)
				wanted_time = 1.0;
		}

		My_time my_time;
		for (count = 1; my_time.elapsed_time() < wanted_time; count++) {
			tmp = a;
			a += b;
			b = tmp;

			#if PRINT_ALL_NUMBERS
				fout << a.to_string() << '\n';
			#endif

		}

		std::cout << "Numbers calculated in " << wanted_time << (wanted_time == 1.0 ? " second: " : " seconds: ") << count << '\n';
		std::cout << "Last digit count: " << a.len << '\n';
	}

	return 0;
}
