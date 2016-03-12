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
#include "model.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

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

    QSharedPointer<Node> getNodeData(int index) { return m_nodes[index];}
    QVector<QSharedPointer<Node> > getAllNodes() {return m_nodes;}
    QSharedPointer<MaterialInfo> getMaterial(int index) {return m_materials[index];}
    QSharedPointer<Node> getNodeByName(QString name);

    Model toModel();

private:
    QSharedPointer<MaterialInfo> processMaterial(aiMaterial *mater);
    QSharedPointer<Mesh> processMesh(aiMesh *mesh);
    void processNode(aiNode *node);
    QVector3D jsonArr3toQVec3(QJsonArray jsonArr3);
    CoordinateFrame jsonXYZtoFrame(QJsonObject jsonFrame);

    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;

    QVector<QSharedPointer<MaterialInfo> > m_materials;
    QVector<QSharedPointer<Mesh> > m_meshes;
    QVector<QSharedPointer<Node> > m_nodes;

    QJsonObject pointsJson;
    QVector3D rootTail;
    QVector3D rootHead;
};

#endif // MODELLOADER_H
