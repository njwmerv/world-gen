#include "chunk.h"
#include "world.h"

int main(){
	constexpr int WINDOW_WIDTH = Chunk::SIZE * 4;
	constexpr int WINDOW_HEIGHT = Chunk::SIZE * 4;
	const int seed = std::chrono::steady_clock::now().time_since_epoch().count();
	World world{seed, WINDOW_WIDTH, WINDOW_HEIGHT};

	// Main loop
	while(world.update()){}

	return 0;
}