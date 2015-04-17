//Represents a terrain, by storing a set of heights and normals at 2D locations

#pragma once

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <Windows.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <SDL.h>

#include "Vec3.h"

#define COLOR 1

#define BASE_HEIGHT 0
#define RAND_SCALE 100
#define SCALE_DIV 2.5
#define SMOOTH_PASS 1
#define ERODE_PASS 10
#define EROSION .01

#define LENGTH 513

#define VERTEX_ERROR_THRESHOLD .5
#define SUBDIVISION_ERROR_THRESHOLD .05

//Represents a vertex on the terrain.
struct Vertex
{
	Vertex()
	{
		height = 0;
		normal = Vec3();
		isActive = false;
	}

	float height;
	Vec3 normal;
	bool isActive;
};

class Terrain
{
public:
	Terrain();
	Terrain(int l2);
	int getLength();
	void setHeight(int x, int z, float y);
	float getHeight(int x, int z);
	Vec3 getNormal(int x, int z);
	
	//Generates terrain using the diamond-square algorithm.
	void generateTerrain(float baseHeight, float randScale, float scaleDiv);
	//Flattens the entire terrain to a height of 0.
	void flatTerrain();
	//Updates the terrain recursively by checking for vertex error and subdividing.
	void Update(int x, int z, int sideLen);
	//Renders the terrain recursively, top-down.
	void Render(int x, int z, int sideLen);

	Terrain* north;		//The terrain north of this one
	Terrain* east;
	Terrain* south;
	Terrain* west;

	int numTrgls;						//Triangle count - for statistics
	int numVerts;						//Vertex count - for statistics
	bool computedStats;					//Set when stats are done computing

private:
	int lastVerts;						//Used in calculation of verts
    int l;								//Length
    Vertex vertices[LENGTH][LENGTH];	//Each vertex on the grid
    bool computedNormals;				//Whether normals is up-to-date
	
	//Computes the normal vectors of each vertex by averaging the 4 adjacent face normals.
	void computeNormals();
	//Smooths the terrain by averaging each vertex with its neighbors.
	void smooth();
	//Erodes the terrain by chipping away small amounts at lower elevations.
	void erode();

	//Sets the vertex at [z][x] to active.
	void setActive(int x, int z);
	//Checks if the vertex at [z][x] is active.
	bool isActive(int x, int z);

	//Does a check to see whether the vertex at [x][z] should be included, according to interpolation error.
	void includeVertex(int x, int z, int sideLen, bool xAxis);
	//Does a check to see whether the subsquare with top-left corner [z][x] should be subdivided.
	bool subdivide(int x, int z, int sideLen);

	//Runs glNormal3f(), glColor3f() and glVertex3f() on the vertex at [z][x].
	void terrVertex(int x, int z);
};