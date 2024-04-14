#pragma once
#include "Viterbi.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); //дескриптор консоли

int main() {
	
	setlocale(LC_ALL, "ru");

	SetConsoleTitle(L"Viterbi");

	char ch;

	const char ESCAPE = 27, UP = 72, DOWN = 80, ENTER = 13;

	string Menu[] = { "1. Кодирование","2. Декодирование", "3. Выход"};

	int active_menu = 0;


	while (true) {

		system("CLS");

		for (int i = 0; i < size(Menu); i++) {

			if (i == active_menu)
				SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			else
				SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);

				cout << Menu[i] << endl;

		}

		ch = _getch();

		if (ch == -32) ch = _getch();

		switch (ch)
		{
		case ESCAPE:
			return 0;
		case UP:
			if(active_menu > 0)
			active_menu--;
			break;
		case DOWN:
			if (active_menu < 1 )
			active_menu++;
			break;
		case ENTER:

			switch (active_menu)
			{

			case 0: {

				system("CLS");

				uint8_t input_number = 0; string output_number; 

				string polynom_enter; vector<uint8_t> polynoms;

				cout << "Введите в двоичном виде число: ";
				
				cin >> output_number;

				cout << "Введите через пробел генерирующие полиномы: ";

				getline(cin, polynom_enter);

				try {

					for (int i = 0; i < size(output_number); i++) {

						for (int i = 0; i < size(output_number); i++) {

							if (output_number[i] == '1')
							{
								input_number |= 1;
								input_number <<= 1;

							}
							else if (output_number[i] == '0') {

								input_number <<= 1;

							}

							else throw '0';

						}
					}

				input_number >>= 1;

				getline(cin, polynom_enter);

				enter(polynom_enter, ' ', polynoms);

				

					Coder_Viterbi cv(input_number, polynoms);

					cv.print_result();

					cout << "\n\nНажмите любую клавишу, чтобы выйти в меню" << endl;
					_getch();

				}

				catch (const int ex) {

					cout << "\nНеккоректные данные: длина вводимой информации превышает длину сдвивого регистра!" << endl;
					cout << "\n\nНажмите любую клавишу, чтобы выйти в меню" << endl;
					_getch();

				}
				catch (const char ex) {


					cout << "\nНеккоректные данные: в водимой информации должны присутствовать только 0 или 1!" << endl;
					cout << "\n\nНажмите любую клавишу, чтобы выйти в меню" << endl;

					_getch();


				}

				break;

			}

			case 1: {

				system("CLS");

				int register_size = 0; vector<uint8_t> polynoms;

				uint16_t input_number = 0; string output_number; string polynom_enter;

				cout << "Введите код: ";

				cin >> output_number;

				cout << "\nВведите размер сдвигового регистра: ";
				cin >> register_size;

				cout << "\nВведите через пробел генерирующие полиномы\n:";

				getline(cin, polynom_enter);
				
				try {

					for (int i = 0; i < size(output_number); i++) {

						if (output_number[i] == '1')
						{
							input_number |= 1;
							input_number <<= 1;

						}
						else if (output_number[i] == '0') {

							input_number <<= 1;

						}

						else throw '0';

					}

					input_number >>= 1;


					getline(cin, polynom_enter);

					enter(polynom_enter, ' ', polynoms);

					Decoder_Viterbi dv(input_number, register_size, polynoms);

					cout << "\n\nНажмите любую клавишу, чтобы выйти в меню" << endl;
					_getch();
				}

				catch(const char ex)	{


					cout << "\nНеккоректные данные: в водимой информации должны присутствовать только 0 или 1!" << endl;
					cout << "\n\nНажмите любую клавишу, чтобы выйти в меню" << endl;

					_getch();
				

				}

				break;

			}
			

			}

			break;


		}
	}


	












	/*vector<uint8_t> polynoms;

	string input;

	cout << "Введите через пробел генерирующие пробелы:"<<endl<<endl;

	getline(cin, input);

	enter(input,' ', polynoms);

	cout << "--------------------------------------------------" << endl << endl;

	try {

		Coder_Viterbi cv(0b00001110, polynoms);

		cv.print_result();

	}

	catch (const int ex) {

		cout << "Неккоректные данные: длина вводимой информации превышает длину сдвивого регистра!" << endl;

	}*/

	
}