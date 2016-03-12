#include "model.h"
#include <stdexcept>

Model::Model(QVector<QSharedPointer<Node> > nodes, QVector<QSharedPointer<MaterialInfo> > materials):
    nodes(nodes),
    materials(materials){
    for (int i = 0; i < nodes.size(); ++i){
        if (nodes[i]->isRoot()){
            rootNode = nodes[i];
            break;
        }
    }
    if (!rootNode)
        throw std::exception();
}

QSharedPointer<Node> Model::getNodeByName(QString name) const {
    for (int i = 0; i < nodes.size(); ++i){
        if (nodes[i]->getName() == name){
            return nodes[i];
        }
    }
    throw std::invalid_argument("Node with given name not found.");
}

QSharedPointer<MaterialInfo> Model::getMaterialByName(QString name) const {
    for (int i = 0; i < materials.size(); ++i){
        if (materials[i]->Name == name){
            return materials[i];
        }
    }
    throw std::invalid_argument("Material with given name not found.");
}

void Node::setParent(QSharedPointer<Node> parent) {
    this->parent = parent;
    root(false);
}

void Node::init() {
    Node n = *this;
    QQuaternion worldToThis;
    QQuaternion rotationFromParent = QQuaternion::fromRotationMatrix(n.frame.toRotationMatrix());

    while (!n.isRoot()) {
        worldToThis = rotationFromParent * worldToThis;
        n = *(n.parent);
        rotationFromParent = QQuaternion::fromRotationMatrix(n.frame.toRotationMatrix());
    }
    worldToThis =  rotationFromParent * worldToThis;

    // TODO FIXXXXXXXXXXXXX
    float test[9] = {1,0,0,
                      0,0,-1,
                      0,1,0};

    rotToOrigin = QQuaternion::fromRotationMatrix(QMatrix3x3(test)) * worldToThis.conjugated();
    transformationToOrigin.rotate(rotToOrigin);
    transformationToOrigin.translate(-head);
    defaultPose = transformationToOrigin.inverted();
}

void Node::calibrate(QQuaternion sensedOrientation) {
    calibration = sensedOrientation.conjugated() * worldRotation;
}

void Node::setWorldRotation(QQuaternion worldRotation) {
    this->worldRotation = worldRotation*calibration;
    rotHead = parent->rotTail + parent->worldRotation.rotatedVector(parent->rotToOrigin.rotatedVector(head - parent->tail));
    rotTail = rotHead + this->worldRotation.rotatedVector(rotToOrigin.rotatedVector(tail - head));

    transformation.setToIdentity();
    transformation.translate(rotHead);
    transformation.rotate(this->worldRotation);

    transformation *= transformationToOrigin;

    for (int i = 0; i < children.size(); ++i){
        children[i]->setWorldRotation(children[i]->getWorldRotation());
    }
}

void Node::setAllRotIdentity() {
    if (isRootNode){
        transformation.translate(-tail);
    }
    for (int i = 0; i < children.size(); ++i){
        children[i]->setWorldRotation(QQuaternion(1,0,0,0));
        children[i]->setAllRotIdentity();
    }
}

void Node::setAllRotDefault() {
    if (isRootNode){
        transformation.translate(-tail);
    }
    for (int i = 0; i < children.size(); ++i){
        children[i]->setWorldRotation(children[i]->rotToOrigin.conjugated());
        children[i]->setAllRotDefault();
    }
}

QMatrix3x3 CoordinateFrame::toRotationMatrix() const {
    float vectors[9] = {xv.x(), yv.x(), zv.x(),
                        xv.y(), yv.y(), zv.y(),
                        xv.z(), yv.z(), zv.z()};
    // TODO FIX THIS LOL
    // (comes from blender and opengl having different frame orientations - see jsonArrToQVec3 in modelloader.cpp)
    float test[9] = {1,0,0,
                     0,0,-1,
                     0,1,0};
    return QMatrix3x3(vectors)*QMatrix3x3(test);
}
