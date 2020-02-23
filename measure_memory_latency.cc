#include "measure_memory_latency.hh"

/*
	Created by Maxx Curtis
	for CSCI 389 Homework 1

	Creates "buffers" ranging from 2^10-2^28 bytes, implemented as vectors of
	characters. Buffers are filled with randomly chosen characters from 'a' to 'z'
	in an attempt to remove any patterns that could be picked up by the compiler.

	Individual bytes are read out of the buffer some number of times equal to the
	buffer's size, and the time taken is averaged to show how long it takes to
	read a single byte from the memory location where the buffer is stored.

	As buffer sizes increase, read times should increase as the CPU is forced to
	read from slower and slower memory locations.
*/

// Constants
const int BUFFER_POWER_LOWER = 10;
const int BUFFER_POWER_UPPER = 28;
const int TEST_SIZE = 200000;

void init_buffer(int bufferSize, std::default_random_engine gen, std::vector<char>& buffer) {
	/* Fills a char vector of size bufferSize with random letters from 'a' to 'z' */
	std::uniform_int_distribution<> distr(97, 122);
	for (int i = 0; i < bufferSize; i++) {
		char entry = distr(gen);
		buffer[i] = static_cast<char>(distr(gen));
	}
	gen();
}

int main() {
	/* Records latency of reading ten bytes from buffers of various sizes */
	std::cout << "\nTesting read speed for buffers from size 2^10-2^16...\n";
	std::cout << "\nBytes Read:   Sum of Bytes:   Time Taken Per Byte: \n";
	std::default_random_engine gen;
	// For each possible size:
	for (int i = BUFFER_POWER_LOWER; i <= BUFFER_POWER_UPPER; i++) {

		int bufferSize = pow(2.0, i);
		std::vector<char> buffer(bufferSize, '0');
		init_buffer(bufferSize, gen, buffer);
		std::uniform_int_distribution<> distr(0, bufferSize - 1);

		int sumOfPulledBytes = 0;
		volatile char randomByte = 'a';
		int randomByteIndex = 0;
		double duration = 0;

		for (int j = 0; j < TEST_SIZE; j++) {
			// Read random bytes from the buffer enough times to make sure the
			// buffer is stored in the 'highest' possible cache
			randomByteIndex = distr(gen);
			randomByte = buffer[randomByteIndex];
			sumOfPulledBytes += static_cast<int>(randomByte);
		}

		// Create vector of random indexes that we can use to choose bytes quickly
		gen();
		std::vector<int> randomIndexes;
		for (int k = 0; k < bufferSize; k++) {
			randomByteIndex = distr(gen);
			randomIndexes.push_back(randomByteIndex);
		}

		// The clock was misbehaving (producing really high values) when it was
		// called inside the for loop, so instead of measuring individual read
		// times, I've situated it outside the for loop and averaged the results.

		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		for (int l = 0; l < bufferSize; l++) {
			// Measure the time it takes to read bufferSize bytes from the buffer
			sumOfPulledBytes += static_cast<int>(buffer[randomIndexes[l]]);
		}
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

		// Average the total time
		duration /= TEST_SIZE;
		buffer.clear();
		std::cout << "2^" << i << "          " << sumOfPulledBytes << "         " << duration << "\n";
	}
	std::cout << "\n";
	return 0;
}
