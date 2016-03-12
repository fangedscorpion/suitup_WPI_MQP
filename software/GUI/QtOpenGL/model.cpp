#include "model.h"
#include <stdexcept>

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

QSharedPointer<Node> Node::getParent() const {
    if (!isRootNode)
        return parent;
    else
        throw std::exception();
}

void Node::setParent(QSharedPointer<Node> parent) {
    this->parent = parent;
    root(false);
    findRotationFromParent();
}

void Node::findRotationFromParent() {
    rotationFromParent = QQuaternion::fromRotationMatrix(frame.toRotationMatrix());
}

void Node::init(){
    Node n = *this;
    QQuaternion worldToThis;

    while (!n.isRoot()) {
        worldToThis = n.rotationFromParent * worldToThis;
        n = *(n.parent);
    }
    worldToThis =  n.rotationFromParent * worldToThis;

    // TODO FIXXXXXXXXXXXXX
    float test[9] = {1,0,0,
                      0,0,-1,
                      0,1,0};

    rotToOrigin = QQuaternion();
    rotToOrigin = rotToOrigin * QQuaternion::fromRotationMatrix(QMatrix3x3(test)) * worldToThis.conjugated();
    transformationToOrigin.rotate(rotToOrigin);
    transformationToOrigin.translate(-head);
    defaultPose = transformationToOrigin.inverted();
//    transformation = transformationToOrigin;
}

void Node::setWorldRotation(QQuaternion worldRotation) {
    this->worldRotation = worldRotation;
    rotHead = parent->rotTail + parent->worldRotation.rotatedVector(parent->rotToOrigin.rotatedVector(head - parent->tail));
    rotTail = rotHead + worldRotation.rotatedVector(rotToOrigin.rotatedVector(tail - head));

    transformation.setToIdentity();
    transformation.translate(rotHead);
    transformation.rotate(worldRotation);

    transformation *= transformationToOrigin;
}

void Node::setAllRotIdentity() {
    for (int i = 0; i < children.size(); ++i){
        children[i]->setWorldRotation(QQuaternion(1,0,0,0));
        children[i]->setAllRotIdentity();
    }
    if (name == "Mid"){
        transformation.translate(-1000,-1000,-1000);
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
