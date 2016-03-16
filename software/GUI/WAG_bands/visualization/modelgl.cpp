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

void ModelGL::updatePose(QHash<QString,NodeState> bandsAndTransforms){
    QList<QString> bands = bandsAndTransforms.keys();
    for (int i = 0; i < bands.length(); ++i){
        getNodeByName(bands[i])->setTransformation(bandsAndTransforms[bands[i]].transformation);
        getNodeByName(bands[i])->setMaterial(nodeStatusToMaterial(bandsAndTransforms[bands[i]].status));
    }
    qDebug()<< "ModelGL::updatePose: received change to nodes";
    emit modelGLChanged();
}

QSharedPointer<Material> ModelGL::nodeStatusToMaterial(NodeStatus status){
    switch (status){
    case NODE_CONNECTED:
        return getMaterialByName("PurpleHoser");
        break;
    case NODE_DISCONNECTED:
        return getMaterialByName("DefaultMaterial");
        break;
    case NODE_UNUSED:
        return getMaterialByName("LightBlue");
        break;
    default:
        return getMaterialByName("DefaultMaterial");
        break;
    }
}
