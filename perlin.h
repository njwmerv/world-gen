#ifndef PERLIN_H
#define PERLIN_H
#include <vector>

// 2D Perlin Noise Generator
class Perlin{
	int seed;
	std::vector<std::vector<float>> influence;

	typedef struct{
		float x, y;
	} vector2;

	float influenceDotOffset(const float angle, const vector2& vector);

	float lerp(const float weight, const float a1, const float a2);

	float smoothStep(const float x);

public:
	Perlin(const int seed = 0, const int width = 64, const int height = 64);

	int getSeed();
	std::vector<std::vector<float>> getInfluences();
	int getWidth();
	int getHeight();

	// Sample perlin noise at point (x, y)
	float perlin(const float x, const float y);
};

#endif // PERLIN_H
