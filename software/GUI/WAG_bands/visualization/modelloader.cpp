#include "modelloader.h"
#include <limits>
#include <QJsonDocument>
#include <QJsonArray>
#include <stdexcept>

#define DEBUGOUTPUT_NORMALS(nodeIndex) (false)//( QList<int>{1}.contains(nodeIndex) )//(false)

ModelLoader::ModelLoader() {
    QFile file("../biped/final/pts.out");
    file.open(QFile::ReadOnly);
    QByteArray blob = file.readAll();
    pointsJson = QJsonDocument::fromJson(blob).object();

    float blend2openglarr[9] = {1,0,0,0,0,1,0,-1,0};
    rotateBlenderToOpenGL = QQuaternion::fromRotationMatrix(QMatrix3x3(blend2openglarr));
}

// look for file using relative path
QString findFile(QString relativeFilePath, int scanDepth) {
    QString str = relativeFilePath;
    for(int ii=-1; ii<scanDepth; ++ii) {
        if(QFile::exists(str)) {
            return str;
        }
        str.prepend("../");
    }
    return "";
}

bool ModelLoader::Load(QString filePath, PathType pathType) {
    QString l_filePath;
    if (pathType == RelativePath)
        l_filePath = findFile(filePath, 5);
    else
        l_filePath = filePath;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(l_filePath.toStdString(),
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType |
            0);

    if(!scene) {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
        return false;
    }

    for(unsigned int ii=0; ii < scene->mNumMaterials; ++ii) {
        QSharedPointer<MaterialInfo> mater = processMaterial(scene->mMaterials[ii]);
        m_materials.push_back(mater);
    }

    for(unsigned int ii=0; ii < scene->mNumMeshes; ++ii) {
        m_meshes.push_back(processMesh(scene->mMeshes[ii]));
    }

    if(scene->mRootNode != NULL) {
        processNode(scene->mRootNode);
    }
    else {
        qDebug() << "Error loading model";
        return false;
    }

    return true;
}

void ModelLoader::getBufferData(QVector<float> **vertices, QVector<float> **normals, QVector<unsigned int> **indices) {
    if(vertices != 0)
        *vertices = &m_vertices;

    if(normals != 0)
        *normals = &m_normals;

    if(indices != 0)
        *indices = &m_indices;
}

QSharedPointer<MaterialInfo> ModelLoader::processMaterial(aiMaterial *material) {
    QSharedPointer<MaterialInfo> mater(new MaterialInfo);

    aiString mname;
    material->Get(AI_MATKEY_NAME, mname);

    int shadingModel;
    material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);

    if(shadingModel != aiShadingMode_Phong && shadingModel != aiShadingMode_Gouraud) {
        qDebug() << "This mesh's shading model is not implemented in this loader, setting to default material";
        mater->Name = "DefaultMaterial";
    }
    else {
        aiColor3D dif(0.f,0.f,0.f);
        aiColor3D amb(0.f,0.f,0.f);
        aiColor3D spec(0.f,0.f,0.f);
        float shine = 0.0;

        material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, dif); //->Get(<material-key>,<where-to-store>))
        material->Get(AI_MATKEY_COLOR_SPECULAR, spec);
        material->Get(AI_MATKEY_SHININESS, shine);

        mater->Ambient = QVector3D(amb.r, amb.g, amb.b);
        mater->Diffuse = QVector3D(dif.r, dif.g, dif.b);
        mater->Specular = QVector3D(spec.r, spec.g, spec.b);
        mater->Shininess = shine;

        mater->Ambient *= .2f;
        if(mater->Shininess == 0.0) mater->Shininess = 30;
    }

    return mater;
}

QSharedPointer<Mesh> ModelLoader::processMesh(aiMesh *mesh) {
    QSharedPointer<Mesh> newMesh(new Mesh);
    newMesh->indexOffset = m_indices.size();
    unsigned int indexCountBefore = m_indices.size();
    int vertindexoffset = m_vertices.size()/3;

    // Get Vertices
    for(uint ii=0; ii < mesh->mNumVertices; ++ii) {
        aiVector3D &vec = mesh->mVertices[ii];
        m_vertices.push_back(vec.x);
        m_vertices.push_back(vec.y);
        m_vertices.push_back(vec.z);
    }

    // Get Normals
    m_normals.resize(m_vertices.size());
    int nind = vertindexoffset * 3;

    for(uint ii=0; ii < mesh->mNumVertices; ++ii) {
        aiVector3D &vec = mesh->mNormals[ii];
        m_normals[nind] = vec.x;
        m_normals[nind+1] = vec.y;
        m_normals[nind+2] = vec.z;
        nind += 3;
    };

    // Get mesh indexes
    for(uint t = 0; t < mesh->mNumFaces; ++t) {
        aiFace* face = &mesh->mFaces[t];

        m_indices.push_back(face->mIndices[0]+vertindexoffset);
        m_indices.push_back(face->mIndices[1]+vertindexoffset);
        m_indices.push_back(face->mIndices[2]+vertindexoffset);
    }

    newMesh->indexCount = m_indices.size() - indexCountBefore;
    newMesh->material = m_materials.at(mesh->mMaterialIndex);

    return newMesh;
}

void ModelLoader::processNode(aiNode *node) {

    for(uint ich = 0; ich < node->mNumChildren; ++ich) {
        QSharedPointer<Node> n(new Node());

        aiNode* thisNode = node->mChildren[ich];
        n->setName(thisNode->mName.length != 0 ? thisNode->mName.C_Str() : "");
        n->setTail(jsonArr3toQVec3(pointsJson.value(n->getName()).toObject().value("tail").toArray(),rotateBlenderToOpenGL));
        n->setHead(jsonArr3toQVec3(pointsJson.value(n->getName()).toObject().value("head").toArray(),rotateBlenderToOpenGL));
        n->setFrame(jsonXYZtoFrame(pointsJson.value(n->getName()).toObject().value("frame").toObject()));
        n->setTransformation(QMatrix4x4(thisNode->mTransformation[0]));

        for(uint imesh = 0; imesh < thisNode->mNumMeshes; ++imesh) {
            QSharedPointer<Mesh> mesh = m_meshes[thisNode->mMeshes[imesh]];
            n->addMesh(mesh);
        }

        qDebug() << "NodeName" << n->getName();
//        qDebug() << "  NodeIndex" << ich;
//        for (int ii=0; ii < n->getMeshes().size(); ++ii) {
//            qDebug() << "    MaterialName" << n->getMeshes()[ii]->material->Name;
//            qDebug() << "    MeshVertices" << n->getMeshes()[ii]->indexCount;
//        }

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
            m_nodes[i]->setParent(getNodeByName(parentName));
            getNodeByName(parentName)->addChild(m_nodes[i]);
        }
    }
    for (int i = 0; i < m_nodes.size(); ++i){
        m_nodes[i]->init();
    }
    rootNode->setAllRotDefault();
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

QSharedPointer<Node> ModelLoader::getNodeByName(QString name){
    for (int i = 0; i < m_nodes.size(); i++){
        if (m_nodes[i]->getName() == name)
            return m_nodes[i];
    }
    throw std::invalid_argument("given name not found in node list");
}

Model ModelLoader::toModel(){
    return Model(m_nodes,m_materials);
}
