#ifndef MODEL_H
#define MODEL_H

#include <QSharedPointer>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <stdexcept>

struct MaterialInfo {
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
    QSharedPointer<MaterialInfo> material;
};

class CoordinateFrame {
public:
    CoordinateFrame() {}
    CoordinateFrame(QVector3D x, QVector3D y, QVector3D z) : xv(x), yv(y), zv(z){}
    QVector3D x() const {return xv;}
    QVector3D y() const {return yv;}
    QVector3D z() const {return zv;}

    QMatrix3x3 toRotationMatrix() const;

//    QQuaternion rotationTo(CoordinateFrame other);
private:
    QVector3D xv;
    QVector3D yv;
    QVector3D zv;
};

class Node {
public:
    Node() {}

    // setters
    void setName(QString name) {this->name = name;}
    void setTransformation(QMatrix4x4 transformation) {this->transformation = transformation;}
    void setTail(QVector3D tail) {this->tail = tail;}
    void setHead(QVector3D head) {this->head = head;}
    void setFrame(CoordinateFrame frame) {this->frame = frame;}
    void setParent(QSharedPointer<Node> parent);
    void addMesh(QSharedPointer<Mesh> mesh) {this->meshes.push_back(mesh);}
    void root(bool isRoot) {this->isRootNode = isRoot;}
    void addChild(QSharedPointer<Node> child) {children.push_back(child);}

    // getters
    QString getName() const {return name;}
    QMatrix4x4 getTransformation() const {return transformation;}
    QVector<QSharedPointer<Mesh> > getMeshes() const {return meshes;}
    QMatrix4x4 getDefaultPose() const {return defaultPose;}
    QQuaternion getWorldRotation() const {return worldRotation;}
    bool isRoot() const {return isRootNode;}

    // other
    void init();
    void findRotationFromParent();
    void setWorldRotation(QQuaternion worldRotation);
    void setAllRotIdentity();

private:
    QString name;
    QMatrix4x4 transformationToOrigin;
    QQuaternion rotToOrigin;
    QMatrix4x4 defaultPose;
    QMatrix4x4 transformation;
    QVector3D tail;
    QVector3D head;
    CoordinateFrame frame;
    bool isRootNode;

    QVector3D rotHead;
    QVector3D rotTail;
    QQuaternion worldRotation;

    QSharedPointer<Node> parent;
    QVector<QSharedPointer<Node> > children;

    QVector<QSharedPointer<Mesh> > meshes;
};

class Model {
public:
    Model() {}
    Model(QVector<QSharedPointer<Node> > nodes, QVector<QSharedPointer<MaterialInfo> > materials);
    QVector<QSharedPointer<Node> > getNodes() const {return nodes;}
    QSharedPointer<Node> getNodeByName(QString name) const;

    QVector<QSharedPointer<MaterialInfo> > getMaterials() const {return materials;}
    QSharedPointer<MaterialInfo> getMaterialByIndex(int index) const {return materials[index];}
    QSharedPointer<MaterialInfo> getMaterialByName(QString name) const;

private:
    QSharedPointer<Node> rootNode;
    QVector<QSharedPointer<Node> > nodes;
    QVector<QSharedPointer<MaterialInfo> > materials;
};

#endif // MODEL_H
