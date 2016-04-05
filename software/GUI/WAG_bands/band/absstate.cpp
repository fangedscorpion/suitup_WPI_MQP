#include "absstate.h"


void AbsState::serialize(QDataStream *ds) {
    qint32 serializeType = stateRep;
    (*ds)<<serializeType;
    float scalarVar = 0;
    float xVar = 0;
    float yVar = 0;
    float zVar = 0;

    switch(stateRep) {
    case QUAT_REP:

        //        scalarVar = scalar();
        //        xVar = QuatState::x();
        //        yVar = QuatState::y();
        //        zVar = QuatState::z();
        scalarVar = 65;
        xVar = 66;
        yVar = 67;
        zVar = 68;


        (*ds)<<scalarVar;
        (*ds)<<xVar;
        (*ds)<<yVar;
        (*ds)<<zVar;

        qDebug()<<"Absstate vars not quat";
        qDebug()<<scalarVar;
        qDebug()<<xVar;
        qDebug()<<yVar;
        qDebug()<<zVar;
    }

}

void QuatState::serialize(QDataStream *ds) {
    qDebug()<<"SERIALIZING IN QUAT STATE ------------------------";
    qint32 serializeType = stateRep;
    (*ds)<<serializeType;

    float scalarVar = 0;
    float xVar = 0;
    float yVar = 0;
    float zVar = 0;

    scalarVar = scalar();
    xVar = x();
    yVar = y();
    zVar = z();


    (*ds)<<scalarVar;
    (*ds)<<xVar;
    (*ds)<<yVar;
    (*ds)<<zVar;

    /* float scalarVar = scalar();
    float xVar = x();
    float yVar = y();
    float zVar = z();

    (*ds)<<scalarVar;
    (*ds)<<xVar;
    (*ds)<<yVar;
    (*ds)<<zVar;

    qDebug()<<"Absstate vars";
    qDebug()<<scalarVar;
    qDebug()<<xVar;
    qDebug()<<yVar;
    qDebug()<<zVar; */

}

AbsState * AbsState::deserialize(QDataStream *ds) {
    static int iter = 0;
    qDebug("Deserializing absState");
    AbsState *state;
    qint32 dataType = 0;
    (*ds)>>dataType;
    qDebug()<<"rep = "<<dataType;
    qDebug()<<"Quat rep"<<QUAT_REP;
    PositionRepresentation actualRep = (PositionRepresentation) dataType;

    float scalarVar = 0;
    float xVar = 0;
    float yVar = 0;
    float zVar = 0;

    switch (actualRep) {
    case QUAT_REP:


        (*ds)>>scalarVar;
        (*ds)>>xVar;
        (*ds)>>yVar;
        (*ds)>>zVar;
        qDebug()<<"Absstate vars deserialize"<<iter;
        iter++;
        qDebug()<<scalarVar;
        qDebug()<<xVar;
        qDebug()<<yVar;
        qDebug()<<zVar;

        state = new QuatState(scalarVar, xVar, yVar, zVar);
        break;
    default:
        state = new AbsState((PositionRepresentation) (QUAT_REP + 1));
        break;
    }
    qDebug("Done deserializing absstate");
    return state;
}

