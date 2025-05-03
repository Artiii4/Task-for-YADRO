#include "Sorter.h"
#include "Tape.h"
#include "Delays.h"

#include <iostream>

int main() {
  int memoryLimit;
  try {
    Delays delays{ 1, 1, 1 };

    Tape input("input.txt", delays, false); 

    input.rewind();
    std::cout << "Data in file input.txt: ";
    while (input.getPosition() < input.getLength()) {
      std::cout << input.read() << " ";
      input.moveRight();
    }

    const int memoryLimit = 1024;
    int inputLength = input.getLength();
    int numBuckets = std::min(memoryLimit / 4, inputLength);

    Tape output("output.txt", delays, true);

    Sorter sorter(numBuckets, "tmp");
    sorter.sort(input, output);

    output.rewind();
    std::cout << "\nData in file output.txt: ";
    while (output.getPosition() < output.getLength()) {
      std::cout << output.read() << " ";
      output.moveRight();
    }
    std::cout << std::endl;
  }

  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
