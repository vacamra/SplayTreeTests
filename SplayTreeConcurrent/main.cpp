
#include <iostream>

int main_parallel_orig_performance();
int main_parallel_stupid();
int main_serial_orig_perfomance();
int main_serial_biasedData();
int main_singleThread();
int main_biasedData();
int main_biasedDataShuffle();
int main_parallel_performance();
int main_clear();

int main()
{
start:
	std::cout << "What mode should I run in?" << std::endl << "Enter: " << std::endl <<
		"s for original serial (performance)," << std::endl <<
		"t for single thread test," << std::endl <<
		"n for naive parallel (one mutex)," << std::endl <<
		"p for parallel," << std::endl <<
		"f for parallel (performance - no checks), " << std::endl <<
		"b for biased (seq insert - nearly impossible)," << std::endl <<
		"v for biased (shuffled insert)," << std::endl <<
		"c for serial (original) biased," << std::endl <<
		"r for clear() and empty() test," << std::endl <<
		"e/q for exit/quit: ";
	char mode;
	std::cin >> mode;
	if (mode == 'p') {
		main_parallel_orig_performance();
	} else if  (mode == 's') {
		main_serial_orig_perfomance();
	} else if (mode == 't') {
		main_singleThread();
	} else if (mode == 'n') {
		main_parallel_stupid();
	} else if (mode == 'b')	{
		main_biasedData();
	} else if (mode == 'v') {
		main_biasedDataShuffle();
	} else if (mode == 'c') {
		main_serial_biasedData();
	} else if (mode == 'f') {
		main_parallel_performance();
	} else if (mode == 'r') {
		main_clear();
	} else if (mode == 'e' || mode == 'q')
		return 0;
	else {
		std::cout << "Wrong mode! Try again!" << std::endl;		
	}
	goto start;
}