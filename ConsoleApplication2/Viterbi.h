#pragma once
#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <vector>

#include <Windows.h>

using namespace std;



void enter(string str, char delimitr, vector<uint8_t> & result);

 uint8_t get_bit(uint8_t num, int index);

 uint8_t set_bit(uint8_t num, int index);

 uint8_t reset_bit(uint8_t num, int index);

 uint8_t inverse_bit(uint8_t num, int index);

 uint8_t sum_bit(uint8_t number, const vector<uint8_t>& polynoms);

 int get_high_bit_position(uint8_t num);

 int get_register_size(const vector<uint8_t>& polynoms);

 int hamming_distance(uint8_t number, int size);

class Viterbi {
protected: 

	uint16_t result = 0;

	uint8_t number = 0;

	int register_size = 0;

	vector<uint8_t> polynoms;

	Viterbi(const vector<uint8_t>& polynoms);


	static uint8_t inverse_number(uint8_t num, int size);

	static uint8_t inverse_code(uint8_t num, int size, const vector<uint8_t>& polynoms);

	

};

class Coder_Viterbi : public Viterbi {
public:

	Coder_Viterbi(uint8_t number, vector<uint8_t> polynoms);

	uint16_t coding();

	void print_result();

};

class Decoder_Viterbi : public Viterbi {
public:

	class Gird {
	public:
		class Way;

		int size = 0;

		uint16_t number = 0;

		vector <Way> map;

		vector <uint8_t> polynoms;

		vector <uint8_t> result;

		class Way {
		public:

			uint8_t result = 0;

			uint8_t number = 0;

			uint8_t left_next_number = 0;

			uint8_t right_next_number = 0;

			uint8_t left_value = 0;

			uint8_t right_value = 0;

			uint8_t path = 0;

			uint8_t past_path = 0;

			uint8_t past_number = -1;

			uint8_t past_next_left_number = 0;

			uint8_t past_next_right_number = 0;

			uint8_t past_left_value = 0;

			uint8_t past_right_value = 0;

			


		public:

			void setter_left_next_number(uint8_t left_previos_number);
				
			uint8_t getter_left_next_number();

			uint8_t getter_right_next_number();

			void setter_right_next_number(uint8_t right_previos_number);

			void setter_left_value(uint8_t left_value);

			void setter_right_value(uint8_t right_value);

			void setter_number(uint8_t number);

			void setter_result(uint8_t number);

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

		void setter(uint16_t number, int register_size, const vector<uint8_t>& polynoms);
	};

public:

	Decoder_Viterbi(uint16_t number, int register_size, const vector<uint8_t>& polynoms);

	void print_result();

	Gird gird;
};
