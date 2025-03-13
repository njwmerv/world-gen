#include "perlin.h"
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

// Constructors
Perlin::Perlin(const int seed, const int width, const int height) : seed{seed} {
	srand(seed);

	// Create influence vectors
	for(int row = 0; row < height; row++){
		std::vector<float> newRow = std::vector<float>(width);
		for(int col = 0; col < width; col++){
			const float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 360);
			newRow[col] = angle;
		}
		influence.emplace_back(newRow);
	}
}

// Accessors
int Perlin::getSeed() const {return seed;}
vector<vector<float>> Perlin::getInfluences() const {return influence;}
int Perlin::getWidth() const {return influence.at(0).size() - 1;}
int Perlin::getHeight() const {return influence.size() - 1;}

// Actual Methods
float Perlin::influenceDotOffset(const float angle, const vector2& vector){
	return vector.x * cos(angle) + vector.y * sin(angle);
}

float Perlin::lerp(const float weight, const float a1, const float a2){
  	return weight * (a2 - a1) + a1;
}

float Perlin::smoothStep(const float x){
  	return ((6 * x - 15) * x + 10) * x * x * x;
}

float Perlin::perlin(const float x, const float y) const {
	// Get index of point
	int ix = static_cast<int>(x);
	int iy = static_cast<int>(y);
    const float xf = x - ix;
    const float yf = y - iy;
	ix %= getWidth();
	iy %= getHeight();
	const int ixR = (ix + 1) % getWidth();
	const int iyR = (iy + 1) % getHeight();

	// Get influence vector for nearest corners
	const float tl_angle = influence.at(iy).at(ix);
	const float tr_angle = influence.at(iy).at(ixR);
	const float bl_angle = influence.at(iyR).at(ix);
	const float br_angle = influence.at(iyR).at(ixR);

	// Get offset vectors to nearest corners
	const vector2 tl_offset{xf, yf};
    const vector2 tr_offset{xf - 1.0f, yf};
    const vector2 bl_offset{xf, yf - 1.0f};
    const vector2 br_offset{xf - 1.0f, yf - 1.0f};

    // Get dot product of each corner's corresponding vectors
	const float tl_dot = influenceDotOffset(tl_angle, tl_offset);
	const float tr_dot = influenceDotOffset(tr_angle, tr_offset);
	const float bl_dot = influenceDotOffset(bl_angle, bl_offset);
	const float br_dot = influenceDotOffset(br_angle, br_offset);

    // Blend horizontally
    const float w1 = smoothStep(xf);
    const float h1 = lerp(w1, tl_dot, tr_dot);
    const float h2 = lerp(w1, bl_dot, br_dot);

    // Blend vertically
    const float w2 = smoothStep(yf);
    const float result = lerp(w2, h1, h2);

    return result;
}