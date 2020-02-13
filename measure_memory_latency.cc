#include "measure_memory_latency.hh"

/*
Created by Maxx Curtis
for CSCI 389 Homework 1


*/

// Constants
const int BUFFER_POWER_LOWER = 10;
const int BUFFER_POWER_UPPER = 26;

void init_buffer(int bufferSize, std::default_random_engine gen, char& buffer) {
	std::uniform_int_distribution<> distr(97, 122);
	for (int i; i < bufferSize; i++) {
		char entry = distr(gen);
		buffer[i] = entry;
	}
	gen();
}

int main() {
		// Records latency of reading ten bytes from buffers of various sizes
	std::cout << "Testing read speed for buffers from size 2^10-2^16...\n";

	std::default_random_engine gen;
	// for each possible size:
	for (int i = BUFFER_POWER_LOWER; i < BUFFER_POWER_UPPER; i++) {

		int bufferSize = pow(2, i);
		char buffer[bufferSize];
		init_buffer(bufferSize, gen, buffer);
		std::uniform_int_distribution<> distr(0, bufferSize);

		for (int j = 0; j < 1000; j++) {
			// Read random bytes from the buffer enough times to make sure the
			// buffer is stored in the 'highest' possible cache
			int randomByteIndex = distr(gen);
			char randomByte = buffer[randomByteIndex];
		}

		double meanTimePerByte = 0;
		for (int k = 0; k < 10; k++) {
			// Read 10 bytes from the buffer and sum times taken for each
			int randomByteIndex = distr(gen);

			// Measure the time it takes to read one byte from the buffer
			std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
			char randomByte = buffer[randomByteIndex];
			std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

			double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
			meanTimePerByte += duration;
		}

		// Average the times taken to read 10 individual bytes
		meanTimePerByte /= 10;

		std::cout << "Buffer size: 2^" << i << "\n";
		std::cout << "Time taken to read one byte: " << meanTimePerByte << "\n";
	}

		// for a certain number of times:
			// grab a number and measure the time


std::cout << "Hello!\n";



	return 0;
}
