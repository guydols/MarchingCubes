#include <vector>
#include <random>
#include <iostream>

#include "MarchingCubes.h"



int main() {



	int width = 256;
	int length = 256;
	int height = 256;

	float pMin = 0.0;
	float pMax = 1.0;

	float isosurface = 0.9999;

    MarchingCubes marchingCubes = MarchingCubes(isosurface, width, height, length);

	std::srand(1337);

	std::vector<std::vector<std::vector<float> > > map;
	std::vector<Vec3> vertices;

	map.resize(height);
	for (int z = 0; z < height; z = z + 1) {
		map[z].resize(length);
	}

	for (int z = 0; z < height; z = z + 1) {
		for (int y = 0; y < length; y = y + 1) {
			map[z][y].resize(width);
		}
	}

	for (int z = 0; z < height; z = z + 1) {
		for (int y = 0; y < length; y = y + 1) {
			for (int x = 0; x < width; x = x + 1) {
				map[z][y][x] = pMax + (std::rand() / (RAND_MAX / (pMin - pMax)));
			}
		}
	}

    vertices = marchingCubes.March(map);

    return 0;
}