#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <QJsonObject>
#include <QVector3D>
#include "model.h"

class ModelLoader {
public:
    ModelLoader();
//    ~ModelLoader() {}

    Model* load();

private:
    QVector3D jsonArr3toQVec3(QJsonArray jsonArr3, QQuaternion rotation = QQuaternion());
    CoordinateFrame jsonXYZtoFrame(QJsonObject jsonFrame);
    static QSharedPointer<Node> getNodeByName(QVector<QSharedPointer<Node> > nodes, QString name);

    QQuaternion rotateBlenderToOpenGL;
    QJsonObject pointsJson;
};

#endif // MODELLOADER_H
