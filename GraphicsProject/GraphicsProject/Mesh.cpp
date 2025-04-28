#include "Mesh.h"

#include "ShaderProgram.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>
#include <fstream>
#include <sstream>

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &indexBufferObject);
}

void Mesh::InitialiseQuad()
{
    //chech that the mesh is not initialise already 
    assert(vertexArrayObject == 0);

    //make buffers 
    glGenBuffers(1, &vertexBufferObject);
    glGenVertexArrays(1, &vertexArrayObject);

    //binding vertex array // a mesh wrapper 
    glBindVertexArray(vertexArrayObject);

    //bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    //define 6 vertices for 2 triangles
    Vertex vertices[6];
    vertices[0].pos = { -0.5f,0,0.5f,1 };
    vertices[1].pos = { 0.5f,0,0.5f,1 };
    vertices[2].pos = { -0.5f,0,-0.5f,1 };

    vertices[3].pos = { -0.5f,0,-0.5f,1 };
    vertices[4].pos = { 0.5f,0,0.5f,1 };
    vertices[5].pos = { 0.5f,0,-0.5f,1 };

    vertices[0].normal = { 0,1,0,0 };
    vertices[1].normal = { 0,1,0,0 };
    vertices[2].normal = { 0,1,0,0 };
    vertices[3].normal = { 0,1,0,0 };
    vertices[4].normal = { 0,1,0,0 };
    vertices[5].normal = { 0,1,0,0 };

    //fill vertex buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    //enable first element as postion 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    //unbind buffers 
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //number of tris that the mesh has.
    triCount = 2;

}

void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
    assert(vertexArrayObject == 0);

    // generate buffers
    glGenBuffers(1, &vertexBufferObject);
    glGenVertexArrays(1, &vertexArrayObject);

    // bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    // fill vertex buffer
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    
    // bind vertex array aka a mesh wrapper
    glBindVertexArray(vertexArrayObject);

    // enable first element as position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    //enable second element as normal 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    //bind indices if there are any 
    if (indexCount != 0)
    {
        glGenBuffers(1, &indexBufferObject);

        //bind vertex buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

        //fill vertex buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        triCount = indexCount / 3;
    }
    else
    {
        triCount = vertexCount / 3;
    }

    //unbind buffers 
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitialiseFromFile(std::string fileName)
{
    Assimp::Importer importer;

    //read vertcies from the model
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);    
    
    if (scene == nullptr)
    {
        std::cout << "There is no scene, we might not have any objects being loaded" << std::endl;
    }
    if (scene != nullptr)
    {
        std::cout << "Scene is loading for object: " << fileName << std::endl;

        if (scene->mNumMeshes > 0)
        {
            //just use the first mesh we find, for now.
            aiMesh* mesh = scene->mMeshes[0];

            //extract indicies from the first mesh
            int numFaces = mesh->mNumFaces;
            std::vector<unsigned int> indices;
            int indexCount = mesh->mNumFaces * 3;

            std::cout << "Vertex count: " << mesh->mNumVertices << std::endl;
            std::cout << "Face count: " << mesh->mNumFaces << std::endl;
            std::cout << "Index count: " << indexCount << std::endl;

            for (int i = 0; i < numFaces; i++)
            {
                indices.push_back(mesh->mFaces[i].mIndices[0]);
                indices.push_back(mesh->mFaces[i].mIndices[2]);
                indices.push_back(mesh->mFaces[i].mIndices[1]);

                //generate a second triangle for quads 
                if (mesh->mFaces[i].mNumIndices == 4)
                {
                    indices.push_back(mesh->mFaces[i].mIndices[0]);
                    indices.push_back(mesh->mFaces[i].mIndices[3]);
                    indices.push_back(mesh->mFaces[i].mIndices[2]);
                }
            }

            //extract vertex data
            int numV = mesh->mNumVertices;
            Vertex* vertices = new Vertex[numV];

            for (int i = 0; i < numV; i++)
            {
                vertices[i].pos = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);
                vertices[i].normal = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0);
                //TODO: IF(HASUVS()) GET UVS OTHERWISE VEC2(0,0)
                if (mesh->mTextureCoords[0])
                {
                    vertices[i].uv = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
                }
                else
                {
                    vertices[i].uv = glm::vec2(0);
                }
            }
            Initialise(numV, vertices, indices.size(), indices.data());
            delete[] vertices;
        }
    }
}

void Mesh::ApplyMat(ShaderProgram* shader)
{
    shader->SetUniform("Ka", Ka);
    shader->SetUniform("Kd", Kd);
    shader->SetUniform("Ks", Ks);
    shader->SetUniform("specularPower", specularPower);
}

void Mesh::LoadMat(const char* fileName)
{
    std::fstream file(fileName, std::ios::in);
    std::string line;
    std::string header;
    char buffer[256];
    while (!file.eof())
    {
        file.getline(buffer, 256);
        line = buffer;
        std::stringstream ss(line, std::stringstream::in | std::stringstream::out);

        if (line.find("Ka") == 0)
        {
            ss >> header >> Ka.x >> Ka.y >> Ka.z;
        }
        else if (line.find("Ks") == 0)
        {
            ss >> header >> Ks.x >> Ks.y >> Ks.z;
        }
        else if (line.find("Kd") == 0)
        {
            ss >> header >> Kd.x >> Kd.y >> Kd.z;
        }
        else if (line.find("Ns") == 0)
        {
            ss >> header >> specularPower;
        }
    }
}

void Mesh::Equip()
{
    glBindVertexArray(vertexArrayObject);
}

void Mesh::Unequip()
{
    glBindVertexArray(0);
}

void Mesh::Draw()
{
    if(indexBufferObject !=0)
    {
        glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, triCount);
    }
}
