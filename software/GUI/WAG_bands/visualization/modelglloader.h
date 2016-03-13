#ifndef MODELGLLOADER_H
#define MODELGLLOADER_H

#include <string>
#include <QMatrix4x4>
#include <vector>
#include <QFile>
#include <QSharedPointer>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include "modelgl.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

class ModelGLLoader {
public:
    enum PathType {
        RelativePath,
        AbsolutePath
    };

    ModelGLLoader();
    ~ModelGLLoader() {}
    bool Load(QString filePath, PathType pathType);
    void getBufferData(QVector<float> **vertices, QVector<float> **normals,
                       QVector<unsigned int> **indices);

    QSharedPointer<NodeGL> getNodeData(int index) { return m_nodes[index];}
    QVector<QSharedPointer<NodeGL> > getAllNodes() {return m_nodes;}
    QSharedPointer<Material> getMaterial(int index) {return m_materials[index];}
    QSharedPointer<NodeGL> getNodeByName(QString name);

    ModelGL toModel();

private:
    QSharedPointer<Material> processMaterial(aiMaterial *mater);
    QSharedPointer<Mesh> processMesh(aiMesh *mesh);
    void processNode(aiNode *node);

    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;

    QVector<QSharedPointer<Material> > m_materials;
    QVector<QSharedPointer<Mesh> > m_meshes;
    QVector<QSharedPointer<NodeGL> > m_nodes;
};

#endif // MODELGLLOADER_H
