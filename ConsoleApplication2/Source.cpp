#pragma once
#include "Viterbi.h"
#include <iostream>

using namespace std;

int main() {

	setlocale(LC_ALL, "ru");

	vector<uint8_t> p = { 15,13 };

	try {

		Coder_Viterbi cv(0b00001110, p);
		cv.print_result();
	}

	catch (const int ex) {

		cout << "Неккоректные данные: длина вводимой информации превышает длину сдвивого регистра!" << endl;

	}

	
}