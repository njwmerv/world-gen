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

	static float influenceDotOffset(float angle, const vector2& vector);

	static float lerp(float weight, float a1, float a2);

	static float smoothStep(float x);

public:
	explicit Perlin(int seed = 0, int width = 64, int height = 64);

	int getSeed();
	std::vector<std::vector<float>> getInfluences();
	int getWidth();
	int getHeight();

	// Sample perlin noise at point (x, y)
	float perlin(float x, float y);
};

#endif // PERLIN_H
