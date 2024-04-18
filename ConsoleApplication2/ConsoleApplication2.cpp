#include "Viterbi.h"

using namespace std;

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

	Viterbi::Viterbi(const vector<uint8_t>& polynoms) // Конструктор базового класса
	{
			this->polynoms = polynoms;
	}

	int get_high_bit_position(uint8_t number) // Получить значениe старшего бита
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

	uint8_t sum_bit(uint8_t number, const vector<uint8_t>& polynoms)  // Сумма битов регистра
	{
			uint8_t result = 0;

			uint8_t temperory_variable = 0;

			uint8_t n = 0; //rename!!!!!!!!

			for (int i = 0; i < polynoms.size(); i++)
			{
				n = number;

				n = n & polynoms[i];
			
				result = result << 1;

				for (int j = 0; j <get_register_size(polynoms); j++)
				{
					temperory_variable = get_bit(n, j);
					result ^= temperory_variable;

				}

			}

			return result;

	}

	int get_register_size(const vector<uint8_t>& polynoms) // Получить размер сдвивого регистра
	{

			int result = 0;

			auto max = max_element(polynoms.begin(), polynoms.end());

			result = get_high_bit_position(*max);

			return result;
		
	}

	int hamming_distance(uint8_t number, int size)
	{
		int result = 0; int i = 0;
		
		while (i < size)
		{
			if (get_bit(number, i) == 1) { result++; }

			i++;
		}

		return result;
	}

	Coder_Viterbi::Coder_Viterbi(uint8_t number, vector<uint8_t> polynoms) : Viterbi(polynoms) { // Конструктор кодера

		if(get_high_bit_position(number) > get_register_size(polynoms)) 
		
			throw 1;
		
		this->number = inverse_number(number, get_register_size(polynoms));

		this->register_size = get_register_size(polynoms);
		
		coding();

	}

	uint16_t Coder_Viterbi::coding() // Процесс кодирования
	{		
			reverse(polynoms.begin(), polynoms.end());

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

	}

	void Coder_Viterbi::print_result() // Вывод результата кодирования в консоль
	{
		cout << "Результат кодирования: " << bitset<sizeof(uint8_t) * 8>(inverse_number(number, register_size)) << " -> "<< bitset<sizeof(uint16_t) * 8>(result);

	}

	Decoder_Viterbi::Decoder_Viterbi(uint16_t number, int register_size, const vector<uint8_t>& polynoms) : Viterbi(polynoms) { // Конструктор декодера
		
		// исключение через подсчет символа вводимой строки

		//cout << bitset<sizeof(uint8_t)*8>(number) << endl;

		this->number = inverse_code(number, register_size, polynoms);


		//cout << bitset<sizeof(uint8_t) * 8>(this->number) << endl;

		this->register_size = register_size;

		gird.setter(this->number, register_size, polynoms);


		gird.find_way();

	}

	void Decoder_Viterbi::print_result()
	{
		gird.print_way();
	}

	void Decoder_Viterbi::Gird::Way::setter_result(uint8_t number) { 

		this->result = this->result << 1;

		this->result |= number;

		
	}

	void Decoder_Viterbi::Gird::Way::setter_left_next_number(uint8_t left_previos_number) { this->left_next_number = left_previos_number; }

	uint8_t Decoder_Viterbi::Gird::Way::getter_left_next_number(){ return left_next_number; }

	uint8_t Decoder_Viterbi::Gird::Way::getter_right_next_number(){	return right_next_number; }

	void Decoder_Viterbi::Gird::Way::setter_right_next_number(uint8_t right_previos_number) { this->right_next_number = right_previos_number; }

	void Decoder_Viterbi::Gird::Way::setter_left_value(uint8_t left_value) { this->left_value = left_value; }

	void Decoder_Viterbi::Gird::Way::setter_right_value(uint8_t right_value) { this->right_value = right_value; }

	void Decoder_Viterbi::Gird::Way::setter_number(uint8_t number) { this->number = number; }

	void Decoder_Viterbi::Gird::Way::sum_path(uint8_t path) {
		
		this->past_path = this->path;
		
		this->path += path; 
	
	}

	uint8_t Decoder_Viterbi::Gird::Way::getter_left_value() { return left_value; }

	uint8_t Decoder_Viterbi::Gird::Way::getter_right_value() { return right_value; }

	uint8_t Decoder_Viterbi::Gird::Way::getter_number() { return number; }

	uint8_t Decoder_Viterbi::Gird::Way::getter_path() { return path; }

	uint8_t Decoder_Viterbi::Gird::Way::getter_result() {

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

		auto min = map.begin();
		
		for (vector<Way>::iterator it = map.begin(); it != map.end() ; it++)
		{
			if (it->path < min->path) {


				min = it;
			}
			
		}

		cout << "\nРезультат декодирования: " << bitset<sizeof(uint8_t) * 8>(inverse_number(number, get_register_size(polynoms)* polynoms.size())) << " -> " << bitset<sizeof(uint16_t) * 8>(min->result);
	
	}

	void Decoder_Viterbi::Gird::setter(uint16_t number, int register_size, const vector<uint8_t>& polynoms)
	{
		this->size = pow(2, register_size - 1);

		this->number = number;

		this->polynoms = polynoms;

		this->map.resize(size);

		int i = 0;

		for (vector<Way>::iterator j = map.begin(); j != map.end(); j++, i++) {

			uint8_t way = i;

			j->number = way;

			j->left_next_number = way >> 1;

			j->right_next_number = set_bit(way >> 1, get_register_size(polynoms) - 2);
		
			j->left_value = sum_bit(way, polynoms);

			j->right_value = sum_bit(set_bit( way, get_register_size(polynoms) - 1), polynoms) ;

		}

	}

	void Decoder_Viterbi::Gird::find_way() { // Декодирование по расчетной решетке

		uint8_t number = this->number;

		vector<uint8_t> result; int divider = pow(2, polynoms.size());  

		for (int i = 0; i < get_register_size(polynoms); i++) {

			uint8_t metric = number;

			metric = metric % divider;

			metric = inverse_number(metric,polynoms.size()); // метрика на каждом шаге


			for (vector<Way>::iterator this_way = map.begin(); this_way != map.end(); this_way++)
			{			

				if (this_way->number % 2 == 0) {			// Декодирование происходит путем разбияния чисел на четные и нечетные пары: 000 и 001, 010 и 011 и т.д.

					for (vector<Way>::iterator neighbour_way = map.begin(); neighbour_way != map.end(); neighbour_way++)
					{
						if (this_way->number + 1 == neighbour_way->number) { // Условия нахождения соседней пары для четного числа

							if (this_way->left_value == neighbour_way->left_value && this_way->right_value == neighbour_way->right_value) {  // Условие, если соседний путь был перестроен

								for (int i = 0; i < polynoms.size(); i++) {

									neighbour_way->left_value = inverse_bit(neighbour_way->left_value, i);

									neighbour_way->right_value = inverse_bit(neighbour_way->right_value, i);

								}
							}

							if ( this_way->path + hamming_distance(this_way->left_value ^ metric, polynoms.size()) <= neighbour_way->path + hamming_distance(neighbour_way->left_value ^ metric, polynoms.size())) { // Условие расчета метрик ветвей

								this_way->past_path = this_way->path;

								this_way->path += hamming_distance(this_way->left_value ^ metric, polynoms.size());

								this_way->past_number = this_way->number;

								this_way->past_next_left_number = this_way->left_next_number;

								this_way->past_next_right_number = this_way->right_next_number;

								this_way->past_left_value = this_way->left_value;

								this_way->past_right_value = this_way->right_value;

								this_way->number = this_way->left_next_number;

								this_way->setter_result(get_bit(this_way->number, get_register_size(polynoms) - 2));

								this_way->left_next_number = this_way->number >> 1;

								this_way->right_next_number = set_bit(this_way->number >> 1, get_register_size(polynoms) - 2);

								this_way->left_value = sum_bit(this_way->number, polynoms);

								this_way->right_value = sum_bit(set_bit(this_way->number, get_register_size(polynoms) - 1), polynoms);

								break;

							}
							else if (this_way->path + (hamming_distance(this_way->right_value ^ metric, polynoms.size())) <= neighbour_way->path + hamming_distance(neighbour_way->right_value ^ metric, polynoms.size())) { // Условие расчета метрик ветвей

								this_way->past_path = this_way->path;

								this_way->path += hamming_distance(this_way->right_value ^ metric, polynoms.size());

								this_way->past_number = this_way->number;

								this_way->past_next_left_number = this_way->left_next_number;

								this_way->past_next_right_number = this_way->right_next_number;

								this_way->past_left_value = this_way->left_value;

								this_way->past_right_value = this_way->right_value;

								this_way->number = this_way->right_next_number;

								this_way->setter_result(get_bit(this_way->number, get_register_size(polynoms) - 2));

								this_way-> left_next_number = this_way->number >> 1;

								this_way-> right_next_number = set_bit(this_way->number >> 1, get_register_size(polynoms) - 2);

								this_way->left_value = sum_bit(this_way->number, polynoms);

								this_way->right_value = sum_bit(set_bit(this_way->number, get_register_size(polynoms) - 1), polynoms);

								break;
							}

							else { // Условие для перестроения пути, если ни одна из веток не прошла предыдущие условия

								this_way->number = neighbour_way->number;

								this_way->result = neighbour_way->result;

								this_way->left_next_number = this_way->number >> 1;

								this_way->right_next_number = set_bit(this_way->number >> 1, get_register_size(polynoms) - 2);

								this_way->left_value = sum_bit(this_way->number, polynoms);

								this_way->right_value = sum_bit(set_bit(this_way->number, get_register_size(polynoms) - 1), polynoms);
								

								if (this_way->path + hamming_distance(this_way->left_value ^ metric, polynoms.size()) >= (this_way->path + hamming_distance(this_way->right_value ^ metric, polynoms.size()))) { ///////////////////////////////////////////////////////

									this_way->path += hamming_distance(this_way->left_value ^ metric, polynoms.size());

									this_way->past_number = this_way->number;

									this_way->past_next_left_number = this_way->left_next_number;

									this_way->past_next_right_number = this_way->right_next_number;

									this_way->past_left_value = this_way->left_value;

									this_way->past_right_value = this_way->right_value;

									this_way->number = this_way->left_next_number;

									this_way->setter_result(get_bit(this_way->number, get_register_size(polynoms) - 2));

									this_way->left_next_number = this_way->number >> 1;

									this_way->right_next_number = set_bit(this_way->number >> 1, get_register_size(polynoms) - 2);

									this_way->left_value = sum_bit(this_way->number, polynoms);

									this_way->right_value = sum_bit(set_bit(this_way->number, get_register_size(polynoms) - 1), polynoms);

									break;

								}
								else if (this_way->path + (hamming_distance(this_way->right_value ^ metric, polynoms.size())) >= this_way->path + hamming_distance(this_way->left_value ^ metric, polynoms.size())) { ////////////////////

									this_way->path += hamming_distance(this_way->right_value ^ metric, polynoms.size());

									this_way->past_number = this_way->number;

									this_way->past_next_left_number = this_way->left_next_number;

									this_way->past_next_right_number = this_way->right_next_number;

									this_way->past_left_value = this_way->left_value;

									this_way->past_right_value = this_way->right_value;

									this_way->number = this_way->right_next_number;

									this_way->setter_result(get_bit(this_way->number, get_register_size(polynoms) - 2));

									this_way->left_next_number = this_way->number >> 1;

									this_way->right_next_number = set_bit(this_way->number >> 1, get_register_size(polynoms) - 2);

									this_way->left_value = sum_bit(this_way->number, polynoms);

									this_way->right_value = sum_bit(set_bit(this_way->number, get_register_size(polynoms) - 1), polynoms);

									break;
								}

								break;

							}

						}

					}

					continue;

				}

				else if (this_way->number % 2 != 0) { //Нечетная пара

					for (vector<Way>::iterator neighbour_way = map.begin(); neighbour_way != map.end(); neighbour_way++)
					{
						
					if(this_way->left_next_number == neighbour_way->past_next_left_number && this_way->right_next_number == neighbour_way->past_next_right_number) { // поиск пары по её предыдущим значения ветвей


						if (this_way->left_value == neighbour_way->past_left_value && this_way->right_value == neighbour_way->past_right_value) {

							neighbour_way->past_number = this_way->number - 1;

							for (int i = 0; i < polynoms.size(); i++) {

								neighbour_way->past_left_value = inverse_bit(neighbour_way->past_left_value,i);

								neighbour_way->past_right_value = inverse_bit(neighbour_way->past_right_value, i);			

							}
						}

						if (this_way->path + hamming_distance(this_way->left_value ^ metric, polynoms.size()) < neighbour_way->past_path + hamming_distance(neighbour_way->past_left_value ^ metric, polynoms.size())) { //////////////
							
							this_way->past_path = this_way->path;

							this_way->path += hamming_distance(this_way->left_value ^ metric, polynoms.size());
						
							this_way->past_number = this_way->number;

							this_way->past_next_left_number = this_way->left_next_number;

							this_way->past_next_right_number = this_way->right_next_number;

							this_way->number = this_way->left_next_number;

							this_way->setter_result(get_bit(this_way->number, get_register_size(polynoms) - 2));

							this_way->left_next_number = this_way->number >> 1;

							this_way->right_next_number = set_bit(this_way->number >> 1, get_register_size(polynoms) - 2);

							this_way->left_value = sum_bit(this_way->number, polynoms);

							this_way->right_value = sum_bit(set_bit(this_way->number, get_register_size(polynoms) - 1), polynoms);

							break;
						}
						else if (this_way->path + (hamming_distance(this_way->right_value ^ metric, polynoms.size())) <= neighbour_way->past_path + hamming_distance(neighbour_way->past_right_value ^ metric, polynoms.size())) {


							this_way->past_path = this_way->path;

							this_way->path += hamming_distance(this_way->right_value ^ metric, polynoms.size());

							this_way->past_number = this_way->number;

							this_way->past_next_left_number = this_way->left_next_number;

							this_way->past_next_right_number = this_way->right_next_number;

							this_way->number = this_way->right_next_number;

							this_way->setter_result(get_bit(this_way->number, get_register_size(polynoms) - 2));

							this_way->left_next_number = this_way->number >> 1;

							this_way->right_next_number = set_bit(this_way->number >> 1, get_register_size(polynoms) - 2);

							this_way->left_value = sum_bit(this_way->number, polynoms);

							this_way->right_value = sum_bit(set_bit(this_way->number, get_register_size(polynoms) - 1), polynoms);

							break;

						}

						else {

							if (neighbour_way->number == neighbour_way->past_next_left_number) {

								this_way->number = neighbour_way->past_next_right_number;

								this_way->path = neighbour_way->past_path + hamming_distance(neighbour_way->past_right_value ^ metric, polynoms.size());

								this_way->result = neighbour_way->result;

							}
							else if (neighbour_way->number == neighbour_way->past_next_right_number) {
							
								this_way->number = neighbour_way->past_next_left_number;

								this_way->path = neighbour_way->past_path + hamming_distance(neighbour_way->past_left_value ^ metric, polynoms.size());

								this_way->result = neighbour_way->result;
							}

							this_way->setter_result(get_bit(this_way->number, get_register_size(polynoms) - 2));

							this_way->left_next_number = neighbour_way->left_next_number;

							this_way->right_next_number = neighbour_way->right_next_number;

							this_way->left_value = neighbour_way->left_value;

							this_way->right_value = neighbour_way->right_value;

							this_way->past_number = neighbour_way->past_number;

							this_way->past_next_left_number = neighbour_way->past_next_left_number;

							this_way->past_next_right_number = neighbour_way->past_next_right_number;

							break;

						}

					}
		
					}

					continue;

				}

			}

			//for (vector<Way>::iterator j = map.begin(); j != map.end(); j++)
			//{

			//	cout << "\nNumber: " << bitset<sizeof(uint8_t) * 8>(j->getter_number()) << endl;
			//	cout << "\n Path: " << static_cast<int>(j->getter_path()) << endl;
			//	cout << "\n Result: " << bitset<sizeof(uint8_t) * 8>(j->getter_result()) << endl;
			//}

				number = number >> polynoms.size();

		}
	}

	uint8_t get_bit(uint8_t number, int index) // Получить значения бита по индексу
	{
	
		number = number & (1 << index);

		number = number >> index;

		return number;

	}

	uint8_t set_bit(uint8_t number, int index) // Установить бит по индексу в единицу
	{
		return number | (1 << index);
	}

	uint8_t reset_bit(uint8_t number, int index) // Установить бит по индексу в ноль
	{
		return number & ~(1 << index);
	}

	uint8_t inverse_bit(uint8_t number, int index) // Инверсия бита по индексу
	{
		return number ^ (1 << index);
	}


