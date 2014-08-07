
#include "Terrain.h"

Terrain::Terrain()
{
    l = LENGTH;
	numTrgls = 0;
	numVerts = 0;
	lastVerts = 0;

	north = east = south = west = NULL;

	computedStats = false;
    computedNormals = false;
}

Terrain::Terrain(int l2)
{
	l = l2;
	computedNormals = false;
}

int Terrain::getLength()
{
    return l;
}

//Sets the height at (x, z) to y
void Terrain::setHeight(int x, int z, float y)
{
    vertices[z][x].height = y;
    computedNormals = false;
}
        
//Returns the height at (x, z)
float Terrain::getHeight(int x, int z)
{
    return vertices[z][x].height;
}

//Returns the normal at (x, z)
Vec3 Terrain::getNormal(int x, int z)
{
    return vertices[z][x].normal;
}

void Terrain::setActive(int x, int z)
{
	vertices[z][x].isActive = true;
}

bool Terrain::isActive(int x, int z)
{
	return vertices[z][x].isActive;
}

void Terrain::computeNormals()
{
    if (computedNormals)
        return;
            
	for(int z = 0; z < l; z++)
	{
		for(int x = 0; x < l; x++)
		{
			Vec3 sum;
			Vec3 v1, v2, v3, v4;

			if(z < l - 1)
				v1 = Vec3(0, getHeight(x, z) - getHeight(x, z + 1), -1);
			if(x < l - 1)
				v2 = Vec3(-1, getHeight(x, z) - getHeight(x + 1, z), 0);
			if(z > 0)
				v3 = Vec3(0, getHeight(x, z) - getHeight(x, z - 1), 1);
			if(x > 0)
				v4 = Vec3(1, getHeight(x, z) - getHeight(x - 1, z), 0);

			if(x < l - 1 && z < l - 1)
				sum += v1.Cross(v2);
			if(x < l - 1 && z > 0)
				sum += v2.Cross(v3);
			if(x > 0 && z > 0)
				sum += v3.Cross(v4);
			if(x > 0 && z < l - 1)
				sum += v4.Cross(v1);

			vertices[z][x].normal = sum.Normalize();
		}
	}

	computedNormals = true;
}

//http://www.shamusyoung.com/twentysidedtale/?p=11874
void Terrain::erode()
{
	for(int i = 0; i < ERODE_PASS; i++)
	{
		//Erosion starting spots
		int startX = 0;
		int startZ = 0;

		//Erode every spot on the map
		while(startZ < l)
		{
			int x = startX;			//Create two temp variables that are the spots downhill being eroded
			int z = startZ;
			bool lowest = false;	//Sentinel to detect when there is no lower point
			int prevX = -1;			//Previous iteration's x and z - start at -1 because there is no previous iteration at first
			int prevZ = -1;

			//Keep eroding until there is no lower point
			while(!lowest)
			{
				//Get current height
				float height = getHeight(x, z);

				//Store the current height for comparison later
				float prevHeight = height;

				int thisX = x;
				int thisZ = z;

				//Get adjacent heights and compare
				//We find the next point before eroding because if we first erode, we would need to store the old height for comparison
				//DO NOT compare if this point is the previous point - INFINITE LOOPS!
				if(thisX - 1 >= 0 && thisX - 1 != prevX)
				{
					float nextHeight = getHeight(thisX - 1, thisZ);
					if(nextHeight < height)
					{
						height = nextHeight;
						x = x-1;
					}
				}

				if(thisX + 1 < l && thisX + 1 != prevX)
				{
					float nextHeight = getHeight(thisX + 1, thisZ);
					if(nextHeight < height)
					{
						height = nextHeight;
						x = thisX;	//Reset x in case it was changed above
						x = x+1;
					}
				}

				if(thisZ - 1 >= 0 && thisZ - 1 != prevZ)
				{
					float nextHeight = getHeight(thisX, thisZ - 1);
					if(nextHeight < height)
					{
						height = nextHeight;
						x = thisX;	//Need to reset x just in case it was changed above
						z = z-1;
					}
				}

				if(thisZ + 1 < l && thisZ + 1 != prevZ)
				{
					float nextHeight = getHeight(thisX, thisZ + 1);
					if(nextHeight < height)
					{
						height = nextHeight;
						z = thisZ;	//Reset Z in case it was changed above
						x = thisX;
						z = z+1;
					}
				}

				//Erode this point
				setHeight(thisX, thisZ, prevHeight - EROSION);

				//If we didn't find any lower point, or we've run out of bounds, break out
				if(height == prevHeight || x < 0 || z < 0 || x >= l || z >= l)
					lowest = true;

				//Store the x and z (what they were before they changed)
				prevX = thisX;
				prevZ = thisZ;

			}

			//Increment startX
			startX++;
			//If startX goes out of bounds, go to the next "row" (startZ++) and wrap startX around
			if(startX >= l)
			{
				startX = 0;
				startZ++;
			}
		}
	}
}

void Terrain::smooth()
{
	float k = .5;
	
	for(int i = 0; i < SMOOTH_PASS; i++)
	{
		/* Rows, left to right */
		for(int x = 1; x < l; x++)
			for (int z = 0; z < l; z++)
				setHeight(x, z, getHeight(x-1, z) * (1-k) + getHeight(x,z) * k);

		/* Rows, right to left*/
		for(int x = l-2; x >= 0; x--)
			for (int z = 0;z < l; z++)
				setHeight(x, z, getHeight(x+1, z) * (1-k) + getHeight(x,z) * k);

		/* Columns, bottom to top */
		for(int x = 0; x < l; x++)
			for (int z = 1; z < l; z++)
				setHeight(x, z, getHeight(x, z-1) * (1-k) + getHeight(x,z) * k);

		/* Columns, top to bottom */
		for(int x = 0; x < l; x++)
			for (int z = l-2; z >= 0; z--)
				setHeight(x, z, getHeight(x, z+1) * (1-k) + getHeight(x,z) * k);
	}
}

float random(float lower, float upper)
{
	float base = rand()/((float)RAND_MAX);
	float random = base * (upper - lower) + lower;
	return random;
}

//Generates terrain using the diamond-square algorithm
void Terrain::generateTerrain(float baseHeight, float randScale, float scaleDiv)
{
	//Seed the rand # generator
	srand(time(NULL));

	int fullLength = getLength();

	//Get some random height values for the first 4 corners
	setHeight(0, 0, baseHeight + random(-randScale, randScale));
	setHeight(0, fullLength - 1, baseHeight + random(-randScale, randScale));
	setHeight(fullLength - 1, 0, baseHeight + random(-randScale, randScale));
	setHeight(fullLength - 1, fullLength - 1, baseHeight + random(-randScale, randScale));

	randScale = 150;

	for(int sideLen = fullLength - 1; sideLen >= 2; sideLen /= 2, randScale /= scaleDiv)
	{
		float newHeight;		//New height of the current node
		float avg;				//Average of the corners
		float c1, c2, c3, c4;	//Heights of corners of diamond or square

		for(int x = 0; x < fullLength - 1; x += sideLen)
			for(int y = 0; y < fullLength - 1; y += sideLen)
			{
				//Do square calculation
				c1 = getHeight(x, y);
				c2 = getHeight(x + sideLen, y);
				c3 = getHeight(x, y + sideLen);
				c4 = getHeight(x + sideLen, y + sideLen);

				//Do average with a little randomness
				avg = (c1+c2+c3+c4)/4 + random(-randScale, randScale);

				//Set the middle to the average
				setHeight(x + sideLen/2, y + sideLen/2, avg);
			}

		int halfSide = sideLen/2;

		/*printf("SQUARES\n");
		printf("halfSide = %i\n", halfSide);
		printf("_________\n");
		for(int i = 0; i < fullLength; i++)
		{
			for(int j = 0; j < fullLength; j++)
			{
				if(abs(getHeight(j, i)) < 2000000)
					printf("%.2f \t", getHeight(j, i));
				else
					printf("### \t");
			}
			printf("\n\n");
		}
		printf("\n\n\n");*/

		  for(int x = 0; x < fullLength - 1; x += halfSide)
			for(int y = (x + halfSide) % sideLen; y < fullLength - 1; y += sideLen)
			{
				c1 = getHeight((x - halfSide + fullLength - 1) % (fullLength - 1), y); //left of center
				c2 = getHeight((x + halfSide) % (fullLength - 1), y); //right of center
				c3 = getHeight(x, (y + halfSide) % (fullLength - 1)); //below center
				c4 = getHeight(x, (y - halfSide + fullLength - 1) % (fullLength - 1)); //above center

				avg = (c1+c2+c3+c4)/4 + random(-randScale, randScale);
				//update value for center of diamond
				setHeight(x, y, avg);

				if(x == 0) 
					setHeight(fullLength - 1, y, avg);
				if(y == 0)
					setHeight(x, fullLength - 1, avg);
			}

		/*printf("DIAMONDS\n");
		printf("_________\n");
		for(int i = 0; i < fullLength; i++)
		{
			for(int j = 0; j < fullLength; j++)
			{
				if(abs(getHeight(j, i)) < 2000000)
					printf("%.2f \t", getHeight(j, i));
				else
					printf("### \t");
			}
			printf("\n\n");
		}
		printf("\n\n\n");*/
	}

	//erode();
	smooth();
	computeNormals();
}

void Terrain::flatTerrain()
{
	for(int x = 0; x < l; x++)
		for(int z = 0; z < l; z++)
			setHeight(x, z, 0);
	computeNormals();
}

float max3(float f1, float f2, float f3)
{
	if(f1 > f2)
		if(f2 > f3)
			return f2;
		else
			return f3;
	else
		if(f1 > f3)
			return f1;
		else
			return f3;
}

//Returns true if a subsquare with center cX, cZ and side length sideLen should be subdivided or not.
//(x, z) is the coordinate of the top left of the subsquare, sideLen is the current side length
bool Terrain::subdivide(int x, int z, int sideLen)
{
	//Subdivide if the block isn't very coplanar
	//or if any of its side vertices are already enabled

	//First, check if any of the sides are already enabled
	if(isActive(x + sideLen/2, z) ||				//Top
		isActive(x + sideLen/2, z + sideLen) ||		//Bottom
		isActive(x, z + sideLen/2) ||				//Left
		isActive(x + sideLen, z + sideLen/2))		//Right
		return true;	//If they are, we know we have to subdivide this


	float sum = 0;
	sum += getHeight(x, z);
	sum += getHeight(x + sideLen, z);
	sum += getHeight(x, z + sideLen);
	sum += getHeight(x + sideLen, z + sideLen);

	if(abs(sum/4 - getHeight(x + sideLen/2, z + sideLen/2)) > SUBDIVISION_ERROR_THRESHOLD)
		return true;
	else
		return false;
}

//Sets a vertex include flag to true if the interpolation error is too large
/*

(x, z) is the point we're considering including, sideLen is the length of the side, xAxis indicates whether the edge
 lies on the x axis or z axis. Note that if the edge lies on the x axis, z varies.
We obtain interpolation error by averaging the two corners adjacent to the side vertex and subtracting this from the
 actual height

*/
void Terrain::includeVertex(int x, int z, int sideLen, bool xAxis)
{
	if(x >= 250)
		int gerp = 0;

	float hC1, hC2;		//Corner vertices forming the line
	float estHeight;	//Height if the vertex was not included

	
	if(xAxis)	//If it's on the z axis, the corner values are x coords
	{
		hC1 = x - sideLen/2;
		hC2 = x + sideLen/2;
		estHeight = (getHeight(hC1, z) + getHeight(hC2, z))/2;
	}
	else		//If it's on the x axis, the corner values are z coords
	{
		hC1 = z - sideLen/2;
		hC2 = z + sideLen/2;
		estHeight = (getHeight(x, hC1) + getHeight(x, hC2))/2;
	}

	//Include vertex if its interpolation error would be too large
	if(abs(estHeight - getHeight(x, z)) > VERTEX_ERROR_THRESHOLD)
		vertices[z][x].isActive = true;
}

//This function recursively processes a subsquare. It should first be called with the parameters (0, 0, l - 1)
// (x, z) is the top left coordinate of the current subsquare, and sideLen is its side length
/*

We begin at the topmost subsquare. The corner and center vertices are enabled. Then, we check the interpolation error
 on each edge vertex to see if we need to include them. subdivide() is run on each of the 4 subsquares, and if any of
 them need to be subdivided, we run Update() on them.

*/
void Terrain::Update(int x, int z, int sideLen)
{
	int halfSide = sideLen/2;		//Half of a side
	bool subdiv = false;			//Flag set when a child box is subdivided
	int cX = x + halfSide;
	int cZ = z + halfSide;
	int brX = x + sideLen;
	int brZ = z + sideLen;
	//Vertex boxCenter = vertices[halfSide][halfSide];
	
	//Enable the four corners and the center
	setActive(x, z);
	setActive(brX, z);
	setActive(x, brZ);
	setActive(brX, brZ);
	setActive(cX, cZ);

	//Check for vertex error on each of the sides
	includeVertex(cX, z, sideLen, true);	//Top side
	includeVertex(cX, brZ, sideLen, true);	//Bottom side
	includeVertex(x, cZ, sideLen, false);	//Left side
	includeVertex(brX, cZ, sideLen, false);	//Right side
	
	//Check for subdivision on each of the 4 subsquares if this subsquare isn't already the smallest it can be (3x3)
	if(sideLen > 3)
	{
		//NOTE: These should be single equals!! We're getting whether or not they're subdivided as we compare them.
		if(subdiv = subdivide(x, z, halfSide))
			Update(x, z, halfSide);
		if(subdiv = subdivide(cX, z, halfSide))
			Update(cX, z, halfSide);
		if(subdiv = subdivide(x, cZ, halfSide))
			Update(x, cZ, halfSide);
		if(subdiv = subdivide(x + halfSide, z + halfSide, halfSide))
			Update(cX, cZ, halfSide);
	}

	//If a child box was subdivided, that means an edge vertex was changed and we must update an adjacent box.
	//Must make sure it doesn't go out of bounds and not infinite loop (don't Update() if the square has already been activated)
	if(isActive(cX, z) && z - sideLen >= 0 && !isActive(cX, cZ - sideLen))		//Top side
		Update(x, z - sideLen, sideLen);
	if(isActive(cX, brZ) && z + sideLen < l - 1 && !isActive(cX, cZ + sideLen))	//Bottom side
		Update(x, z + sideLen, sideLen);
	if(isActive(x, cZ) && x - sideLen >= 0 && !isActive(cX - sideLen, cZ))		//Left side
		Update(x - sideLen, z, sideLen);
	if(isActive(brX, cZ) && x + sideLen < l - 1 && !isActive(cX + sideLen, cZ))	//Right side
		Update(x + sideLen, z, sideLen);
}

//Set a color, normal and run glVertex3f(x, getHeight(x, z), z)
void Terrain::terrVertex(int x, int z)
{
	Vec3 normal = getNormal(x, z);

	if(COLOR)
		glColor3f(normal.x * normal.x, normal.y * normal.y, normal.z * normal.z);
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(x, getHeight(x, z), z);
	numVerts++;
}

//This function recursively renders the terrain. It should first be called with the parameters (0, 0, l - 1)
/*

We first run render on the entire terrain.
 The first vertex to be rendered is the east side vertex (arbitrarily).
 For each of the 4 side vertices:
	If it's enabled, check if the center of the corresponding subsquare is enabled (see below)
		If the subsquare is enabled, then Render() it
		Mark that the corresponding corner vertex is already rendered (since the subsquare is rendered)
 At the end, render all corner vertices that weren't taken care of by subsquares following these rules:
	Start at the east vertex (arbitrarily)
	Render the side vertex

 *This is what I mean by subsquare corresponding to an edge vertex:
 o-----o
 |\   /|
 | \ / |
 |  X--o <--- edge vertex
 | /|\/|
 |/ |/\| <--- subsquare corresponding (i.e. the adjacent subsquare in the clockwise direction)
 o-----o

*/
void Terrain::Render(int x, int z, int sideLen)
{
	int halfSide = sideLen/2;		//Half side length
	int cX = x + halfSide;			//Center x coord
	int cZ = z + halfSide;			//Center z coord
	int brX = x + sideLen;			//Bottom-right X coord
	int brZ = z + sideLen;			//Bottom-right Z coord
	bool vBR = true, vBL = true, vTL = true, vTR = true;		//Flags to determine whether or not to render each corner vertex

	//Check if the center is enabled before attempting to render
	if(isActive(cX, cZ))
	{
		//If the east side is active...
		if(isActive(brX, cZ))
		{
			//It's a side vertex, so check if the center of the corresponding subsquare is enabled
			if(halfSide > 1 && isActive(cX + halfSide/2, cZ + halfSide/2))
			{
				//If the center's enabled, this quad is subdivided, so render the interior
				Render(cX, cZ, halfSide);
				//We no longer want to render the bottom right corner - it's been rendered by the subsquare
				vBR = false;
			}
		}

		//If the south side is active...
		if(isActive(cX, brZ))
		{
			//It's a side vertex, so check if the center of the corresponding subsquare is enabled
			if(halfSide > 1 && isActive(cX - halfSide/2, cZ + halfSide/2))
			{
				//If the center's enabled, this quad is subdivided, so render the interior
				Render(x, cZ, halfSide);
				//We no longer want to render the bottom left corner - it's been rendered by the subsquare
				vBL = false;
			}
		}

		//If the west side is active...
		if(isActive(x, cZ))
		{
			//It's a side vertex, so check if the center of the corresponding subsquare is enabled
			if(halfSide > 1 && isActive(cX - halfSide/2, cZ - halfSide/2))
			{
				//If the center's enabled, this quad is subdivided, so render the interior
				Render(x, z, halfSide);
				//We no longer want to render the top left corner - it's been rendered by the subsquare
				vTL = false;
			}
		}

		//If the north side is active...
		if(isActive(cX, z))
		{
			//It's a side vertex, so check if the center of the corresponding subsquare is enabled
			if(halfSide > 1 && isActive(cX + halfSide/2, cZ - halfSide/2))
			{
				//If the center's enabled, this quad is subdivided, so render the interior
				Render(cX, z, halfSide);
				//We no longer want to render the top right corner - it's been rendered by the subsquare
				vTR = false;
			}
		}

		//Render the quadrants that remain unrendered, beginning from the bottom right quadrant and going clockwise

		if(vBR)
		{
			glBegin(GL_TRIANGLE_FAN);
			terrVertex(cX, cZ);				//Center
			
			if(isActive(brX, cZ))			//If East is active
				terrVertex(brX, cZ);		//East

			terrVertex(brX, brZ);			//South-east

			if(!vBL)
			{
				if(isActive(cX, brZ))
					terrVertex(cX, brZ);		//South (if not rendered next)
				glEnd();					//End if next clockwise was already rendered

				//STATISTICS
				if(!computedStats)
					numTrgls += numVerts - lastVerts - 2;	lastVerts = numVerts;
			}
		}
		if(vBL)
		{
			if(!vBR)
			{
				glBegin(GL_TRIANGLE_FAN);
				terrVertex(cX, cZ);			//Center
			}

			if(isActive(cX, brZ))			//If South is active
				terrVertex(cX, brZ);		//South

			terrVertex(x, brZ);				//South-west

			if(!vTL)
			{
				if(isActive(x, cZ))
					terrVertex(x, cZ);			//West (if not rendered next)
				glEnd();					//End if next clockwise was already rendered

				//STATISTICS
				if(!computedStats)
					numTrgls += numVerts - lastVerts - 2;	lastVerts = numVerts;
			}
		}
		if(vTL)
		{
			if(!vBL)
			{
				glBegin(GL_TRIANGLE_FAN);
				terrVertex(cX, cZ);				//Center
			}

			if(isActive(x, cZ))					//If West is active
				terrVertex(x, cZ);				//West

			terrVertex(x, z);					//North-west

			if(!vTR)
			{
				if(isActive(cX, z))
					terrVertex(cX, z);				//North (if not rendered next)
				glEnd();						//End if next clockwise was already rendered

				//STATISTICS
				if(!computedStats)
					numTrgls += numVerts - lastVerts - 2;	lastVerts = numVerts;
			}
		}
		if(vTR)
		{
			if(!vTL)
			{
				glBegin(GL_TRIANGLE_FAN);
				terrVertex(cX, cZ);				//Center
			}

			if(isActive(cX, z))					//If North is active
				terrVertex(cX, z);				//North

			terrVertex(brX, z);					//North-east

			if(isActive(brX, cZ))				//If East is active
				terrVertex(brX, cZ);			// East
			else								//Otherwise
				terrVertex(brX, brZ);			// we end at the South-East to close it off

			glEnd();							//End - We've come full circle

			//STATISTICS
			if(!computedStats)
				numTrgls += numVerts - lastVerts - 2;	lastVerts = numVerts;
		}
	}
}