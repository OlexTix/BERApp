#include "BerAppH.h"
using namespace std;

struct berResults //a struct of variables used to calculate BER results
{
	double bitNumber; //number of bits
	double bitErr; //variable for error bits
	float BER;// variable used to present results
	clock_t timeStart;// Calculation start time
	clock_t timeEnd;// Calculation end time

};

uint8_t HammingDistance(uint8_t n1, uint8_t n2);
void createFile(const string name, const int count, const char value);
berResults BERCalculate(string filepath1, string filepath2);
void printBERResult(berResults results);



int main(int argc, char* argv[])
{
	string filepath1;//path to File 1
	string filepath2; //path to File 2
	berResults results;

	openLogFile("log.txt"); //create the log file by the name of "log.txt"
	if (argc != 3) //check if there are exactly 3 arguments- name, count, value
	{
		saveLogFile("No filepath specified");
		saveLogFile("Creating the test .bin files");

		//Test 1- 100B files (the exact same files)

		createFile("test1_file1.bin", 100, 0x55);
		createFile("test1_file2.bin", 100, 0x55); 
		 
		//Test 2- 100B files
		createFile("test2_file1.bin", 100, 0x55);
		createFile("test2_file2.bin", 100, 0x5F); 

		//Test 3- 400MB files
		createFile("test3_file1.bin", 400000000, 0x55);
		createFile("test3_file2.bin", 400000000, 0x50);

		saveLogFile("Test files are being prepared");
		saveLogFile("Insert correct arguments and run the BERApp once again. Tip: ./BerApp.exe test1_file1.bin test1_file2.bin");
		

	}



	else
	{
		filepath1 = argv[1];
		filepath2 = argv[2];

		saveLogFile("Processing test files");
		results = BERCalculate(filepath1, filepath2);
		printBERResult(results);


	}


	closeLogFile();



	return 0;
}

uint8_t HammingDistance(uint8_t n1, uint8_t n2)
{
	uint8_t hammX = n1 ^ n2; // XOR operation
	uint8_t setBits = 0;
	while (hammX > 0)
	{
		setBits += hammX & 1;
		hammX >>= 1;

	}

	return setBits;

}


void createFile(const string name, const int count, const char value)


{


	fstream file;
	file.open(name.c_str(), ios::binary | ios::out);
	for (int i = 0; i < count; i++)
	{

		file.write((char*)&value, 1);

	}

	file.close();

}



 berResults BERCalculate(string filepath1, string filepath2)
{

	fstream file1, file2;

	berResults results;
	results.timeStart = 0;
	results.timeEnd = 0;
	results.BER = 0;
	results.bitErr = 0;
	results.bitNumber = 0;
	
	saveLogFile("Calculating BER...");
	file1.open(filepath1.c_str(), ios::binary | ios::in);
	file2.open(filepath2.c_str(), ios::binary | ios::in);
	char a = 0x00;
	char b = 0x00;
	results.timeStart = clock();

	while (!file1.eof())
	{
		file1 >> a;
		file2 >> b;
		if (!file1.eof())
		{
			results.bitErr += HammingDistance(a, b);//add another error bits to their total amount
				results.bitNumber += 8;//add compared bits to their total amount


		}

	}

	results.BER = (float)results.bitErr / results.bitNumber;//calculate bit error rate
	results.timeEnd = clock();
	saveLogFile("BerApp has finished its work");
	return results;


}


void printBERResult(berResults results)
{

	stringstream messageResult;
	messageResult << "Presenting the BERApp results: " << endl;
	messageResult << "BER (Bit Error Rate): " << results.BER << endl;
	messageResult << "Number of bits: " << results.bitNumber << endl;
	messageResult << "Error bits: " << results.bitErr << endl;
	messageResult<< "Total calculation time: " << ((float)results.timeEnd - results.timeStart) / CLOCKS_PER_SEC << " sec " << endl;
	saveLogFile(messageResult.str());


}
