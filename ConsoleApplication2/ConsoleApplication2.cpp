#include "Viterbi.h"

using namespace std;

char* enter() {
	char *ent = new char[30];

	cout << "Введите генерирующие полиномы через запятую:" << endl;
	cin.getline(ent, 30);
	
	return ent;
}

	


          //ЗАМЕТКА: написать исключение, когда пытаются ввести число больше размера регистра




	Viterbi::Viterbi(const vector<uint8_t>& polynoms) // Конструктор базового класса
	{
			this->polynoms = polynoms;
	}

	int Viterbi::get_high_bit_position(uint8_t number) // Получить значениe старшего бита
	{
			int result = 0;

			for (int i = 7; i != 0; i--)
			{
				if (static_cast<int>(get_bit(number, i)) == 1) {

					result = i + 1;
					break;
				}
			}

			return result;
	
	}

	uint8_t Viterbi::get_bit(uint8_t number, int index) // Получить значения бита по индексу
	{
			number = number & (1 << index);

			number = number >> index;

			return number;

	}

	uint8_t Viterbi::set_bit(uint8_t number, int index) // Установить бит по индексу в единицу
	{
		return number |(1 << index);
	}

	uint8_t Viterbi::reset_bit(uint8_t number, int index) // Установить бит по индексу в ноль
	{
		return number &~(1 << index);
	}

	uint8_t Viterbi::inverse_bit(uint8_t number, int index) // Инверсия бита по индексу
	{
		return number^(1<<index);
	}

	uint8_t Viterbi::inverse_number(uint8_t number, int size) // Инверсия кодируемого числа
	{

		uint8_t result = number;

		for (int i = 0, j = size-1; i != size / 2; ++i, --j)
		{
			if (get_bit(number, i) == get_bit(number, j)) {

				continue;
			}
			else {

				result = inverse_bit(result, i);

				result = inverse_bit(result, j );

			}
		}

		return result;
	}

	uint8_t Viterbi::inverse_code(uint8_t number, int size, const vector<uint8_t>& polynoms) // Инверсия кода
	{
		uint8_t result = number;

		for (int i = 0, j = size*polynoms.size()-1; i != (size * polynoms.size()) / 2; ++i, --j)
		{
			if (get_bit(number, i) == get_bit(number, j )) {

				continue;
			}
			else {

				result = inverse_bit(result, i);

				result = inverse_bit(result, j );

			}
		}

		return result;

	}

	uint8_t Viterbi::sum_bit(uint8_t number, const vector<uint8_t>& polynoms)  // Сумма битов регистра
	{
			uint8_t result = 0;

			uint8_t temperory_variable = 0;

			uint8_t n = 0; //rename!!!!!!!!

			for (int i = 0; i < polynoms.size(); i++)
			{
				n = number;

				n = n & polynoms[i];
			
				result = result << 1;

				for (int j = 0; j <get_registr_size(polynoms); j++)
				{
					temperory_variable = get_bit(n, j);
					result ^= temperory_variable;

				}

			}

			return result;
		
	}

	int Viterbi::get_registr_size(const vector<uint8_t>& polynoms) // Получить размер сдвивого регистра
	{

			int result = 0;

			auto max = max_element(polynoms.begin(), polynoms.end());

			result = get_high_bit_position(*max);

			return result;
		
	}

	Coder_Viterbi::Coder_Viterbi(uint8_t number, const vector<uint8_t>& polynoms) : Viterbi(polynoms) { // Конструктор кодера

		if (get_high_bit_position(number) > get_registr_size(polynoms)) 

			throw 1;

		
		


		this->number = inverse_number(number, get_registr_size(polynoms));

		this->register_size = get_registr_size(polynoms);

		

		coding();

	}


	uint16_t Coder_Viterbi::coding() // Процесс кодирования
	{		
		cout<<endl<< bitset<sizeof(uint8_t) * 8>(number)<<endl;
			
			uint16_t result = 0;
			int i = 0; 

			int divider = pow(2, register_size);

			uint8_t registr = number % divider;

			while (i < register_size) {

				result = result << polynoms.size();

				result |= sum_bit(registr % divider, polynoms);

				registr = registr << 1;

				i++;
			}

			this->result = inverse_code(result, register_size, polynoms);

			return inverse_code(result, register_size, polynoms);
			//return result;

	}

	void Coder_Viterbi::print_result() // Вывод результата кодирования в консоль
	{
		cout << "Coding result: " << bitset<sizeof(uint8_t) * 8>(inverse_number(number, register_size)) << " -> "<< bitset<sizeof(uint16_t) * 8>(result);

	}

	Decoder_Viterbi::Decoder_Viterbi(uint8_t number, const vector<uint8_t>& polynoms, int register_size) : Viterbi(polynoms) { // Конструктор декодера

		this->number = inverse_code(number, register_size, polynoms);

		this->register_size = register_size;

		Gird gird(register_size, this->number, polynoms);

		//gird.print_gird();

		gird.find_way();

		//gird.print_way();

	}

	void Decoder_Viterbi::Gird::Node::setter_result(uint8_t number, int index) { 

		this->result |= get_bit(number, index);

		this->result = this->result << 1;
	}

	void Decoder_Viterbi::Gird::Node::setter_left_value(uint8_t left_value) { this->left_value = left_value; }

	void Decoder_Viterbi::Gird::Node::setter_right_value(uint8_t right_value) { this->right_value = right_value; }

	void Decoder_Viterbi::Gird::Node::setter_number(uint8_t number) { this->number = number; }

	void Decoder_Viterbi::Gird::Node::sum_path(uint8_t path) { this->path += path; }

	uint8_t Decoder_Viterbi::Gird::Node::getter_left_value() { return left_value; }

	uint8_t Decoder_Viterbi::Gird::Node::getter_right_value() { return right_value; }

	uint8_t Decoder_Viterbi::Gird::Node::getter_number() { return number; }

	uint8_t Decoder_Viterbi::Gird::Node::getter_path() { return path; };

	uint8_t Decoder_Viterbi::Gird::Node::getter_result() {

		return result;
	}

	void Decoder_Viterbi::Gird::print_gird() {	// Вывод расчетной решетки в консоль

		int k = 0;

		for (auto it = map.begin(); it != map.end(); it++)
		{
			cout << k + 1 << "\'th backet" << endl;

			cout << "Number = " << bitset<sizeof(uint8_t) * 8>(it->getter_number()) << "| Left value = " << bitset<sizeof(uint8_t) * 8>(it->getter_left_value()) << "| Right value = " << bitset<sizeof(uint8_t) * 8>(it->getter_right_value()) << endl;

			cout << "----------------" << endl;

			k++;
		}

	}

	void Decoder_Viterbi::Gird::print_way() { // Вывод результата декодирования в консоль 

		uint8_t result = 0;

		for (vector<Node>::iterator it = map.begin(); it != map.end() - 1; it++)
		{
			if (it <= (it + 1)) {

				result = it->getter_result();
			}
			else
			{
				result = (it + 1)->getter_result();
			}

		}

		cout << "Result:" << bitset<sizeof(uint8_t) * 8>(result);

	}

	void Decoder_Viterbi::Gird::find_way() { // Декодирование по расчетной решетке

		uint8_t number = this->number;

		vector<uint8_t> result;

		for (int i = 0; i < get_registr_size(polynoms); i++) {

			uint8_t metric = number;

			metric = metric % 4;

			//cout << "----------------" << endl;

			for (vector<Node>::iterator j = map.begin(); j != map.end(); j++)
			{
				uint8_t metric = number;

				metric = metric % 4;

				if ((j->getter_left_value() ^ metric) + j->getter_path() <= (j->getter_right_value() ^ metric) + j->getter_path()) {

					j->sum_path((j->getter_left_value() ^ metric));

					j->setter_number(j->getter_number() >> 1);

					j->setter_number(sum_bit(reset_bit(j->getter_number(), get_registr_size(polynoms) - 2), polynoms));

					j->setter_result(j->getter_number(), get_registr_size(polynoms) - 2);

					j->setter_left_value(sum_bit(j->getter_number(), polynoms));

					j->setter_right_value(sum_bit(set_bit(j->getter_number(), get_registr_size(polynoms) - 1), polynoms));

					//cout << "Left:" << static_cast<int>(j->getterNumber()) << "\tpath:" << static_cast<int>(j->getterPath()) << endl;
				}
				else {

					j->sum_path((j->getter_right_value() ^ metric));

					j->setter_number(j->getter_number() >> 1);

					j->setter_number(set_bit(j->getter_number(), get_registr_size(polynoms) - 2));

					j->setter_result(j->getter_number(), get_registr_size(polynoms) - 2);

					j->setter_left_value(sum_bit(j->getter_number(), polynoms));

					j->setter_right_value(sum_bit(set_bit(j->getter_number(), get_registr_size(polynoms) - 1), polynoms));

					//cout << "Right:" << static_cast<int>(j->getterNumber()) << "\tpath:" << static_cast<int>(j->getterPath()) << endl;
				}

			}

			number = number >> polynoms.size();

			//cout << "----------------" << endl;
		}


	}

	Decoder_Viterbi::Gird::Gird(int register_size, uint8_t number, const vector<uint8_t>& polynoms) { // Конструктор расчетной решетки

		this->size = pow(2, register_size - 1);

		this->number = number;

		this->polynoms = polynoms;

		this->map.resize(size);

		int i = 0;

		for (vector<Node>::iterator j = map.begin(); j != map.end(); j++, i++) {

			uint8_t node = i;

			j->setter_number(node);

			j->setter_left_value(sum_bit(node, polynoms));

			j->setter_right_value(sum_bit(set_bit(node, register_size - 1), polynoms));

		}

	}



