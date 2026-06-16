#include <cstdlib>
#include <iostream>
#include <string>
#include "model/Object3d.hpp"
#include "graphics/Window.hpp"

int main(int ac, char **av) {
	if (ac != 2) {
		std::cout << "Valid exec.: ./scop <filename>";
		std::exit(1);
	}
	const std::string inputFilename = av[1];
	Object3d obj = Object3d(inputFilename);
	try {
		Window window = Window(obj);
		window.run();
	} catch (const std::exception& e) {
	    std::cerr << "Fatal renderer error. Reason: " << e.what() << std::endl;
		std::exit(1);
	}
}
