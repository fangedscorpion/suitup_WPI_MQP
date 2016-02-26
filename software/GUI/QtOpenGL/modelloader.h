#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>
#include <QMatrix4x4>
#include <vector>
#include <QFile>
#include <QSharedPointer>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

struct MaterialInfo {
    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;

    bool isTexture;
    QString textureName;
};

struct LightInfo {
    QVector4D Position;
    QVector3D Intensity;
};

struct Mesh {
    unsigned int indexCount;
    unsigned int indexOffset;
    QSharedPointer<MaterialInfo> material;
};

struct Node {
    QString name;

    QMatrix4x4 transformation;
    QVector3D tail;
    QVector3D head;

    QVector<QSharedPointer<Mesh> > meshes;
    QVector<Node> nodes;
};

class ModelLoader {
public:
    enum PathType {
        RelativePath,
        AbsolutePath
    };

    ModelLoader();
    bool Load(QString filePath, PathType pathType);
    void getBufferData(QVector<float> **vertices, QVector<float> **normals,
                       QVector<unsigned int> **indices);

    QSharedPointer<Node> getNodeData() { return m_rootNode; }
    QSharedPointer<MaterialInfo> getMaterial(int index) {return m_materials[index];}
    Node getNodeByName(QString name);

private:
    QSharedPointer<MaterialInfo> processMaterial(aiMaterial *mater);
    QSharedPointer<Mesh> processMesh(aiMesh *mesh);
    void processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode);
    QVector3D jsonArr3toQVec3(QJsonArray jsonArr3);

    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;

    QVector<QSharedPointer<MaterialInfo> > m_materials;
    QVector<QSharedPointer<Mesh> > m_meshes;
    QSharedPointer<Node> m_rootNode;

    QJsonObject pointsJson;
    QVector3D rootTail;
    QVector3D rootHead;
};

#endif // MODELLOADER_H
