#pragma once

#include <fstream>
#include <cstdlib>
#include <cmath>

class Big_Integer {
public:
	Big_Integer();
	Big_Integer(const int);
	Big_Integer(const char *);
	~Big_Integer();

	Big_Integer& operator =(const Big_Integer &);

	void operator +=(const Big_Integer &);
	void operator *=(const Big_Integer &);

	Big_Integer operator +(const Big_Integer &);
	Big_Integer operator *(const Big_Integer &);

	friend std::ostream& operator <<(std::ostream &, const Big_Integer &);

	size_t size() const;

	//! Has to be freed
	friend void get_output(const Big_Integer &, int64_t *&, size_t &);
	
private:
	int32_t *table;
	size_t length;
	size_t capacity;
	static constexpr size_t STEP_SIZE = 512;
	static constexpr int32_t CELL_SIZE = 15;
	static constexpr int32_t CELL_BASE = 1 << CELL_SIZE;
	static constexpr int32_t CELL_MASK = CELL_BASE - 1;

	void increase_capacity();

};

Big_Integer::Big_Integer() {
	capacity = STEP_SIZE;
	table = (int32_t *) malloc(capacity * sizeof(int32_t));
	std::fill_n(table, capacity, (int32_t)0);
	length = 1;
}

Big_Integer::Big_Integer(int num) : Big_Integer() {
	if (num != 0)
		length = 0;
	
	while (num != 0) {
		table[length++] = num & CELL_MASK;
		num >>= CELL_SIZE;
	}
}

Big_Integer::~Big_Integer() {
	free(table);
}

void Big_Integer::increase_capacity() {
	this->capacity += STEP_SIZE;
	this->table = (int32_t *) realloc(this->table, capacity * sizeof(int32_t));
	std::fill_n(this->table + this->capacity - STEP_SIZE, STEP_SIZE, 0);
}

std::ostream& operator <<(std::ostream& fout, Big_Integer& num) {
	//? Array to hold output, base 10
	int64_t *output;
	size_t output_size;

	get_output(num, output, output_size);

	for (size_t i = 0; i < output_size; i++)
		fout << output[output_size - i - 1];

	free(output);
	return fout;
}

Big_Integer& Big_Integer::operator =(const Big_Integer& other) {
	if (this->capacity != other.capacity) {
		this->length = other.length;
		this->capacity = other.capacity;
		
		free(this->table);
		this->table = (int32_t *) calloc(this->capacity, sizeof(int32_t));
	}
	std::copy(other.table, other.table + other.capacity, this->table);
	return *this;
}

void Big_Integer::operator +=(const Big_Integer &other) {
	while (this->capacity < other.capacity)
		this->increase_capacity();

	int32_t carry = 0;

	for (size_t i = 0; i < std::max(other.length, this->length); i++) {
		this->table[i] += other.table[i] + carry;
		carry = this->table[i] >> Big_Integer::CELL_SIZE;
		this->table[i] &= Big_Integer::CELL_MASK;
	}
	
	if (this->length + carry >= capacity)
		this->increase_capacity();
	this->table[this->length] = carry;
	this->length += carry;
}

size_t Big_Integer::size() const {
	//? Array to hold output, base 10
	int64_t *output = NULL;
	size_t output_size = 0;

	get_output(*this, output, output_size);

	free(output);
	return output_size;
}

void get_output(const Big_Integer& num, int64_t*& output, size_t& output_size) {
	const float DIGIT_RATIO = std::log(1LL << num.CELL_SIZE) / std::log(10) + 1;
	const size_t TEMP_CAPACITY = num.capacity * DIGIT_RATIO;

	//? Array to hold output, base 10
	output = (int64_t *) calloc(TEMP_CAPACITY, sizeof(int64_t));
	output_size = 0;

	//? Array to hold output, base 10, powers of 2^CELL_SIZE
	int64_t *power = (int64_t *) calloc(TEMP_CAPACITY, sizeof(int64_t));
	size_t power_size = power[0] = 1;

	for (size_t i = 0; i < num.length; i++) {
		for (int j = 0; j < power_size; j++) {
			output[j] += num.table[i] * power[j];
		}

		for (size_t j = 0; j < power_size; j++) {
			int64_t tmp = output[j];
			output[j] %= 10;

			for (size_t l = j + 1; tmp > 9; l++) {
				output[l] += (tmp /= 10) % 10;
			}
		}
		output_size = power_size;

		while (output[output_size] != 0) {
			output[output_size + 1] += output[output_size] / 10;
			output[output_size] %= 10;
			output_size++;
		}

		if (i + 1 < num.length) {
			//? Calculate next power
			for (size_t j = 0; j < power_size; j++) {
				power[j] *= (1ULL << 15ULL);
			}

			for (size_t j = 0; j < power_size; j++) {
				int64_t tmp = power[j];
				power[j] %= 10;

				for (size_t l = j + 1; tmp > 9; l++) {
					power[l] += (tmp /= 10) % 10;
				}
			}

			while (power[power_size] != 0) {
				power[power_size + 1] += power[power_size] / 10 % 10;
				power[power_size] %= 10;
				power_size++;
			}
		}
	}

	output_size = TEMP_CAPACITY - 1;
	while (output_size > 0 && !output[output_size])
		output_size--;
	output_size++;

	free(power);
}
