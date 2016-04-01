#include "modelloader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

ModelLoader::ModelLoader(){
    QFile file("../biped/final/pts.out");
    file.open(QFile::ReadOnly);
    QByteArray blob = file.readAll();
    pointsJson = QJsonDocument::fromJson(blob).object();

    float blend2openglarr[9] = {1,0,0,0,0,1,0,-1,0};
    rotateBlenderToOpenGL = QQuaternion::fromRotationMatrix(QMatrix3x3(blend2openglarr));
}

Model *ModelLoader::load() {
    QVector<QSharedPointer<Node> > m_nodes;
    QStringList names = pointsJson.keys();

    for (int i = 0; i < names.length(); ++i){
        // this line has a memory leak...
        QSharedPointer<Node> n(new Node);
        n->setName(names[i]);
        n->setTail(jsonArr3toQVec3(pointsJson.value(names[i]).toObject().value("tail").toArray(),rotateBlenderToOpenGL));
        n->setHead(jsonArr3toQVec3(pointsJson.value(names[i]).toObject().value("head").toArray(),rotateBlenderToOpenGL));
        n->setFrame(jsonXYZtoFrame(pointsJson.value(names[i]).toObject().value("frame").toObject()));
        m_nodes.push_back(n);
    }

    QSharedPointer<Node> rootNode;
    // assign parents
    for (int i = 0; i < m_nodes.size(); ++i){
        QString parentName = pointsJson.value(m_nodes[i]->getName()).toObject().value("parent_name").toString();
        if (parentName == ""){
            rootNode = m_nodes[i];
            m_nodes[i]->root(true);
        }
        else {
            m_nodes[i]->setParent(ModelLoader::getNodeByName(m_nodes,parentName));
            ModelLoader::getNodeByName(m_nodes,parentName)->addChild(m_nodes[i]);
        }
    }
    for (int i = 0; i < m_nodes.size(); ++i){
        m_nodes[i]->init();
    }
    rootNode->setAllRotDefault();
    return new Model(m_nodes);
}

QSharedPointer<Node> ModelLoader::getNodeByName(QVector<QSharedPointer<Node> > nodes, QString name){
    for (int i = 0; i < nodes.size(); ++i){
        if (nodes[i]->getName() == name){
            return nodes[i];
        }
    }
    throw std::invalid_argument("name not found in given list of nodes");
}

QVector3D ModelLoader::jsonArr3toQVec3(QJsonArray jsonArr3, QQuaternion rotation){
    if (jsonArr3.size() != 3){
        throw std::invalid_argument("requires 3-element QJsonArray");
    }
    // convert from blender axes to opengl axes
    return rotation.rotatedVector(QVector3D(
                                    jsonArr3[0].toDouble(),
                                    jsonArr3[1].toDouble(),
                                    jsonArr3[2].toDouble()));
}

CoordinateFrame ModelLoader::jsonXYZtoFrame(QJsonObject jsonFrame){
    return CoordinateFrame(
                jsonArr3toQVec3(jsonFrame.value("x").toArray(),rotateBlenderToOpenGL),
                jsonArr3toQVec3(jsonFrame.value("y").toArray(),rotateBlenderToOpenGL),
                jsonArr3toQVec3(jsonFrame.value("z").toArray(),rotateBlenderToOpenGL));
}
