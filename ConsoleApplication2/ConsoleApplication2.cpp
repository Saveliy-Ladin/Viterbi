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


uint8_t getBit(uint8_t number, int index) {

	number = number & (1 << index);
	number = number >> index;

	return number;
}

class Viterbi {
public:

	uint8_t number;

	int register_size = 0;

	vector<uint8_t> polynoms;

	Viterbi(uint8_t number, const vector<uint8_t>& polynoms){

		this->number = number;
		this->polynoms = polynoms;

	}

	int get_High_Bit_Position(uint8_t num);

	uint8_t getBit(uint8_t num, int index);

	uint8_t setBit(uint8_t &num, int index);

	uint8_t resetBit(uint8_t& num, int index);

	uint8_t inverseBit(uint8_t& num, int index);

	uint8_t inverseNumber(uint8_t num, int size);

	int8_t sumBit(uint8_t number, const vector<uint8_t>& polynoms);

	};

	class Coder_Viterbi : public Viterbi {
	public:

		Coder_Viterbi(uint8_t number, const vector<uint8_t>& polynoms):Viterbi(number,polynoms){
			
			this->register_size = get_Registr_Size(polynoms);

		}

		uint16_t coding(uint8_t number, vector<uint8_t>& polynoms);

		int get_Registr_Size(const vector<uint8_t>& polynoms);

	};

	class Decoder_Viterbi : public Viterbi {
	public:
		class Grid { // Вычислительная решетка
			
		public:
			class ZZ;

			int size;
			ZZ *array = new ZZ[size];
			

			void print() {
				for (int i = 0; i < size; i++)
				{
					//cout << i << "\t" << "val0 = " << static_cast<int>(array[i].val0) << "| val1 = " << static_cast<int>(array[i].val1) << endl;
				cout << bitset<sizeof(uint8_t) * 8>(i) << "\t" << "val0 = " << bitset<sizeof(uint8_t) * 8>(array[i].val0) << "| val1 = " << bitset<sizeof(uint8_t) * 8>(array[i].val1) << endl;

					//cout << i << endl;
				}

			}

			void init(int size) {
				this->size = size;
				//array.reserve(size);
			}
			
			Grid() {
				this->size = 0;
			}

			class ZZ { // Символы, генерируемые кодером
			public:
				uint8_t val0;
				uint8_t val1;

				void init(uint8_t val0, uint8_t val1) {
					this->val0 = val0;
					this->val1 = val1;
				}

				ZZ() {
					this->val0 = 0;
					this->val1 = 0;

				}


			};


		};

		Grid grid;

		void print() {

			grid.print();

		}

		Decoder_Viterbi(uint8_t number, const vector<uint8_t>& polynoms, int register_size) : Viterbi(number, polynoms){
			this->register_size = register_size;
			
			int a = pow(2, register_size-1);

			this->grid.init(a);

			uint8_t val0 = 0, val1 = 0;

			for (int i = 0; i < a; i++)
			{
				
				uint8_t temp = i;
				
					val0 = sumBit(temp, polynoms);
					val1 = sumBit(setBit(temp, register_size-1),polynoms);

				grid.array[i].init(val0, val1);

			}

		
			//this->grid.print();

		}

	};

	int main() {
		setlocale(LC_ALL, "ru");

		vector<uint8_t> pol = { 15,13 };
		uint8_t ex = 0b11111000; //               ЗАМЕТКА: написать исключение, когда пытаются ввести число больше размера регистра

		//Coder_Viterbi v(ex,pol);

		Decoder_Viterbi dv(ex,pol,4);
		dv.print();

		
		//cout << bitset<sizeof(uint8_t) * 8>(v.coding(ex, pol));


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
				n = n & polynoms[i];
				
				result = result << 1;

				for (int j = 0; j <register_size; j++)
				{
					temp = getBit(n, j);
					result ^= temp;

				}

			}

			return result;
		
	}

	int Coder_Viterbi::get_Registr_Size(const vector<uint8_t>& polynoms) // получить размер сдвивого регистра
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
			int divider = pow(2, register_size);
			uint8_t registr = number % divider;

			while (i < register_size) {

				result = result << polynoms.size();

				result |= sumBit(registr % divider, polynoms);

				registr = registr << 1;

				i++;
			}

			return inverseNumber(result, i*2);

	}


