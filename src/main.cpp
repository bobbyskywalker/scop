#include <cstdlib>
#include <iostream>
#include <string>
#include "../inc/model/Object3d.hpp"
#include "../inc/graphics/Window.hpp"

int main(int ac, char **av) {
	if (ac != 2) {
		std::cout << "Valid exec.: ./scop <filename>";
		std::exit(1);
	}
	std::string inputFilename = av[1];
	Object3d obj = Object3d(inputFilename);
	Window window = Window();
	window.run();
}
