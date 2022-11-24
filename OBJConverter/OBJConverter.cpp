
#include <iostream>
#include <fstream>

using namespace std;

typedef struct {
    float X, Y, Z;
} VertexType;

typedef struct {
    int VIndex1, VIndex2, VIndex3;
    int TIndex1, TIndex2, TIndex3;
    int NIndex1, NIndex2, NIndex3;
} FaceType;

void GetModelFilename(char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);

int main()
{
    char Filename[256];
    int VertexCount, TextureCount, NormalCount, FaceCount;
    char Exit;

    GetModelFilename(Filename);

    bool Result = ReadFileCounts(Filename, VertexCount, TextureCount, NormalCount, FaceCount);

    if (!Result) {
        return -1;
    }

    cout << endl;
    cout << "Vertices: " << VertexCount << endl;
    cout << "UVs: " << TextureCount << endl;
    cout << "Normals: " << NormalCount << endl;
    cout << "Faces: " << FaceCount << endl;

    Result = LoadDataStructures(Filename, VertexCount, TextureCount, NormalCount, FaceCount);

    if (!Result) {
        return -1;
    }

    cout << "\nFile has been converted" << endl;
    cout << "\nPress any key to exit";
    cin >> Exit;
}

void GetModelFilename(char* Filename) {
    bool Done = false;
    ifstream FileIn;

    while (!Done) {
        cout << "Enter model filename: ";

        cin >> Filename;

        FileIn.open(Filename);

        if (FileIn.good()) {
            Done = true;
        }
        else {
            FileIn.clear();
            cout << endl;
            cout << "File " << Filename << " could not be opened" << endl << endl;
        }
    }
}

bool ReadFileCounts(char* Filename, int& VertexCount, int& TextureCount, int& NormalCount, int& FaceCount) {
    ifstream FileIn;
    char Input;

    VertexCount = 0;
    TextureCount = 0;
    NormalCount = 0;
    FaceCount = 0;

    FileIn.open(Filename);

    if (FileIn.fail()) {
        return false;
    }

    FileIn.get(Input);
    while (!FileIn.eof()) {
        if (Input == 'v') {
            FileIn.get(Input);

            if (Input == ' ') VertexCount++;
            if (Input == 't') TextureCount++;
            if (Input == 'n') NormalCount++;
        }

        if (Input == 'f') {
            FileIn.get(Input);
            
            if (Input == ' ') FaceCount++;
        }

        while (Input != '\n') {
            FileIn.get(Input);
        }
        FileIn.get(Input);
    }
    FileIn.close();
    
    return true;
}

bool LoadDataStructures(char* Filename, int VertexCount, int TextureCount, int NormalCount, int FaceCount) {
    VertexType* Vertices{ nullptr }, * TexCoords{ nullptr }, * Normals{ nullptr };
    FaceType* Faces;
    ifstream FileIn;
    int VertexIndex, TexCoordIndex, NormalIndex, FaceIndex, VIndex, TIndex, NIndex;
    char Input, Input2;
    ofstream FileOut;

    Vertices = new VertexType[VertexCount];

    if (!Vertices) {
        return false;
    }

    TexCoords = new VertexType[TextureCount];

    if (!TexCoords) {
        return false;
    }

    Normals = new VertexType[NormalCount];

    if (!Normals) {
        return false;
    }

    Faces = new FaceType[FaceCount];

    if (!Faces) {
        return false;
    }

    VertexIndex = 0;
    TexCoordIndex = 0;
    NormalIndex = 0;
    FaceIndex = 0;

    FileIn.open(Filename);

    if (FileIn.fail()) {
        return false;
    }

    FileIn.get(Input);

    while (!FileIn.eof()) {
        if (Input == 'v') {
            FileIn.get(Input);

            if (Input == ' ') {
                FileIn >> Vertices[VertexIndex].X >> Vertices[VertexIndex].Y >> Vertices[VertexIndex].Z;

                // Enable invert vertices if importing from right-hand system
                Vertices[VertexIndex].Z = Vertices[VertexIndex].Z * -1.f;
                VertexIndex++;
            }

            if (Input == 't') {
                FileIn >> TexCoords[TexCoordIndex].X >> TexCoords[TexCoordIndex].Y;

                // Enable invert vertices if importing from right-hand system
                TexCoords[TexCoordIndex].Y = 1.f - TexCoords[TexCoordIndex].Y;
                TexCoordIndex++;
            }

            if (Input == 'n') {
                FileIn >> Normals[NormalIndex].X >> Normals[NormalIndex].Y >> Normals[NormalIndex].Z;

                // Enable invert vertices if importing them from right-hand system
                Normals[NormalIndex].Z = Normals[NormalIndex].Z * -1.f;
                NormalIndex++;
            }
        }

        if (Input == 'f') {
            FileIn.get(Input);

            if (Input == ' ') {
                // Read in the face data in reverse to convert it to left-hand system
                FileIn  >> Faces[FaceIndex].VIndex3 >> Input2 >> Faces[FaceIndex].TIndex3 >> Input2 >> Faces[FaceIndex].NIndex3
                        >> Faces[FaceIndex].VIndex2 >> Input2 >> Faces[FaceIndex].TIndex2 >> Input2 >> Faces[FaceIndex].NIndex2
                        >> Faces[FaceIndex].VIndex1 >> Input2 >> Faces[FaceIndex].TIndex1 >> Input2 >> Faces[FaceIndex].NIndex1;
                
                FaceIndex++;
            }
        }

        while (Input != '\n') {
            FileIn.get(Input);
        }
        FileIn.get(Input);
    }

    FileIn.close();

    FileOut.open("model.txt");

    FileOut << "Vertex Count: " << (FaceCount * 3) << endl << endl;
    FileOut << "Data: " << endl << endl;

    for (int i = 0; i < FaceIndex; i++) {
        VIndex = Faces[i].VIndex1 - 1;
        TIndex = Faces[i].TIndex1 - 1;
        NIndex = Faces[i].NIndex1 - 1;

        FileOut << Vertices[VIndex].X << ' ' << Vertices[VIndex].Y << ' ' << Vertices[VIndex].Z << ' '
                << TexCoords[TIndex].X << ' ' << TexCoords[TIndex].Y << ' '
                << Normals[NIndex].X << ' ' << Normals[NIndex].Y << ' ' << Normals[NIndex].Z << endl;

        VIndex = Faces[i].VIndex2 - 1;
        TIndex = Faces[i].TIndex2 - 1;
        NIndex = Faces[i].NIndex2 - 1;

        FileOut << Vertices[VIndex].X << ' ' << Vertices[VIndex].Y << ' ' << Vertices[VIndex].Z << ' '
            << TexCoords[TIndex].X << ' ' << TexCoords[TIndex].Y << ' '
            << Normals[NIndex].X << ' ' << Normals[NIndex].Y << ' ' << Normals[NIndex].Z << endl;

        VIndex = Faces[i].VIndex3 - 1;
        TIndex = Faces[i].TIndex3 - 1;
        NIndex = Faces[i].NIndex3 - 1;

        FileOut << Vertices[VIndex].X << ' ' << Vertices[VIndex].Y << ' ' << Vertices[VIndex].Z << ' '
            << TexCoords[TIndex].X << ' ' << TexCoords[TIndex].Y << ' '
            << Normals[NIndex].X << ' ' << Normals[NIndex].Y << ' ' << Normals[NIndex].Z << endl;
    }

    FileOut.close();

    if (Vertices) {
        delete[] Vertices;
        Vertices = nullptr;
    }

    if (TexCoords) {
        delete[] TexCoords;
        TexCoords = nullptr;
    }

    if (Normals) {
        delete[] Normals;
        Normals = nullptr;
    }

    if (Faces) {
        delete[] Faces;
        Faces = nullptr;
    }
    return true;
}