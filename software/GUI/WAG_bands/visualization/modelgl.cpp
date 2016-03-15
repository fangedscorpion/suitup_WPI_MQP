#include "modelgl.h"
#include <stdexcept>

QSharedPointer<NodeGL> ModelGL::getNodeByName(QString name) const {
    for (int i = 0; i < nodes.size(); ++i){
        if (nodes[i]->getName() == name){
            return nodes[i];
        }
    }
    throw std::invalid_argument("Node with given name not found.");
}

QSharedPointer<Material> ModelGL::getMaterialByName(QString name) const {
    for (int i = 0; i < materials.size(); ++i){
        if (materials[i]->Name == name){
            return materials[i];
        }
    }
//    throw std::invalid_argument("Material with given name not found.");
    return getMaterialByName("DefaultMaterial");
}

void ModelGL::updatePose(QString nodeName, QMatrix4x4 pose){
    getNodeByName(nodeName)->setTransformation(pose);
    qDebug()<< "ModelGL::updatePose: received change to node " << nodeName;
    emit modelGLChanged();
}
