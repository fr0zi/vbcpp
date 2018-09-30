#ifndef RSTATICMODEL_H_INCLUDED
#define RSTATICMODEL_H_INCLUDED


#include "../Graphics/OGLDriver.h"
#include "../Graphics/Vertex.h"
#include "../Graphics/Material.h"
#include "../Graphics/VBO.h"
#include "../Graphics/IBO.h"
#include "../Graphics/AABB.h"


struct StaticModelMesh
{
    Vertex*         vertices;
    unsigned int    verticesCount;

    unsigned int*   indices;
    unsigned int    indicesCount;

    VBO* vbo;
    IBO* ibo;

    unsigned int firstVertex;
    unsigned int firstVertexInVbo;
    Material material;

    ~StaticModelMesh()
    {
        if (vertices)
        {
            delete[] vertices;
        }

        if (indices)
        {
            delete[] indices;
        }

        if (vbo)
        {
            //OGLDriver::getInstance().deleteVBO(vbo);
        }

        if (ibo)
        {
            //OGLDriver::getInstance().deleteIBO(ibo);
        }

    }

    void setMeshData(Vertex* vertices, unsigned int verticesCount, unsigned int* indices, unsigned int indicesCount,
                     Material material)
    {
        this->vertices = vertices;
        this->verticesCount = verticesCount;
        this->indices = indices;
        this->indicesCount = indicesCount;
        this->firstVertex = 0;
        this->material = material;

        int vboIndex = OGLDriver::getInstance().getVboIndexForVertices(material.shader, verticesCount, sizeof(vertices[0]));
        int iboIndex = OGLDriver::getInstance().getIboIndexForIndices(material.shader, indicesCount);
        vbo = OGLDriver::getInstance().vbos[material.shader][vboIndex];// createVBO(verticesCount * sizeof(Vertex));
        ibo = OGLDriver::getInstance().ibos[material.shader][iboIndex];//createIBO(indicesCount * sizeof(unsigned int));

        firstVertexInVbo = vbo->getQuantumOfVertices();
        for (unsigned int i = 0; i < this->indicesCount; ++i)
        {
            this->indices[i] += firstVertexInVbo;
        }

        this->firstVertex = ibo->getIndicesCount();

        vbo->addVertexData(vertices, verticesCount);
        ibo->addIndices(indices, indicesCount);
    }

};


class RStaticModel : public Resource
{
    private:
        StaticModelMesh*_meshes;
        unsigned int    _meshesCount;

        glm::vec3*      _collisionMesh;
        unsigned int    _collisionMeshSize;

        GLenum          _primitiveType;

        AABB _aabb;
        VBO* _aabbVbo;
        IBO* _aabbIbo;

        void calculateAABB();
        void createAabbVbo();

    public:
        RStaticModel(string path, StaticModelMesh* meshes, unsigned int meshesCount, GLenum primitiveType = GL_TRIANGLES,
                     glm::vec3* collisionMesh = NULL, unsigned int collisionMeshSize = 0);
        RStaticModel()
            : Resource(RT_MODEL, "")
        {
            _meshesCount = 0;
            _collisionMeshSize = 0;
        }
        ~RStaticModel();

        unsigned int    getMeshesCount();
        StaticModelMesh*getMesh(unsigned int i);
        unsigned int    getCollisionMeshSize();
        glm::vec3*      getCollisionMesh();

        GLenum getPrimitiveType() {return _primitiveType;}

        AABB* getAABB();
        VBO* getAabbVbo();
        IBO* getAabbIbo();

};



#endif // RSTATICMODEL_H_INCLUDED