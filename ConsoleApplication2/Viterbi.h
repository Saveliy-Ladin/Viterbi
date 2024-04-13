#pragma once
#include <iostream>
#include <bitset>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

class Viterbi {
protected: 

	uint16_t result = 0;

	uint8_t number = 0;

	int register_size = 0;

	vector<uint8_t> polynoms;

	Viterbi(const vector<uint8_t>& polynoms);

	static int get_high_bit_position(uint8_t num);

	static int get_registr_size(const vector<uint8_t>& polynoms);

	static uint8_t get_bit(uint8_t num, int index);

	static uint8_t set_bit(uint8_t num, int index);

	static uint8_t reset_bit(uint8_t num, int index);

	static uint8_t inverse_bit(uint8_t num, int index);

	static uint8_t inverse_number(uint8_t num, int size);

	static uint8_t inverse_code(uint8_t num, int size, const vector<uint8_t>& polynoms);

	static uint8_t sum_bit(uint8_t number, const vector<uint8_t>& polynoms);

};

class Coder_Viterbi : public Viterbi {
public:

	Coder_Viterbi(uint8_t number, const vector<uint8_t>& polynoms);

	uint16_t coding();

	void print_result();

};

class Decoder_Viterbi : public Viterbi {
public:

	class Gird {
	public:
		class Node;

		int size = 0;

		uint8_t number;

		vector <Node> map;

		vector <uint8_t> polynoms;

		vector <uint8_t> result;

		class Node {
		public:

			uint8_t result = 0;

			uint8_t number = 0;

			uint8_t left_value = 0;

			uint8_t right_value = 0;

			uint8_t path = 0;

		public:

			void setter_left_value(uint8_t left_value);

			void setter_right_value(uint8_t right_value);

			void setter_number(uint8_t number);

			void setter_result(uint8_t number, int index);

			void sum_path(uint8_t path);

			uint8_t getter_left_value();

			uint8_t getter_right_value();

			uint8_t getter_number();

			uint8_t getter_result();

			uint8_t getter_path();


		};


	public:

		void print_gird();

		void find_way();

		void print_way();

		Gird(int register_size, uint8_t number, const vector<uint8_t>& polynoms);

	};

public:

	Decoder_Viterbi(uint8_t number, const vector<uint8_t>& polynoms, int register_size);

};
