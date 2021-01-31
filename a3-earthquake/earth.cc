/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string> &search_path) {
    // init shader program
    shader_.Init();
    
    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // init geometry
    const int nslices = 10;
    const int nstacks = 10;

    // TODO: This is where you need to set the vertices and indiceds for earth_mesh_.

    // As a demo, we'll add a square with 2 triangles.
    std::vector<unsigned int> indices;
    std::vector<Point3> vertices;

	//Push in ther vertices
	//We want to go from 0 to 10 so thats why we do nstacks + 1 and nslices + 1
	int count = 0;
	for (int i = 0; i < nstacks+1; i++) {
		for (int j = 0; j < nslices+1; j++) {
			 
			//All this math is a simple way of going from -pi to pi
			//and -pi/2 to pi/2
			float x, y;
			x = 1.0f * j / nslices; // turns x to a number between 0 & 1
			x -= 0.5f; // x is between -0.5 and 0.5
			x *= 2.0f; // x is between -1 and 1 
			x *= M_PI;

			y = 1.0f * i / nstacks; //y = between 0 and 1
			y -= 0.5f; // y between -0.5 and 0.5
			y *= 2.0f; // y between -1 and 1
			y *= M_PI / 2; // -pi/2 and pi/2 
			vertices.push_back(Point3(x, y, 0));
			count++;
		}
	}

	//There could have been a million better ways to do this
	//I basically push the first row of indices separetly
	//I start by pushing the first 6 indices for the first box -- why? Not sure, it was for clarity on my end.
	indices.push_back(0);
	indices.push_back(nslices + 2);
	indices.push_back(nslices + 1);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(nslices + 2);


	//Once done, I do the rest of the nslices in the first row.
	for (int i = 1; i < nslices; i++) {
		indices.push_back(i);
		indices.push_back(i + nslices + 2);
		indices.push_back(i + nslices + 1);

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + nslices + 2);

	}

	//I do the first one differently because I start at 0, but in hindsight, I think I could have done it in one loop.
	//Essentially all I'm doing in this loop is making indices like this:
	//0  1
	//11 12
	//I push 0, 11, and 12
	//Then I push 0, 12, and 1. 
	//I'm basically adding 11 and 12 to the top left number to get the bottom numbers
	//But its a little more complicated with my code.
	for (int i = 1; i < nstacks; i++) {
		for (int j = 0; j < nslices; j++) {
			indices.push_back(j + (i * nslices) + i);
			indices.push_back(j + (i * nslices) + nslices + i + 2);
			indices.push_back(j + (i * nslices) + nslices + i + 1);

			indices.push_back(j + (i * nslices) + i);
			indices.push_back(j + (i * nslices) + i + 1);
			indices.push_back(j + (i * nslices) + nslices + i + 2);
		}
	}
    
	std::vector<Point2> texcoords;
	//Texture coordinates. I matched the point2's to the point3 vertices.
	//Convert to float as necessary
	for (int i = 0; i < nstacks + 1; i++) {
		for (int j = 0; j < nslices + 1; j++) {
			float y = 1 - (1.0f * i / nstacks);
			float x = 1.0f * j / nslices;
			texcoords.push_back(Point2(x, y));
		}
	}




    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.UpdateGPUMemory();
	earth_mesh_.SetTexCoords(0, texcoords);
}



void Earth::Draw(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10,10,10);
    light.ambient_intensity = Color(1,1,1);
    light.diffuse_intensity = Color(1,1,1);
    light.specular_intensity = Color(1,1,1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> sphere calculations in one place.
	double lat = GfxMath::ToRadians(latitude);
	double lonj = GfxMath::ToRadians(longitude);
	float x = 1.0f * cos(lat) * sin(lonj);
	float y = 1.0f * sin(lat);
	float z = 1.0f * cos(lat) * cos(lonj);
    return Point3(x, y, z);
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> plane calculations in one place.
	float x = (1.0f * longitude / 180) * M_PI;
	float y = (1.0f * latitude / 90) * M_PI / 2;
    return Point3(x, y, 0);
}



void Earth::DrawDebugInfo(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
	//std::cout << earth_mesh_.num_triangles() << std::endl;
    for (int t=0; t<earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.triangle_vertices(t);
		//std::cout << t << " " << indices.size() << std::endl;
		std::vector<Point3> loop;
        loop.push_back(earth_mesh_.vertex(indices[0]));
        loop.push_back(earth_mesh_.vertex(indices[1]));
        loop.push_back(earth_mesh_.vertex(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1,1,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005);
    }
}

