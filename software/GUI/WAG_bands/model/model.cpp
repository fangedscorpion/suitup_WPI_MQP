#include "model.h"
#include "band/suit.h"
#include <stdexcept>

Model::Model(QVector<QSharedPointer<Node> > nodes) : QObject(){
    this->nodes = nodes;
    for (int i = 0; i < nodes.size(); ++i){
        namesAndStates.insert(nodes[i]->getName(),nodes[i]->getState());
    }
    for (int i = 0; i < nodes.size(); ++i){
        if (nodes[i]->isRoot()){
            rootNode = nodes[i];
            break;
        }
    }
    if (!rootNode)
        throw std::exception();
}

void Model::updatePose(PositionSnapshot *pose){
    bool somethingChanged = false;
    QHash<BandType, AbsState*> map = pose->getSnapshot();
    QList<BandType> bands = map.keys();
    for (int i = 0; i < bands.size(); ++i){
        QString bandName = AbsBand::bandTypeToModelName(bands[i]);
        AbsState* state = map[bands[i]];
        switch (state->getStateRep()) {
        case QUATERNION:
            somethingChanged = true;
            getNodeByName(bandName)->setWorldRotation(*(static_cast<QuatState*>(state)));
//            qDebug() << bandName;
//            qDebug() << (static_cast<QuatState*>(state))->length();
            break;
        default:
            qDebug() << "Model::updatePose(): Don't know how to handle given state representation!";
        }
    }
    if (somethingChanged)
        updateNamesAndStates();
}

void Model::updatePoseWithTime(qint32, PositionSnapshot *pose){
    updatePose(pose);
}

void Model::updateNodeStatus(QHash<BandType, NodeStatus> statuses){
    QList<BandType> bands = statuses.keys();
    for (int i = 0; i < bands.size(); ++i){
        QString name = AbsBand::bandTypeToModelName(bands[i]);
        getNodeByName(name)->setStatus(statuses[bands[i]]);
    }
    updateNamesAndStates();
}

QSharedPointer<Node> Model::getNodeByName(QString name) const {
    for (int i = 0; i < nodes.size(); ++i){
        if (nodes[i]->getName() == name)
            return nodes[i];
    }
    throw std::invalid_argument("node with given name not found");
}

void Model::resetPose(){
    rootNode->setAllRotDefault();
    updateNamesAndStates();
}

void Model::updateNamesAndStates(){
    for (int i = 0; i < nodes.size(); ++i){
        namesAndStates[nodes[i]->getName()] = nodes[i]->getState();
    }
    emit poseUpdated(namesAndStates);
}

void Node::setParent(QSharedPointer<Node> parent) {
    this->parent = parent;
    root(false);
}

void Node::init() {
    Node n = *this;
    QQuaternion worldToThis;
    QQuaternion rotationFromParent = QQuaternion::fromRotationMatrix(n.frame.toRotationMatrix());

    status = NODE_DISCONNECTED;

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
    // need to fix
    calibration = sensedOrientation.conjugated() * worldRotation;
}

NodeState Node::getState() const {
    NodeState n;
    n.status = status;
    n.transformation = transformation;
    return n;
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
        transformation = defaultPose;
        transformation.translate(-1000*tail); // TODO fix...
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
