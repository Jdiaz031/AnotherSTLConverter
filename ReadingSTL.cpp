#include <stdio.h>
#include <iostream>
#include <string>
#include<fstream>
#include<sstream>
#include "../include/ReadingSTL.h"

int STLFile::FacetCount(std::string STL_FILEPATH){
    std::ifstream BIN_READ(STL_FILEPATH,std::ios::binary);
    int Facets;
    BIN_READ.seekg(80);
    BIN_READ.read(reinterpret_cast<char*>(&Facets),sizeof(uint32_t));
    BIN_READ.close();
    std::ifstream ASCII_READ(STL_FILEPATH);
    int ASCIIFaceCount=0;
    std::string line;
    while(std::getline(ASCII_READ,line)){
            if(line.find("normal") !=std::string::npos){
                ASCIIFaceCount++;
            }
        }
    ASCII_READ.close();
    if(Facets>ASCIIFaceCount && ASCIIFaceCount==0){
        BINARYFILE(STLFile);
        return Facets;
    }
    else{
        ASCIIFILE(STLFile);
        return ASCIIFaceCount;
    }
}

//Function to obtain the vertices of an ASCII STL file
std::vector<float> ParseVectors(const std::string& line){
    std::vector<float> vertex;
    std::istringstream iss(line);
    std::string token;
    iss>>token;
    while (iss>>token){
        vertex.push_back(std::stod(token));
    }
    return vertex;
}

//Function to obtain the normals of an ASCII STL file
std::vector<float> ParseNormal(const std::string& line){
    std::vector<float> normal;
    std::istringstream iss(line);
    std::string token;
    for(int i=0; i<2;++i){
        iss>>token;
    }
    while (iss>>token){
        normal.push_back(std::stod(token));
    }
    return normal;
}

TriangularPlanes* STLFile::ReadingSolidVertexNormal(int FacetCount,
        std::string STL_FILEPATH){
    TriangularPlanes* Planes = new TriangularPlanes[FacetCount];
    if(IS_BIN){
        std::ifstream File(STL_FILEPATH,std::ios::binary);
        int position;
        position=84;
        File.seekg(position);
        for(int i=0;i<=FacetCount;++i){
            File.read(reinterpret_cast<char*>(Planes[i].Normal),3*sizeof(float));
            position+=12;
            File.seekg(position);
            File.read(reinterpret_cast<char*>(Planes[i].Vertex1),3*sizeof(float));
            position+=12;
            File.seekg(position);
            File.read(reinterpret_cast<char*>(Planes[i].Vertex2),3*sizeof(float));
            position+=12;
            File.seekg(position);
            File.read(reinterpret_cast<char*>(Planes[i].Vertex3),3*sizeof(float));
            position+=14;
            File.seekg(position);
        }
        File.close();
    }
    else if(!IS_BIN){
        std::ifstream File(STL_FILEPATH);
        std::string line;
        int planes_count=0;
        int vertex_iterations=0;
        while (std::getline(File,line)){
            if(line.find("normal") !=std::string::npos){
                std::vector<float> normal= ParseNormal(line);
                std::copy(normal.begin(),normal.end(),
                        Planes[planes_count].Normal);
                }
                else if(line.find("vertex") !=std::string::npos){
                    std::vector<float> vert=ParseVectors(line);
                    switch (vertex_iterations){
                        case 0:
                            std::copy(vert.begin(),vert.end(),
                                    Planes[planes_count].Vertex1);
                            vertex_iterations++;
                            break;
                        case 1:
                            std::copy(vert.begin(),vert.end(),
                                    Planes[planes_count].Vertex2);
                            vertex_iterations++;
                            break;
                        case 2:
                            std::copy(vert.begin(),vert.end(),
                                    Planes[planes_count].Vertex3);
                            vertex_iterations=0;
                            planes_count++;
                            break;
                    }
                }
            }
        File.close();
    }
    return Planes;
}

// For STL conversion
void Convert(std::string filename, TriangularPlanes* Planes, int faces, STLFile Stl){
    if(Stl.IS_BIN){
        std::ofstream NewSTL(filename);
        NewSTL<<"solid Hoozus Program\n";
        for(int i=0;i<faces;i++){
            NewSTL<<"  facet normal ";
            NewSTL<<Planes[i].Normal[0]<<" "<<
                Planes[i].Normal[1]<<" "<<
                Planes[i].Normal[2]<<"\n";
            NewSTL<<"    outer loop\n";
            NewSTL<<"      vertex ";
            NewSTL<<Planes[i].Vertex1[0]<<" "<<
                Planes[i].Vertex1[1]<<" "<<
                Planes[i].Vertex1[2]<<"\n";
            NewSTL<<"      vertex ";
            NewSTL<<Planes[i].Vertex2[0]<<" "<<
                Planes[i].Vertex2[1]<<" "<<
                Planes[i].Vertex2[2]<<"\n";
            NewSTL<<"      vertex ";
            NewSTL<<Planes[i].Vertex3[0]<<" "<<
                Planes[i].Vertex3[1]<<" "<<
                Planes[i].Vertex3[2]<<"\n";
            NewSTL<<"    endloop\n";
            NewSTL<<"  endfacet\n";
        }
        NewSTL<<"endsolid Hoozus Program";
        NewSTL.close();
    }
    else{
        std::ofstream NewSTL(filename,std::ios::binary);
        std::string Header;
        std::string ByteCount;
        Header.resize(80,' ');
        ByteCount.resize(2,' ');
        NewSTL.write(Header.c_str(),80);
        NewSTL.write(reinterpret_cast<const char*>(&faces),sizeof(faces));
        for(int i=0;i<faces;++i){
            NewSTL.write(reinterpret_cast<const char*>(&Planes[i].Normal),
                    sizeof(Planes[i].Normal));
            NewSTL.write(reinterpret_cast<const char*>(&Planes[i].Vertex1),
                    sizeof(Planes[i].Vertex1));
            NewSTL.write(reinterpret_cast<const char*>(&Planes[i].Vertex2),
                    sizeof(Planes[i].Vertex2));
            NewSTL.write(reinterpret_cast<const char*>(&Planes[i].Vertex3),
                    sizeof(Planes[i].Vertex3));
            NewSTL.write(ByteCount.c_str(),2);
        }
        NewSTL.close();
    }
}

