
#include <iostream>

int main_parallel();
int main_serial();


int main()
{
start:
	std::cout << "What mode should I run in?" << std::endl << "Enter s for serial, p for parallel, e/q for exit/quit: ";
	char mode;
	std::cin >> mode;
	if (mode == 'p') {
		main_parallel();
	} else if  (mode == 's')
	{
		main_serial();
	} else if (mode == 'e' || mode == 'q')
		return 0;
	else {
		std::cout << "Wrong mode! Try again!" << std::endl;
		goto start;
	}
}