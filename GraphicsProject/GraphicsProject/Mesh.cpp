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

    // enable 1st element as position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    //enable 2nd element as normal 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    //enable 3rd element as UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    //enable 4th element as tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 40);

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
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);// | aiProcess_FlipUVs);
    
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

                //add tangent and bitangent here and to vertex specification -Finn
                
                //TODO: IF(HASUVS()) GET UVS OTHERWISE VEC2(0,0)
                if (mesh->mTextureCoords[0])
                {
                    vertices[i].uv = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
                }
                else
                {
                    vertices[i].uv = glm::vec2(0);
                }
                if (mesh->HasTangentsAndBitangents())
                {
                    vertices[i].tangent = glm::vec4(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1);
                    vertices[i].biTangent = glm::vec4(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z, 1);
                }
            }
            if (!mesh->HasTangentsAndBitangents())
            {
                CalTangents(vertices, numV, indices);
            }
            Initialise(numV, vertices, indices.size(), indices.data());
            delete[] vertices;
        }
    }
}

void Mesh::CalTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices)
{
    glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
    glm::vec4* tan2 = tan1 + vertexCount;
    memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);
    unsigned int indexCount = (unsigned int)indices.size();
    for (unsigned int a = 0; a < indexCount; a += 3) {
        long i1 = indices[a];
        long i2 = indices[a + 1];
        long i3 = indices[a + 2];
        const glm::vec4& v1 = vertices[i1].pos;
        const glm::vec4& v2 = vertices[i2].pos;
        const glm::vec4& v3 = vertices[i3].pos;
        const glm::vec2& w1 = vertices[i1].uv;
        const glm::vec2& w2 = vertices[i2].uv;
        const glm::vec2& w3 = vertices[i3].uv;
        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        float r = 1.0F / (s1 * t2 - s2 * t1);
        glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
            (t2 * z1 - t1 * z2) * r, 0);
        glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
            (s1 * z2 - s2 * z1) * r, 0);
        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }
    for (unsigned int a = 0; a < vertexCount; a++) {
        const glm::vec3& n = glm::vec3(vertices[a].normal);
        const glm::vec3& t = glm::vec3(tan1[a]);
        // Gram-Schmidt orthogonalize
        vertices[a].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n,
            t)), 0);
        // Calculate handedness (direction of bitangent)
        vertices[a].tangent.w = (glm::dot(glm::cross(glm::vec3(n),
            glm::vec3(t)), glm::vec3(tan2[a])) < 0.0F) ? 1.0F : -1.0F;
    }
    delete[] tan1;
}

void Mesh::ApplyMat(ShaderProgram* shader)
{
    shader->SetUniform("Ka", Ka);
    shader->SetUniform("Kd", Kd);
    shader->SetUniform("Ks", Ks);
    shader->SetUniform("specularPower", specularPower);
 
    mapKd.Bind(0);
    shader->SetBoolUniform("albedoMap", 0);
    mapNormal.Bind(1);
    shader->SetBoolUniform("normalMap", 1);
    mapKs.Bind(2);
    shader->SetBoolUniform("specularMap", 2);    
}

void Mesh::LoadMat(const char* fileName)
{
    std::fstream file(fileName, std::ios::in);
    std::string line;
    std::string header;
    char buffer[256];

    std::string directory(fileName);
    int index = directory.rfind('/');
    if (index != -1)
    {
        directory = directory.substr(0, index + 1);
    }

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
        else if (line.find("map_Kd") == 0)
        {
            std::string mapFileName;
            ss >> header >> mapFileName;
            mapKd.LoadFromFile((directory + mapFileName).c_str());
        }
        else if (line.find("map_Ks") == 0)
        {
            std::string mapFileName;
            ss >> header >> mapFileName;
            mapKs.LoadFromFile((directory + mapFileName).c_str());
        }
        else if (line.find("bump") == 0)
        {
            std::string mapFileName;
            ss >> header >> mapFileName;
            mapNormal.LoadFromFile((directory + mapFileName).c_str());
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
