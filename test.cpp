#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>


int main()
{
	std::ifstream ifile("test.txt");
	std::ofstream ofile("test2.txt");
	char c = 0;
	std::string buf;
	std::stringstream toPrint;
	while(c = ifile.get())
	{
		if(ifile.eof())
			break;
		buf.push_back(c);
	}
	std::cout << buf;
	for(int i = 0 ; i < 1000000; i++)
	{
		toPrint << buf;
		toPrint << rand()%10000;
		toPrint << "\n\n";
	}
	ofile << toPrint.rdbuf();
}