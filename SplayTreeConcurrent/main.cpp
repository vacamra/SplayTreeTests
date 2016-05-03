
#include <iostream>

int main_parallel_orig_performance();
int main_parallel_stupid();
int main_serial_orig_perfomance();
int main_singleThread();

int main()
{
start:
	std::cout << "What mode should I run in?" << std::endl << "Enter " << std::endl <<
		"s for original serial (performance)," << std::endl <<
		"t for single thread test," << std::endl <<
		"n for naive parallel (one mutex)," << std::endl <<
		"p for parallel," << std::endl << 
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
	} else if (mode == 'e' || mode == 'q')
		return 0;
	else {
		std::cout << "Wrong mode! Try again!" << std::endl;		
	}
	goto start;
}