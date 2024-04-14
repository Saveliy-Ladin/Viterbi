#pragma once
#include "Viterbi.h"
#include <iostream>

using namespace std;


///�� ������� ��� ������� ��������� ��� �����������!

void enter(string str, char delimiter, vector<uint8_t>& result) {

	vector<string> out;

	stringstream string_stream(str);

	string polynom;

	while (getline(string_stream, polynom, delimiter)) {
		out.push_back(polynom);
	}

	for (vector<string>::iterator it = out.begin(); it != out.end(); it++) {

		result.push_back(stoi(*it));

	}

}

int main() {

	setlocale(LC_ALL, "ru");

	vector<uint8_t> polynoms;

	string input;

	cout << "������� ����� ������ ������������ �������:"<<endl<<endl;

	getline(cin, input);

	enter(input,' ', polynoms);

	cout << "--------------------------------------------------" << endl << endl;

	try {

		Coder_Viterbi cv(0b00001110, polynoms);

		cv.print_result();

	}

	catch (const int ex) {

		cout << "������������ ������: ����� �������� ���������� ��������� ����� �������� ��������!" << endl;

	}

	
}