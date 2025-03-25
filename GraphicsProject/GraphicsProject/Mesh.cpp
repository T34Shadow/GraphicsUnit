#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <vector>

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

    // bind vertex array aka a mesh wrapper
    glBindVertexArray(vertexArrayObject);

    // bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    // fill vertex buffer
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // enable first element as position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Initialise(const char* fileName)
{

    //read vertcies from the model
    const aiScene* scene = aiImportFile(fileName, 0);

    //just use the first mesh we find, for now.
    aiMesh* mesh = scene->mMeshes[0];

    //extract indicies from the first mesh
    int numFaces = mesh->mNumFaces;
    std::vector<unsigned int> indices;

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
    }
    Initialise(numV, vertices, indices.size(), indices.data());
    delete[] vertices;


}

void Mesh::Draw()
{
    glBindVertexArray(vertexArrayObject);
    if(indexBufferObject !=0)
    {
        glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
    }
}
