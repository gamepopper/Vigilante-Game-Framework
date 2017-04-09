#pragma once
//Generates Perlin Noise Map
//Reference: https://github.com/WesOfX/perlin-noise/

#include <vector>

/*
* To use the Generator class, construct an object using either the
* default constructor or the seed constructor. The default
* constructor will initialize the permutation vector using Perlin's
* reference values from his reference Java implementation. Using
* the seed constructor the permutation vector is generated using a
* standard library random engine seeded with the provided seed. To
* get noise values, use the "get" function. To reseed the noise, use the
* "reseed" function. Examples below.
*
* // Create reference Perlin generator
* PerlinGenerator generator();
*
* // Print noise at 55, 99, 0
* std::cout << generator.get(55, 99, 0) << std::endl;
*
* // Reseed noise with new seed
* generator.reseed(42);
*
* // Print new noise at 55, 99, 0
* std::cout << generator.get(55, 99, 0) << std::endl;
*/

// A reference Perlin noise generator
class PerlinGenerator
{
	// Permutation vector for pseudo-random numbers
	std::vector<unsigned char> permutation;

public:
	// Creates Perlin noise generator with reference permutation vector
	PerlinGenerator();

	// Createz Perlin noise generator with a custom seed
	PerlinGenerator(unsigned int seed);

	// Returns noise value between -1.0 and 1.0
	float get(float x, float y, float z) const;

	void init();
	// Repopulates permutation vector using new seed
	void reseed(unsigned int seed);

private:
	// Functions for interpolating noise
	static float lerp(float t, float a, float b);
	static float fade(float t);
	static float grad(int hash, float x, float y, float z);
};

/*
* To use the Noise class, construct an object using either the
* default constructor or the generation constructor. Use the
* "generate" function to resize and fill the noise array. Use the
* "get" function to access the noise in the array using a 2D
* location. Optionally, you can use the generation constructor to
* immediately call the generate function during construction.
* Examples below.
*
* // Generate noise with generation constructor
* PerlinNoise::GenerateFloat(
* 	800,  // width
* 	600,  // height
* 	6.5,  // frequency
* 	3,    // octaves
* 	0.75, // persistence
* 	42    // seed
* );
*/

// A set of static functions for generating perlin noise maps.
class PerlinNoise
{
public:
	// Resizes the noise map and populates it with noise
	static std::vector<float> GenerateFloat(
		unsigned short width,  // Number of columns
		float frequency,       // Frequency of noise features
		unsigned int octaves,  // Number of octaves (halving frequency)
		float persistence,     // Compounding weight of octaves
		unsigned int seed = 0  // Seed for perlin noise
		);

	// Resizes the noise map and populates it with noise
	static std::vector<float> GenerateFloat(
		unsigned short width,  // Number of columns
		unsigned short height, // Number of rows
		float frequency,       // Frequency of noise features
		unsigned int octaves,  // Number of octaves (halving frequency)
		float persistence,     // Compounding weight of octaves
		unsigned int seed = 0  // Seed for perlin noise
		);

	// Resizes the noise map and populates it with noise
	static std::vector<std::vector<float>> GenerateFloat(
		unsigned short width,  // Number of columns
		unsigned short height, // Number of rows
		unsigned short depth,  // Number of layers
		float frequency,       // Frequency of noise features
		unsigned int octaves,  // Number of octaves (halving frequency)
		float persistence,     // Compounding weight of octaves
		unsigned int seed = 0  // Seed for perlin noise
		);
};