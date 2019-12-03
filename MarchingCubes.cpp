#include <cmath>

#include "MarchingCubes.h"

MarchingCubes::MarchingCubes(float isosurface, int width, int height, int length) {

    this->isosurface = isosurface;
    this->width = width;
    this->height = height;
    this->length = length;

}

MarchingCubes::~MarchingCubes() {

}

std::vector<Vec3> MarchingCubes::March(std::vector<std::vector<std::vector<float> > > &map) {

    std::vector<Vec3> vertices;

    for (int z = 0; z < height - 1; z = z + 1) {
        for (int y = 0; y < length - 1; y = y + 1) {
            for (int x = 0; x < width - 1; x = x + 1) {

                Vec3 curVector3;
                GridCell curGirdCel;
                std::vector<Triangle> triangles;

                curVector3.x = (float)x;
                curVector3.y = (float)y;
                curVector3.z = (float)z;
                curGirdCel.point[0] = curVector3;

                curVector3.x = (float)x + 1.0;
                curVector3.y = (float)y;
                curVector3.z = (float)z;
                curGirdCel.point[1] = curVector3;

                curVector3.x = (float)x + 1.0;
                curVector3.y = (float)y + 1.0;
                curVector3.z = (float)z;
                curGirdCel.point[2] = curVector3;

                curVector3.x = (float)x;
                curVector3.y = (float)y + 1.0;
                curVector3.z = (float)z;
                curGirdCel.point[3] = curVector3;

                curVector3.x = (float)x;
                curVector3.y = (float)y;
                curVector3.z = (float)z + 1.0;
                curGirdCel.point[4] = curVector3;

                curVector3.x = (float)x + 1.0;
                curVector3.y = (float)y;
                curVector3.z = (float)z + 1.0;
                curGirdCel.point[5] = curVector3;

                curVector3.x = (float)x + 1.0;
                curVector3.y = (float)y + 1.0;
                curVector3.z = (float)z + 1.0;
                curGirdCel.point[6] = curVector3;

                curVector3.x = (float)x;
                curVector3.y = (float)y + 1.0;
                curVector3.z = (float)z + 1.0;
                curGirdCel.point[7] = curVector3;

                curGirdCel.value[0] = map[x][y][z];
                curGirdCel.value[1] = map[x + 1][y][z];
                curGirdCel.value[2] = map[x + 1][y + 1][z];
                curGirdCel.value[3] = map[x][y + 1][z];
                curGirdCel.value[4] = map[x][y][z + 1];
                curGirdCel.value[5] = map[x + 1][y][z + 1];
                curGirdCel.value[6] = map[x + 1][y + 1][z + 1];
                curGirdCel.value[7] = map[x][y + 1][z + 1];

                this->Polygonise(curGirdCel, triangles, isosurface);

                for (std::vector<Triangle>::iterator t = triangles.begin(); t != triangles.end(); ++t) {

                    vertices.push_back(t->point1);
                    vertices.push_back(t->point2);
                    vertices.push_back(t->point3);

                }
            }
        }
    }
    return vertices;
}

void MarchingCubes::Polygonise(GridCell &Grid, std::vector<Triangle> &triangles, float isosurface) {

    int cubeCase;
    Vec3 vertexList[12];

    // Check every point in the grid to determine which cube config is needed
    cubeCase = 0;
    if (Grid.value[0] > isosurface) cubeCase |= 1;
    if (Grid.value[1] > isosurface) cubeCase |= 2;
    if (Grid.value[2] > isosurface) cubeCase |= 4;
    if (Grid.value[3] > isosurface) cubeCase |= 8;
    if (Grid.value[4] > isosurface) cubeCase |= 16;
    if (Grid.value[5] > isosurface) cubeCase |= 32;
    if (Grid.value[6] > isosurface) cubeCase |= 64;
    if (Grid.value[7] > isosurface) cubeCase |= 128;

    //Cube is entirely in/out of the surface
    if (this->cubeCases[cubeCase] == 0)
        return;

    //Find the vertices where the surface intersects the cube
    if (this->cubeCases[cubeCase] & 1)
        vertexList[0] = this->Interpolate(Grid.point[0],Grid.point[1],Grid.value[0],Grid.value[1], isosurface);
    if (this->cubeCases[cubeCase] & 2)
        vertexList[1] = this->Interpolate(Grid.point[1],Grid.point[2],Grid.value[1],Grid.value[2], isosurface);
    if (this->cubeCases[cubeCase] & 4)
        vertexList[2] = this->Interpolate(Grid.point[2],Grid.point[3],Grid.value[2],Grid.value[3], isosurface);
    if (this->cubeCases[cubeCase] & 8)
        vertexList[3] = this->Interpolate(Grid.point[3],Grid.point[0],Grid.value[3],Grid.value[0], isosurface);
    if (this->cubeCases[cubeCase] & 16)
        vertexList[4] = this->Interpolate(Grid.point[4],Grid.point[5],Grid.value[4],Grid.value[5], isosurface);
    if (this->cubeCases[cubeCase] & 32)
        vertexList[5] = this->Interpolate(Grid.point[5],Grid.point[6],Grid.value[5],Grid.value[6], isosurface);
    if (this->cubeCases[cubeCase] & 64)
        vertexList[6] = this->Interpolate(Grid.point[6],Grid.point[7],Grid.value[6],Grid.value[7], isosurface);
    if (this->cubeCases[cubeCase] & 128)
        vertexList[7] = this->Interpolate(Grid.point[7],Grid.point[4],Grid.value[7],Grid.value[4], isosurface);
    if (this->cubeCases[cubeCase] & 256)
        vertexList[8] = this->Interpolate(Grid.point[0],Grid.point[4],Grid.value[0],Grid.value[4], isosurface);
    if (this->cubeCases[cubeCase] & 512)
        vertexList[9] = this->Interpolate(Grid.point[1],Grid.point[5],Grid.value[1],Grid.value[5], isosurface);
    if (this->cubeCases[cubeCase] & 1024)
        vertexList[10] = this->Interpolate(Grid.point[2],Grid.point[6],Grid.value[2],Grid.value[6], isosurface);
    if (this->cubeCases[cubeCase] & 2048)
        vertexList[11] = this->Interpolate(Grid.point[3],Grid.point[7],Grid.value[3],Grid.value[7], isosurface);


    for (int i = 0; this->triangleTable[cubeCase][i] != -1; i = i + 3) {
        Triangle newTriangle;
        newTriangle.point1 = vertexList[this->triangleTable[cubeCase][i]];
        newTriangle.point2 = vertexList[this->triangleTable[cubeCase][i + 1]];
        newTriangle.point3 = vertexList[this->triangleTable[cubeCase][i + 2]];
        triangles.push_back(newTriangle);
    }

}

Vec3 MarchingCubes::Interpolate(Vec3 &aPoint, Vec3 &bPoint, float aValue, float bValue, float iso) {
    float mu;
    Vec3 newPoint;

    if (std::abs(iso - aValue) < 0.00001)
        return aPoint;
    if (std::abs(iso - bValue) < 0.00001)
        return bPoint;
    if (std::abs(aValue - bValue) < 0.00001)
        return aPoint;

    mu = (iso - aValue) / (bValue - aValue);
    newPoint.x = aPoint.x + mu * (bPoint.x - aPoint.x);
    newPoint.y = aPoint.y + mu * (bPoint.y - aPoint.y);
    newPoint.z = aPoint.z + mu * (bPoint.z - aPoint.z);

    return newPoint;
}