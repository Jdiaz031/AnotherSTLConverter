#include <iostream>
#include <stdio.h>
#include "ReadingSTL.h"

int main(int argc, char** argv){
    STLFile stl;
    int faces=stl.FacetCount(argv[1]);
    TriangularPlanes* VertsNorms=stl.ReadingSolidVertexNormal(faces,argv[1]);
    printf("%s is in ", argv[1]);
    if(stl.IS_BIN){
        printf("BINARY format,converting to ASCII: %s",argv[2]);
    }
    else{
        printf("ASCII format,converting to BINARY: %s",argv[2]);
    }
    Convert(argv[2],VertsNorms,faces,stl);
    return 0;
}
