#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;


char* enter() {
	char *ent = new char[30];

	cout << "Введите генерирующие полиномы через запятую:" << endl;
	cin.getline(ent, 30);
	
	return ent;


}


uint8_t getBit(uint8_t num, int index) {

	num = num & (1 << index);
	num = num >> index;

	return num;
}

class Viterbi {
public:

	uint8_t number;
	int registr_size;

	Viterbi(uint8_t number){

		this->number = number;

	}

	int get_High_Bit_Position(uint8_t num);

	uint8_t getBit(uint8_t num, int index);

	uint8_t setBit(uint8_t &num, int index);

	uint8_t resetBit(uint8_t& num, int index);

	uint8_t inverseBit(uint8_t& num, int index);

	uint8_t inverseNumber(uint8_t num, int size);

	int8_t sumBit(uint8_t number, const vector<uint8_t>& polynoms);

	int get_Registr_Size(const vector<uint8_t>& polynoms);

	
	};

	class Coder_Viterbi : public Viterbi {
	public:

		vector<uint8_t> polynoms;
			
		Coder_Viterbi(uint8_t number, const vector<uint8_t>& polynoms):Viterbi(number){
			
			this->polynoms = polynoms;
			this->registr_size = get_Registr_Size(polynoms);
		}


		uint16_t coding(uint8_t number, vector<uint8_t>& polynoms);


	};

	int main() {
		setlocale(LC_ALL, "ru");

		vector<uint8_t> pol = { 15,13 };
		uint8_t ex = 0b1010; //               ЗАМЕТКА: написать исключение, когда пытаются ввести число больше размера регистра

		Coder_Viterbi v(ex,pol);


		cout << bitset<sizeof(uint8_t) * 8>(v.coding(ex, pol));


		return 0;
	}


	int Viterbi::get_High_Bit_Position(uint8_t num) //получить значения старшего бита
	{
			int result = 0;

			for (int i = 7; i != 0; i--)
			{
				if (static_cast<int>(getBit(num, i)) == 1) {

					result = i + 1;
					break;
				}
			}

			return result;
	
	}

	uint8_t Viterbi::getBit(uint8_t num, int index) // Получить значения бита по индексу
	{
			num = num & (1 << index);
			num = num >> index;

			return num;

	}

	uint8_t Viterbi::setBit(uint8_t &num, int index)
	{
		return num |(1 << index);
	}

	uint8_t Viterbi::resetBit(uint8_t& num, int index)
	{
		return num &~(1 << index);
	}

	uint8_t Viterbi::inverseBit(uint8_t& num, int index)
	{
		return num^(1<<index);
	}

	uint8_t Viterbi::inverseNumber(uint8_t num, int size)
	{

		uint8_t temp = num;

		for (int i = 0, j = size; i != size / 2; ++i, --j)
		{
			if (getBit(num, i) == getBit(num, j - 1)) {

				continue;
			}
			else {

				temp = inverseBit(temp, i);

				temp = inverseBit(temp, j - 1);

			}
		}

		return temp;
	}

	int8_t Viterbi::sumBit(uint8_t number, const vector<uint8_t>& polynoms)  // Сложение битов регистра
	{
			uint8_t result = 0;
			uint8_t temp = 0;

			uint8_t n = 0; //rename!!!!!!!!

			for (int i = 0; i < polynoms.size(); i++)
			{
				n = number;

				//cout << "sumBit number before:" << bitset<sizeof(uint8_t) * 8>(n) << endl << endl;

				n = n & polynoms[i];
				// cout << "sumBit number after:" << bitset<sizeof(uint8_t) * 8>(n) << endl << endl;
				result = result << 1;

				for (int j = 0; j < get_High_Bit_Position(n); j++)
				{
					temp = getBit(n, j);
					result ^= temp;

				}

			}

			//cout << "sumBit RESULT:" << bitset<sizeof(uint8_t) * 8>(result) << endl << endl;



			return result;
		
	}

	int Viterbi::get_Registr_Size(const vector<uint8_t>& polynoms) // получить размер сдвивого регистра
	{

			int result = 0;

			auto max = max_element(polynoms.begin(), polynoms.end());

			result = get_High_Bit_Position(*max);

			return result;
		
	}

	uint16_t Coder_Viterbi::coding(uint8_t number, vector<uint8_t>& polynoms) // Процесс кодирования
	{		
		number = inverseNumber(number, get_Registr_Size(polynoms));
		cout<<endl<< bitset<sizeof(uint8_t) * 8>(number)<<endl;
			
			uint16_t result = 0;
			int i = 0; 
			int divider = pow(2, registr_size);
			uint8_t registr = number % divider;

			while (i < registr_size) {

				//cout << "Registr:" << bitset<sizeof(uint8_t) * 8>(registr) << endl << endl;
				//cout << "Result before:" << bitset<sizeof(uint8_t) * 8>(result) << endl << endl;
				result = result << polynoms.size();

				result |= sumBit(registr % divider, polynoms);

				//cout << "Result after:" << bitset<sizeof(uint8_t) * 8>(result) << endl << endl;

				registr = registr << 1;

				i++;
			}

			return inverseNumber(result, i*2);

			

		
	}
