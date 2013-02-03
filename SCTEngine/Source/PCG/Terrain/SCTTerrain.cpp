#include <SCTCore.h>
#include <PCG\Terrain\SCTTerrain.h>
#include <Render\SCTVertexDefs.h>
#include <Resource\SCTMesh.h>
#include <time.h>
#include <SCTMathDebug.h>


namespace SCT
{
namespace PCG
{

SCTTerrain::SCTTerrain()
:	mpMesh (NULL),
	mHeightmapSize (0)
{
}

SCTTerrain::~SCTTerrain()
{
}

SCTReturn SCTTerrain::Initialize()
{
	srand(SUInt32(time(0)));

	return OK;
}

SCTReturn SCTTerrain::Shutdown()
{
	return OK;
}

SCTReturn SCTTerrain::GenerateTerrain(const SCTTerrainConfig &config)
{
	SCTReturn ret;

	// 1. Generate the grid for the terrain
	GenerateGrid(config.grid);

	// Get Vertex buffer
	//Render::SCTVertexPosNormTex	*vertexBuffer;
	Render::SCTVertexPosTangBiNormTex	*vertexBuffer;
	SUInt32								vertexCount	= 0;
	mpMesh->GetVertexBuffer(&vertexBuffer, vertexCount);

	if(!vertexBuffer || !vertexCount)
		return CORRUPTED_VERTEX_BUFFER;


	// 2. Load Elevation map from image & process it
	std::vector<ColorRGB> imageData;
	SUInt32 gridSize	= (SUInt32)sqrt(float(vertexCount));
	ret = Core::SCTImageLoader::getSingleton().LoadImageFromFile(Core::SCTStringHelper::getSingleton().stringToWString(config.elevationMap.filename).c_str(),
																imageData, 
																true, 
																gridSize, gridSize);

	ProcessElevationMap(imageData, config.elevationMap, mElevationMap);
	
	SCTPrintD("Elevation Map size: %d, vertex count: %d", mElevationMap.size(), vertexCount);

	// 3. Apply the Diamond-Square algorithm
	if(!ApplyDiamondSquare(vertexBuffer, vertexCount, config.dSquareConfig, config.height))
		Core::SCTLogManager::getSingleton().PrintDebug("Failed to apply Diamond-Square algorithm!");

	/*
	// TEST: take the seabed further down
	for(SUInt32 i = 0; i < vertexCount; i++)
	{
		if(mElevationMap[i] < 0)
			vertexBuffer[i].position.y += 2*mElevationMap[i];
	}
	*/

	// 4a. Post-processing: Faulting
	if(config.applyFaulting)
		ApplyFaulting(vertexBuffer, vertexCount, config.faultingConfig, config.height);

	// 4b. Post-processing: Smooth
	if(config.smoothTerrain)
	{
		for(SUInt32 i = 0; i < config.smoothConfig.numPasses; i++)
		{
			switch(config.smoothConfig.type)
			{
			// Apply the smooth Band filter to the terrain
			case SCTSmoothConfig::eBandFilter:
				{
					SmoothTerrainBandFilter(vertexBuffer, vertexCount, config.smoothConfig.pass[i].kFactor);
				} break;

			// Apply the Matrix smooth filter
			case SCTSmoothConfig::eMatrixFilter:
				{
					SmoothTerrainMatrixFilter(vertexBuffer, vertexCount, config.smoothConfig.pass[i].kFactor);
				} break;
			}
		}
	}

	// 5. Calculate the terrain's normals, tangents and binormals
	if(!CalculateNormals())
		SCTPrintD("Failed to calculate the terrain normals");

	if(!CalculateTangentsBinormals())
		SCTPrintD("Failed to calculate the terrain tangents and binormals");

	// 6. Store the generated terrain to a heightmap
	// Size of the height map (size * size)
	mHeightmapWidth	= config.grid.size;
	mHeightmapSize	= mHeightmapWidth * mHeightmapWidth;

	// Store the heightmap & a vector with just the positions (bound box calculations)
	float maxHeight = 0;
	std::vector<Math::Vector3> terrainPositions;
	for(SUInt32 i = 0; i < vertexCount; i++)
	{
		float currentHeight = vertexBuffer[i].position.y;
		mHeightmap.push_back(currentHeight);

		if(currentHeight > maxHeight)
			maxHeight = currentHeight;

		// Store the position
		terrainPositions.push_back(vertexBuffer[i].position);
	}

	std::cout<<"[0,0]";
	Math::DumpVector3(vertexBuffer[0].position);

	// Do bound box calculation for this terrain part
	Math::BoundingBox aabb;
	aabb.Calculate(&terrainPositions[0], terrainPositions.size());

	// Update the vertex buffer
	mpMesh->SetAABB(aabb);
	mpMesh->SetVertexBuffer(vertexBuffer, vertexCount, Render::ePosTangBiNormTex);

	terrainPositions.clear();
	return OK;
}

float SCTTerrain::GetTerrainHeight(float x, float z, float tileSpacing)
{
	/* DOESNT WORK RIGHT
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f * mHeightmapWidth) / -tileSpacing;
	float d = (z + 0.5f * mHeightmapWidth) / -tileSpacing;

	std::cout << c << ", " << d << ", x=" << x << ",z=" << z << "\n";

	// Get the row and column we are in.
	SInt32 row = abs((SInt32)floorf(d)) - 1;
	SInt32 col = abs((SInt32)floorf(c)) - 1;

	//std::cout<<"Row: " << row << ", col: " << col << "\n";

	// Grab the heights of the cell we are in.
	float A = row * mHeightmapWidth + col;
	float B = row * mHeightmapWidth + col + 1;
	float C = (row + 1) * mHeightmapWidth + col;
	float D = (row + 1) * mHeightmapWidth + col + 1;

	if(A<0 || B<0 || C<0 || D<0)
		return 1000.0f;
	if(A>(mHeightmapSize-1) || B>(mHeightmapSize-1) || C>(mHeightmapSize-1) || D>(mHeightmapSize-1))
		return 1000.0f;

	//std::cout<<A<<", "<<B<<", "<<C<<", "<<D<<"\n";

	// Get the heights
	float hA	= mHeightmap[A];
	float hB	= mHeightmap[B];
	float hC	= mHeightmap[C];
	float hD	= mHeightmap[D];
	
	//float s	= c - (float(col));
	//float t	= d - (float(row));

	//std::cout << s << "," << t << "\n";

	/*
	float height = 0;
	// If upper triangle ABC.
	if( s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		height = A + uy + vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		height = D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
	*/
	//std::cout<<"height [GetTerrainHeight]: " << height;
	//std::cout<<"Position: row=" << row << ", col=" << col << "\n";
	
	return 0;
}

/* --------------------------------------------------------------------------
		Terrain Generation Functions Functions
   -------------------------------------------------------------------------- */
SCTReturn SCTTerrain::ApplyFaulting(Render::SCTVertexPosTangBiNormTex *vertices, SUInt32 vertexCount, const SCTFaulting &config, const Math::Range &height, float dFactor)
{
	// Calculate the size of the grid (the grid is square)
	SUInt32	size	= SUInt32(sqrt((float)vertexCount));

	config;
	vertices;
	height;

	// faulting algorithm configuration
	float	displacement		= config.displacement;
	//float	l_displacementStart	= displacement_start;
	//float	l_step				= step;

	// vars
	SInt32			i1, i2;						// random points on the grid
	Math::Vector2	lineP1, lineP2, faultLine, currentPoint;
	
	// Terrain generation Loop
	for(SUInt32 i = 0; i< config.iterations; i++)
	{
		//if(technique == DISPLACEMENT_RANDOM)
			//l_displacement = (rand() % 10) * 0.1;

		// Pick two random points in the grid to form the line
		i1	= rand() % (vertexCount - 1);
		i2 = i1;

		while (i2 == i1)
		{
			i2 = rand() % (vertexCount - 1);
		}

		// Get the P1 and P2 for the random faulting line
		lineP1.x	= vertices[i1].position.x;
		lineP1.y	= vertices[i1].position.z;

		lineP2.x	= vertices[i2].position.x;
		lineP2.y	= vertices[i2].position.z;

		// Calculate the fault line (vector P2 - P1)
		faultLine.x	= lineP2.x - lineP1.x;
		faultLine.y	= lineP2.y - lineP1.y;

		// Loop through every point in the grid
		for(SUInt32 j = 0; j < vertexCount; j++)
		{
			// Get the point's x and z and create the currentPoint
			currentPoint.x	= vertices[j].position.x - lineP1.x;
			currentPoint.y	= vertices[j].position.z - lineP1.y;

			// Calculate the cross product (Y axis) of the faulting line
			// and the current vertex
			float crossProdY	= (faultLine.x * currentPoint.y - faultLine.y * currentPoint.x);

			// find the cross product of v_line and v_tPoint,
			// and decide which direction to displace
			if(crossProdY > 0)
				vertices[j].position.y	+= displacement;
			else
				vertices[j].position.y	-= displacement;
		}
		/*
		// increase, decrease the displacement
		if(technique == DISPLACEMENT_INCREMENTAL)
			l_displacement	+= l_step;
		if(technique == DISPLACEMENT_DECREMENTAL)
			l_displacement	-= l_step;
		*/
	}	// end of faulting algorithm

	return OK;
}

SCTReturn SCTTerrain::ApplyParticleDeposition(Render::SCTVertexPosTangBiNormTex *vertices, SUInt32 vertexCount, const SCTParticleDeposition &config, const Math::Range &height, float dFactor)
{
	// Calculate the size of the grid (the grid is square)
	SUInt32	size	= SUInt32(sqrt((float)vertexCount));

	float	radius			= config.radius;
	
	// Jumps
	for(SUInt32 iJump = 0; iJump < config.jumps; iJump++)
	{
		std::cout<<"Jump: " << iJump <<"\n";

		// Get a random point on the grid (root)
		SUInt32 rootIndex	= rand() % (size*size);

		// Get all the neighbours of root in the radius


		// Get a random number of particles to drop in this area	
		SUInt32 numParticles	= (SUInt32)Math::GetRandom(config.particlesPerJump.min, config.particlesPerJump.max);
		
		numParticles	= 5;
		for(SUInt32 iParticle = 0; iParticle < numParticles; iParticle++)
		{
			std::cout<<"iParticle: " << iParticle + 1 << "/" <<numParticles <<"\n";
			DropParticle(rootIndex, rootIndex, radius, dFactor, vertices, size);
		}

	}	//\ End of Jumps


	return OK;
}

SCTReturn SCTTerrain::ApplyDiamondSquare(Render::SCTVertexPosTangBiNormTex *vertices, SUInt32 vertexCount, const SCTDiamondSquare &config, const Math::Range &height, float dFactor)
{
	// Calculate the size of the grid (the grid is square)
	SUInt32	size	= SUInt32(sqrt((float)vertexCount));

	// Initialize diamond square variables
	SUInt32 sideLength	= size - 1;
	float	hOffset		= (float)sideLength;
	float	r			= pow(2, -1 * config.roughness);
	
	// Height offset
	float	randomOffset;

	while(sideLength >= 2)
	{
		// Construct neighbor list
		SInt32	sideLnDist	= size * (sideLength/2);

		SInt32	neighbor[]	= { (sideLnDist + SInt32(sideLength/2)), (sideLnDist - SInt32(sideLength/2)),
								-(sideLnDist + SInt32(sideLength/2)), -(sideLnDist - SInt32(sideLength/2))};

		SInt32	neighborSq[]	= { (sideLnDist), (SInt32(sideLength/2)), -(sideLnDist), -(SInt32(sideLength/2)) };

		// A. Diamond step
		for(SUInt32 i = 0; i < size-1; i += sideLength)
		{
			for(SUInt32 j = 0; j < size-1; j += sideLength)
			{
				// Get the Midpoint index
				SInt32 mi	= (i + sideLength/2);
				SInt32 mj	= (j + sideLength/2);

				SInt32 midIndex	= mi + mj*size;

				// Calculate the corners -- using neighbour list
				SInt32 index0 = midIndex + neighbor[0];
				SInt32 index1 = midIndex + neighbor[1];
				SInt32 index2 = midIndex + neighbor[2];
				SInt32 index3 = midIndex + neighbor[3];

				// Get the average height of the corners
				float average	=	vertices[index0].position.y	+
									vertices[index1].position.y	+
									vertices[index2].position.y	+
									vertices[index3].position.y;

				average /= 4.0f;

				// Raise the middle point by average + hOffset
				//randomOffset	= Math::GetRandom(-hOffset * mElevationMap[midIndex], hOffset * mElevationMap[midIndex]);
				randomOffset	= Math::GetRandom(0, hOffset * mElevationMap[midIndex]);
				vertices[midIndex].position.y	= (average + randomOffset);
			}
		}


		// B. Square step
		for(SUInt32 i = 0; i < size-1; i += sideLength)
		{
			for(SUInt32 j = 0; j < size-1; j += sideLength)
			{
				// Get the Midpoint index
				SInt32 mi	= (i + sideLength/2);
				SInt32 mj	= (j + sideLength/2);

				SInt32 midIndex	= mi + mj*size;

				// Get the corners
				SInt32 index0 = midIndex + neighbor[0];
				SInt32 index1 = midIndex + neighbor[1];
				SInt32 index2 = midIndex + neighbor[2];
				SInt32 index3 = midIndex + neighbor[3];

				// Get the midpoints to elevate
				SInt32 indexMid0 = midIndex + neighborSq[0];
				SInt32 indexMid1 = midIndex + neighborSq[1];
				SInt32 indexMid2 = midIndex + neighborSq[2];
				SInt32 indexMid3 = midIndex + neighborSq[3];

				// Elevate the corners by average + random ranged value
				//randomOffset	= Math::GetRandom(-hOffset * mElevationMap[indexMid0], hOffset * mElevationMap[indexMid0]);
				randomOffset	= Math::GetRandom(0, mElevationMap[indexMid0]);
				vertices[indexMid0].position.y = (	(vertices[index0].position.y	+ vertices[index1].position.y
												+ vertices[index2].position.y + vertices[index3].position.y)/4.0f 
												+ randomOffset);

				//randomOffset	= Math::GetRandom(-hOffset * mElevationMap[indexMid1], hOffset * mElevationMap[indexMid1]);
				randomOffset	= Math::GetRandom(0, mElevationMap[indexMid1]);
				vertices[indexMid1].position.y = (	(vertices[index0].position.y + vertices[index1].position.y 
												+ vertices[index2].position.y	+ vertices[index3].position.y)/4.0f 
												+ randomOffset);

				//randomOffset	= Math::GetRandom(-hOffset * mElevationMap[indexMid2], hOffset * mElevationMap[indexMid2]);
				randomOffset	= Math::GetRandom(0, mElevationMap[indexMid2]);
				vertices[indexMid2].position.y = (	(vertices[index0].position.y + vertices[index1].position.y 
												+ vertices[index2].position.y	+ vertices[index3].position.y)/4.0f 
												+ randomOffset);

				//randomOffset	= Math::GetRandom(-hOffset * mElevationMap[indexMid3], hOffset * mElevationMap[indexMid3]);
				randomOffset	= Math::GetRandom(0, mElevationMap[indexMid3]);
				vertices[indexMid3].position.y = (	(vertices[index0].position.y + vertices[index1].position.y 
												+ vertices[index2].position.y	+ vertices[index3].position.y)/4.0f 
												+ randomOffset);
			}
		}


		// Subdivide the grid, multiply height offset by r
		sideLength	/= 2;
		hOffset		*= r;
	}	

	return OK;
}

/* --------------------------------------------------------------------------
		Private Functions
   -------------------------------------------------------------------------- */
bool SCTTerrain::DropParticle(SUInt32 currentIndex, SUInt32 rootIndex, float radius, float dFactor, Render::SCTVertexPosTangBiNormTex	*vertices, SInt32	gridSize)
{
	rootIndex = 12;

	// construct a list of neighbours from root based on the radius

	/*
	// Predefined list of neighbours
	SInt32	neighbours[]	=			{1, (gridSize-1), gridSize, (gridSize+1), -1, -(gridSize-1), -gridSize, -(gridSize+1)};
	bool	visited[8];				// defines if a neighbour has been visited
	SUInt32	nbCount			= 8;	// neighbour count

	// Clear the visited list
	for(SUInt32 i=0; i<nbCount; i++)
		visited[i]	= false;

	// Check if the current vertex is in the radius of the root vertex
	Math::Vector2	current(vertices[currentIndex].position.x, vertices[currentIndex].position.z);
	Math::Vector2	root(vertices[rootIndex].position.x, vertices[rootIndex].position.z);
	
	// Distance of current vertex from root
	float distFromRoot = (current - root).Length();

	// Yay! the current vertex is inside the radius of the root
	if(distFromRoot <= radius)
	{
		std::cout	<<"* current = " << currentIndex <<", root = " << rootIndex
					<<", distFromRoot = " << distFromRoot <<"\n";

		// Check height of current vs root
		if(vertices[currentIndex].position.y < vertices[rootIndex].position.y)
		{
			vertices[currentIndex].position.y += 1.0f * dFactor;
			std::cout<<"current +\n";
			return true;
		}
		
		// Cannot deposit particle here, visit neighbours
		bool done		= false;
		bool allVisited = false;

		while(!allVisited && !done)			// -- bug: infinite
		{
			// select random neighbour
			SUInt32 nIndex;
			do
			{
				// Pick random neighbour
				nIndex	= rand() % 8;

				// If this neighbour is the root, mark it as visited
				if((currentIndex + neighbours[nIndex]) == rootIndex)
					visited[nIndex]	= true;

				// If the neighbor is not on the grid, mark it as visited
				if((currentIndex + neighbours[nIndex]) >= (gridSize*gridSize))
					visited[nIndex]	= true;

				if((SInt32(currentIndex) + neighbours[nIndex]) < 0)
					visited[nIndex]	= true;

				allVisited =	visited[0] && visited[1] && visited[2] && visited[3] &&
								visited[4] && visited[5] && visited[6] && visited[7];  

			} while(visited[nIndex] && !allVisited);


			// This neighbor is fine, vist it and do the same...
			visited[nIndex]	= true;
			std::cout << "\tNeighbor index = " << (currentIndex + neighbours[nIndex]) << "|\t" << visited[nIndex] << "\n";
			done = DropParticle((currentIndex + neighbours[nIndex]), rootIndex, radius, dFactor, vertices, gridSize);
		}

		// Could not drop particle to neighbour
		// This is root, so elevate root
		if(!done && (currentIndex == rootIndex))
		{
			vertices[rootIndex].position.y += 1.0f * dFactor;
			std::cout<<"Root+\n";

			done = true;
		}

		return done;
		// DO ElevateNeighbor while !done
	}
	*/
	return false;
}

// Elevation Map functions
void SCTTerrain::ProcessElevationMap(const std::vector<ColorRGB> &inRawImageData, const SCTElevationMap &config, std::vector<float> &outElevationMap)
{
	std::vector<ColorRGB>			imageData	= inRawImageData;
	std::vector<ColorRGB>::iterator it;
	for(it = imageData.begin(); it != imageData.end(); it++)
	{
		ColorRGB color = *it;
		
		// Mountains
		if((color.r > color.g) && (color.r > color.b))
			outElevationMap.push_back(Math::GetRandom(0.6f, 0.8f));

		// Plains
		else if((color.g >= color.r) && (color.g >= color.b))
			outElevationMap.push_back(Math::GetRandom(0, 0.1f));

		// Seabed
		else if((color.b >= color.r) && (color.b > color.g))
			outElevationMap.push_back(Math::GetRandom(-0.4f, -0.3f));
		
		else
			outElevationMap.push_back(1.0f);
	}
}

SCTReturn SCTTerrain::GenerateGrid(const SCTGrid &grid)
{
	SUInt32 size					= grid.size;
	SCTGrid::SCTGridPattern pattern	= grid.pattern;
	SUInt32 texRep					= grid.textureRepetitions;

	// Create the vertex buffer
	if(size % 2 == 0)
		size += 1;		// odd number for vertices (makes diamond square easier)

	// Calculate the number of vertices
	SUInt32 numVertices		= size * size;
	SUInt32 currentVertex	= 0;

	// Create the Vertex array
	Render::SCTVertexPosTangBiNormTex *vertices	= new Render::SCTVertexPosTangBiNormTex[numVertices];

	float halfSize	= size / 2.0f;
	float step		= float(texRep) / (size - 1);	// Used to calculate the Texture coords

	for(SUInt32 i = 0; i < size; i++)	// i --> Zaxis
	{
		for(SUInt32 j = 0; j < size; j++)
		{
			vertices[currentVertex].position	= Math::Vector3(-halfSize + j, 0, -halfSize + i);
			vertices[currentVertex].normal		= Math::Vector3(0, 1.0f, 0);
			vertices[currentVertex].texture		= Math::Vector2(j * step, 1.0f - i * step);
			vertices[currentVertex].tangent		= Math::Vector3(0, 0.0000001f, 0);
			vertices[currentVertex].binormal	= Math::Vector3(0, 0.0000001f, 0);


			currentVertex++;
		}
	}

	// Generate the Index buffer
	SUInt32 *indices	= NULL;
	SUInt32 indexCount	= 0;
	mpMesh->GetIndexBuffer(&indices, indexCount);

	if(indices)
		delete [] indices;

	GenerateIndices(size, pattern, &indices, indexCount);
	
	// Pass the vertex and index buffers to the submesh
	mpMesh->SetVertexBuffer(vertices, numVertices, Render::ePosTangBiNormTex);
	mpMesh->SetIndexBuffer(indices, indexCount);

	return OK;
}

SCTReturn SCTTerrain::GenerateIndices(SUInt32 size, SCTGrid::SCTGridPattern pattern, SUInt32 **indexBuffer, SUInt32 &indexCount)
{
	// Calculate the number of indices based on the size of the grid
	SUInt32	numIndices	= (SUInt32)pow(float(size-1), 2) * 6;	// number of faces * number of indices per face
	
	SUInt32	*indices	= new SUInt32[numIndices];
	
	// A. Apply the Standard patter to the index buffer
	if(pattern == SCTGrid::eStandard)
	{
		int currentIndex	= 0;

		for(SUInt32 i = 0; i < (size - 1) ; i++)
		{
			for(SUInt32 j = 0; j < (size - 1); j++)
			{
				indices[currentIndex]		= (i+1) * size + j;
				indices[currentIndex + 1]	= i *size + j + 1;
				indices[currentIndex + 2]	= i *size + j;

				indices[currentIndex + 3]	= (i+1) * size + j;
				indices[currentIndex + 4]	= (i+1) * size + j + 1;
				indices[currentIndex + 5]	= i *size + j + 1;

				currentIndex += 6;
			}
		}
	}

	// B. Apply the Quilt pattern to the index buffer
	if(pattern == SCTGrid::eQuilt)
	{
		int currentIndex	= 0;
		int rootIndex		= 0;

		for(SUInt32 i = 0; i < (size - 1) ; i++)
		{
			for(SUInt32 j = 0; j < (size - 1); j++)
			{
				if((j+i) % 2 == 0)	// j + Width as r\noot
				{
					indices[currentIndex]		= (i+1) * size + j;
					indices[currentIndex + 1]	= i *size + j + 1;
					indices[currentIndex + 2]	= i *size + j;

					indices[currentIndex + 3]	= (i+1) * size + j;
					indices[currentIndex + 4]	= (i+1) * size + j + 1;
					indices[currentIndex + 5]	= i *size + j + 1;

					currentIndex += 6;
				}
				else		// j as root
				{
					indices[currentIndex]		= i *size + j;
					indices[currentIndex + 1]	= (i+1) * size + j;
					indices[currentIndex + 2]	= (i+1) * size + j + 1;

					indices[currentIndex + 3]	= i *size + j;
					indices[currentIndex + 4]	= (i+1) * size + j + 1;
					indices[currentIndex + 5]	= i * size + j + 1;

					currentIndex += 6;
				}
			}	//\ End of j
		}	//\ End of i
	}	//\ End of Quilt pattern
	
	// Return the index buffer and the index count
	*indexBuffer	= indices;
	indexCount		= numIndices;

	return OK;
}

SCTReturn SCTTerrain::CalculateNormals()
{
	Render::SCTVertexPosTangBiNormTex	*vertexBuffer	= NULL;
	SUInt32								*indexBuffer	= NULL;
	SUInt32								vertexCount		= 0;
	SUInt32								indexCount		= 0;

	mpMesh->GetVertexBuffer(&vertexBuffer, vertexCount);
	mpMesh->GetIndexBuffer(&indexBuffer, indexCount);

	// Calculate the size of the grid
	SInt32	size	= SInt32(sqrt((float)vertexCount));

	if(vertexBuffer && indexBuffer)
	{
		// Clear normals
		for(SUInt32 i = 0; i < vertexCount; i++)
			vertexBuffer[i].normal	= Math::Vector3(0, 0, 0);

		// For every face
		SUInt32 numFaces	= indexCount / 3;
		for(SUInt32 i = 0; i < numFaces; i++)
		{
			Math::Vector3 v0		= vertexBuffer[indexBuffer[3 * i + 0]].position;
			Math::Vector3 v1		= vertexBuffer[indexBuffer[3 * i + 1]].position;
			Math::Vector3 v2		= vertexBuffer[indexBuffer[3 * i + 2]].position;

			Math::Vector3 v01		= v1 - v0;
			Math::Vector3 v02		= v2 - v0;

			// Calculate the normal of this face
			Math::Vector3 normal	= v01.Cross(v02);

			vertexBuffer[indexBuffer[3 * i + 0]].normal += normal;
			vertexBuffer[indexBuffer[3 * i + 1]].normal += normal;
			vertexBuffer[indexBuffer[3 * i + 2]].normal += normal;
		}

		// Renormalise normals
		for(SUInt32 i = 0; i < vertexCount; i++)
			vertexBuffer[i].normal.Normalize();
	}
	
	return OK;
}

// The code for calculating the Tangents and Binormals,
// was taken from the forum http://forums.create.msdn.com/forums/p/30443/172880.aspx
// post of user user 'Koder4Fun'
SCTReturn SCTTerrain::CalculateTangentsBinormals()
{
	Render::SCTVertexPosTangBiNormTex	*vertexBuffer	= NULL;
	SUInt32								*indexBuffer	= NULL;
	SUInt32								vertexCount		= 0;
	SUInt32								indexCount		= 0;

	mpMesh->GetVertexBuffer(&vertexBuffer, vertexCount);
	mpMesh->GetIndexBuffer(&indexBuffer, indexCount);

	// Calculate the size of the grid
	SInt32	size	= SInt32(sqrt((float)vertexCount));

	if(vertexBuffer && indexBuffer)
	{
		// Clear tangent, binormal
		for(SUInt32 i = 0; i < vertexCount; i++)
		{
			vertexBuffer[i].tangent		= Math::Vector3(0, 0, 0);
			vertexBuffer[i].binormal	= Math::Vector3(0, 0, 0);
		}

		// For every face
		SUInt32 numFaces	= indexCount / 3;
		for(SUInt32 i = 0; i < numFaces; i++)
		{
			// The vertices of the triangle
			Math::Vector3 v1			= vertexBuffer[indexBuffer[3 * i + 0]].position;
			Math::Vector3 v2			= vertexBuffer[indexBuffer[3 * i + 1]].position;
			Math::Vector3 v3			= vertexBuffer[indexBuffer[3 * i + 2]].position;

			// The texture coordinates of the triangle
			
			Math::Vector2 tex1			= vertexBuffer[indexBuffer[3 * i + 0]].texture;
			Math::Vector2 tex2			= vertexBuffer[indexBuffer[3 * i + 1]].texture;
			Math::Vector2 tex3			= vertexBuffer[indexBuffer[3 * i + 2]].texture;

			// Face normal
			Math::Vector3 faceNormal	= vertexBuffer[indexBuffer[3 * i + 0]].normal;
			// 
			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;
 
			float s1 = tex2.x - tex1.x;
			float s2 = tex3.x - tex1.x;
			float t1 = tex2.y - tex1.y;
			float t2 = tex3.y - tex1.y;
 
			float r = 1.0f / (s1 * t2 - s2 * t1);

			// 
			Math::Vector3 sdir = Math::Vector3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);  
			Math::Vector3 tdir = Math::Vector3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);  
 
			// Gram-Schmidt orthogonalize  
			Math::Vector3 tangent = sdir - faceNormal.Dot(sdir);  
			tangent.Normalize();  
 
			// Calculate handedness (here maybe you need to switch >= with <= depend on the geometry winding order)
			Math::Vector3 crossProd = faceNormal.Cross(sdir);

			float tangentdir = (crossProd.Dot(tdir) >= 0.0f) ? 1.0f : -1.0f;  
			Math::Vector3 binormal = faceNormal.Cross(tangent) * tangentdir;  

			// Store the tangent of this face
			vertexBuffer[indexBuffer[3 * i + 0]].tangent += tangent;
			vertexBuffer[indexBuffer[3 * i + 1]].tangent += tangent;
			vertexBuffer[indexBuffer[3 * i + 2]].tangent += tangent;

			// Store the binormals of this face
			vertexBuffer[indexBuffer[3 * i + 0]].binormal += binormal;
			vertexBuffer[indexBuffer[3 * i + 1]].binormal += binormal;
			vertexBuffer[indexBuffer[3 * i + 2]].binormal += binormal;
		}

		// Renormalise tangents and binormals
		for(SUInt32 i = 0; i < vertexCount; i++)
		{
			vertexBuffer[i].tangent.Normalize();
			vertexBuffer[i].binormal.Normalize();
		}
	}

	return OK;
}

// Smooth terrain functions
void SCTTerrain::SmoothTerrainBandFilter(Render::SCTVertexPosTangBiNormTex *vertices, SUInt32 vertexCount, float k)
{
	// Calculate the size of the grid (the grid is square)
	SUInt32	size	= SUInt32(sqrt((float)vertexCount));

	// Pass #1. Apply filter from left to right
	for(SUInt32 i = 1; i < size; i++)
	{
		for(SUInt32 j = 0; j < size; j++)
		{
			SInt32	current		= i + j * size;
			SInt32 previous		= (i-1) + j * size;

			//std::cout<< current << ", " << previous << "|\t";
			vertices[current].position.y = vertices[previous].position.y * (1-k) + vertices[current].position.y * k;
		}
		//std::cout<<"\n";
	}
	
	// Pass #2. Apply filter from right to left
	for(SUInt32 i = (size - 2); i < -1; i--)
	{
		for(SUInt32 j = (size - 1); j < -1; j--)
		{
			SInt32	current		= i + j * size;
			SInt32 previous		= (i+1) + j * size;

			//std::cout<< current << ", " << previous << "|\t";
			vertices[current].position.y = vertices[previous].position.y * (1-k) + vertices[current].position.y * k;
		}
		//std::cout<<"\n";
	}

	// Pass #3. Apply filter from bottom to top
	for(SUInt32 i = 1; i < size; i++)
	{
		for(SUInt32 j = 0; j < size; j++)
		{
			SInt32	current		= i * size + j;
			SInt32 previous		= (i-1) * size + j;

			//std::cout<< current << ", " << previous << "|\t";
			vertices[current].position.y = vertices[previous].position.y * (1-k) + vertices[current].position.y * k;
		}
		//std::cout<<"\n";
	}

	// Pass #4. Apply filter from top to bottom
	for(SUInt32 i = (size - 2); i < -1; i--)
	{
		for(SUInt32 j = (size - 1); j < -1; j--)
		{
			SInt32	current		= i * size + j;
			SInt32 previous		= (i+1) * size + j;

			//std::cout<< current << ", " << previous << "|\t";
			vertices[current].position.y = vertices[previous].position.y * (1-k) + vertices[current].position.y * k;
		}
		//std::cout<<"\n";
	}
}

void SCTTerrain::SmoothTerrainMatrixFilter(Render::SCTVertexPosTangBiNormTex *vertices, SUInt32 vertexCount, float k)
{
	// Calculate the size of the grid (the grid is square)
	SUInt32	size	= SUInt32(sqrt((float)vertexCount));

	/*
	Math::Matrix4	filter	= {	1, 0, 0, 0
								0, 1, 0, 0
								0, 0, 1, 0
								0, 0, 0, 1 };

	Math::DumpMatrix4(filter);

	for(SUInt32 i = 0; i < vertexCount; i++)
	{
		// Construct matrix of neighbour values

		// Neighbours' indices
		SUInt32		n0	= i + size;

		Math::Matrix4	filter	= {	0, 0, 0, 0
									0, 1, 0, 0
									0, 0, 0, 0
									0, 0, 0, 1 };
	
	}*/
}

}	// End of PCG Namespace
}	// End of SCT Namespace