#ifndef _READINGSTL_H
#define _READINGSTL_H

#include <string>
#include <vector>

#define BINARYFILE(STLFile) (IS_BIN=true)
#define ASCIIFILE(STLFile) (IS_BIN=false)

struct TriangularPlanes{
    float Vertex1[3];
    float Vertex2[3];
    float Vertex3[3];
    float Normal[3];
};

class STLFile{
    public:
        int FacetCount(std::string);
        bool IS_BIN;

        TriangularPlanes* ReadingSolidVertexNormal(int,std::string);
};

// For STL conversion

void Convert(std::string, TriangularPlanes*,int, STLFile);


#endif
