#include "Mesh.h"

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
    glDisableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    //unbind buffers 
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //number of tris that the mesh has.
    triCount = 2;

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
        glDrawArrays(GL_TRIANGLES, 0, 0 * triCount);
    }
}
