/** CSci-4611 Assignment 3:  Earthquake
 */

#include "quake_app.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

std::vector<float> normalizeList(std::vector<float> quakeList) {
	float maxVec = *std::max_element(quakeList.begin(), quakeList.end());
	for (int i = 0; i < quakeList.size(); i++) {
		quakeList[i] /= maxVec;
	}
	return quakeList;
}

void printList() {
	std::vector<unsigned int> indices;
	int stacks = 2;
	int slices = 4;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	std::cout << indices[0] << ", " << indices[1] << ", " << indices[2] << std::endl;
	std::cout << indices[3] << ", " << indices[4] << ", " << indices[5] << std::endl;

	for (int i = 1; i < slices; i++) {
		indices.push_back(i * 2 + 1);
		indices.push_back(i * 2);
		indices.push_back(i * 2 + 2);

		indices.push_back(i * 2 + 1);
		indices.push_back(i * 2 + 2);
		indices.push_back(i * 2 + 3);

		std::cout << indices[i * slices] << ", " << indices[i * slices + 1] << ", " << indices[i * slices + 2] << std::endl;
		std::cout << indices[i * slices + 3] << ", " << indices[i * slices + 4] << ", " << indices[i * slices + 5] << std::endl;
	}
	if (stacks > 1) {
		int ind_size = indices.size();
		indices.push_back(1);
		indices.push_back(indices[ind_size - 1] + 1);
		indices.push_back(indices[ind_size + 1] + 1);

		indices.push_back(1);
		indices.push_back(indices[ind_size + 1] + 1);
		indices.push_back(2);

		std::cout << indices[ind_size] << ", " << indices[ind_size + 1] << ", " << indices[ind_size + 2] << std::endl;
		std::cout << indices[ind_size + 3] << ", " << indices[ind_size + 4] << ", " << indices[ind_size + 5] << std::endl;
		
		for (int i = 1; i < slices; i++) {
			ind_size = indices.size();
			indices.push_back(indices[ind_size - 1]);
			indices.push_back(indices[ind_size - 2]);
			indices.push_back(indices[ind_size - 2] + 1);

			indices.push_back(indices[ind_size]);
			indices.push_back(indices[ind_size + 2]);
			indices.push_back(indices[ind_size] + 2);

			std::cout << indices[ind_size] << ", " << indices[ind_size + 1] << ", " << indices[ind_size + 2] << std::endl;
			std::cout << indices[ind_size + 3] << ", " << indices[ind_size + 4] << ", " << indices[ind_size + 5] << std::endl;
		}
	}
	int vertices_per_row = 2 * 3 * slices; //36
	for (int i = 2; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			int ind_size = indices.size();
			if (j == 0) {
				indices.push_back(indices[ind_size - vertices_per_row + 1]);
				indices.push_back(indices[ind_size - 2] + 1);
				indices.push_back(indices[ind_size + 1] + 1);

				indices.push_back(indices[ind_size - vertices_per_row + 1]);
				indices.push_back(indices[ind_size + 1] + 1);
				indices.push_back(indices[ind_size] + 1);
			}
			else {
				indices.push_back(indices[ind_size - vertices_per_row + 1]);
				indices.push_back(indices[ind_size - 2]);
				indices.push_back(indices[ind_size + 1] + 1);

				indices.push_back(indices[ind_size - vertices_per_row + 1]);
				indices.push_back(indices[ind_size + 1] + 1);
				indices.push_back(indices[ind_size] + 1);
			}

			std::cout << indices[ind_size] << ", " << indices[ind_size + 1] << ", " << indices[ind_size + 2] << std::endl;
			std::cout << indices[ind_size + 3] << ", " << indices[ind_size + 4] << ", " << indices[ind_size + 5] << std::endl;
		}
	}



	
}

int main(int argc, const char *argv[]) {
	//printList();
    QuakeApp app;
    app.Run();
    
/*	std::vector<float> quakes = { 0.0, 2.3, 5.1, 1.1, 7.6, 1.7 };
	std::vector<float> normalizedQuakes = normalizeList(quakes);

	for (int i = 0; i < normalizedQuakes.size(); i++) {
		std::cout << normalizedQuakes[i] << " --- ";
	}
	std::cout << std::endl;*/
	return 0;
}
