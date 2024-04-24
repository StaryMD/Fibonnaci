#include <iostream>

#include "Big_Integer.cpp"
#include "my_time"

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

  if (count != 0) {  // Calculate 'count' fibonnaci numbers
    if (cargs >= 3 && std::string(vargs[2]) == "fast") {
      // Use matrix multiplication for calculating nth fib
      /*Matrix a(1, 0, 0, 0), b(1, 1, 1, 0);

      while (count) {
              if (count & 1)
                      a *= b;
              b *= b;
      }*/
    } else {  // Do it like a casual
      My_time my_time;
      Big_Integer a(1), b(0), c;

      for (int i = 0; i != count; i++) {
        c = a;
        a += b;
        b = c;

#if PRINT_ALL_NUMBERS
        fout << a << '\n';
#endif
      }

#if PRINT_ALL_NUMBERS
      // std::cout << "\n\n";
#endif
      std::cout << "Elapsed time: " << my_time.elapsed_time() << '\n';
      std::cout << "Last digit count: " << a.size() << '\n';

#if PRINT_LAST_NUMBER
      std::cout << count << "th fibonacci number:\n";
      std::cout << a << '\n';
#endif
    }
  } else {  // Calculate fibonnaci numbers for a number of seconds
    double wanted_time = 1.0;

    if (cargs >= 3) {
      wanted_time = atof(vargs[2]);

      if (wanted_time == 0.0) {
        wanted_time = 1.0;
      }
    }

    My_time timer;
    Big_Integer a(1), b(0), c;

    for (count = 0; timer.elapsed_time() < wanted_time; count++) {
      c = a;
      a += b;
      b = c;

#if PRINT_ALL_NUMBERS
      fout << a << '\n';
#endif
    }

    std::cout << "Numbers calculated in " << wanted_time
              << (wanted_time == 1.0 ? " second: " : " seconds: ") << count
              << '\n';
    std::cout << "Last digit count: " << a.size() << '\n';
  }
  return 0;
}
