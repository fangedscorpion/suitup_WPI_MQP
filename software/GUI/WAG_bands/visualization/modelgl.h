#ifndef MODELGL_H
#define MODELGL_H

#include <QVector>
#include <QSharedPointer>
#include <QMatrix4x4>

#include "model/model.h"

struct Material {
    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;
};

struct LightInfo {
    QVector4D Position;
    QVector3D Intensity;
};

struct Mesh {
    unsigned int indexCount;
    unsigned int indexOffset;
    QSharedPointer<Material> material;
};

class NodeGL {
public:
    NodeGL(){}
    ~NodeGL(){}

    QVector<QSharedPointer<Mesh> > getMeshes() const {return meshes;}
    void addMesh(QSharedPointer<Mesh> mesh) {this->meshes.push_back(mesh);}
    QMatrix4x4 getTransformation() {return transformation;}
    QString getName() const {return name;}
    void setName(QString name) {this->name = name;}
    void setMaterial(QSharedPointer<Material> material) {this->material = material;}
    void setTransformation(QMatrix4x4 transformation) {this->transformation = transformation;}
    QSharedPointer<Material> getMaterial() {return material;}

private:
    QString name;
    QVector<QSharedPointer<Mesh> > meshes;
    QSharedPointer<Material> material;
    QMatrix4x4 transformation;
};


class ModelGL : public QObject {
    Q_OBJECT
public:
    ModelGL() : QObject() {}
    ModelGL(QVector<QSharedPointer<NodeGL> > nodes, QVector<QSharedPointer<Material> > materials) : QObject(), nodes(nodes), materials(materials) {}
    ~ModelGL(){}

    QVector<QSharedPointer<NodeGL> > getNodes() const {return nodes;}
    QVector<QSharedPointer<Material> > getMaterials() const {return materials;}

    QSharedPointer<NodeGL> getNodeByName(QString name) const;
    QSharedPointer<Material> getMaterialByName(QString name) const;

    QSharedPointer<Material> nodeStatusToMaterial(NodeStatus status);

public slots:
    void updatePose(QHash<QString, NodeState> bandsAndTransforms);

signals:
    void modelGLChanged();

private:
    QVector<QSharedPointer<NodeGL> > nodes;
    QVector<QSharedPointer<Material> > materials;
};

#endif // MODELGL_H
