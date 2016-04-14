#include "absstate.h"


void AbsState::serialize(QDataStream *ds) {
    qint32 serializeType = stateRep;
    (*ds)<<serializeType;
    // QuatStates should not be serialized here
    // WILL CAUSE ERRORS

}

void QuatState::serialize(QDataStream *ds) {
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
}

AbsState * AbsState::deserialize(QDataStream *ds) {
    AbsState *state;
    qint32 dataType = 0;
    (*ds)>>dataType;
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

        state = new QuatState(scalarVar, xVar, yVar, zVar);
        break;
    default:
        state = new AbsState((PositionRepresentation) (QUAT_REP + 1));
        break;
    }
    return state;
}

