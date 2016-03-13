#ifndef MODELGL_H
#define MODELGL_H

#include <QVector>
#include <QSharedPointer>
#include <QMatrix4x4>

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
    void setTransformation(QMatrix4x4 transformation) {this->transformation = transformation;}

private:
    QString name;
    QVector<QSharedPointer<Mesh> > meshes;
    QMatrix4x4 transformation;
};


class ModelGL {
public:
    ModelGL(){}
    ModelGL(QVector<QSharedPointer<NodeGL> > nodes, QVector<QSharedPointer<Material> > materials) : nodes(nodes), materials(materials) {}
    ~ModelGL(){}

    QVector<QSharedPointer<NodeGL> > getNodes() const {return nodes;}
    QVector<QSharedPointer<Material> > getMaterials() const {return materials;}

    QSharedPointer<NodeGL> getNodeByName(QString name) const;
    QSharedPointer<Material> getMaterialByName(QString name) const;
//    QSharedPointer<Material> getMaterialByIndex(int index) const;

private:
    QVector<QSharedPointer<NodeGL> > nodes;
    QVector<QSharedPointer<Material> > materials;
};

#endif // MODELGL_H
