#include "modelloader.h"
#include <limits>
#include <QJsonDocument>
#include <QJsonArray>

#define DEBUGOUTPUT_NORMALS(nodeIndex) (false)//( QList<int>{1}.contains(nodeIndex) )//(false)

ModelLoader::ModelLoader() {
    QFile file("../biped/final/pts.out");
    file.open(QFile::ReadOnly);
    QByteArray blob = file.readAll();
    pointsJson = QJsonDocument::fromJson(blob).object();

    QJsonArray headArr = pointsJson.value("Mid").toObject().value("tail").toArray();
    QJsonArray tailArr = pointsJson.value("Mid").toObject().value("head").toArray();
    rootTail = jsonArr3toQVec3(tailArr);
    rootHead = jsonArr3toQVec3(headArr);
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

    const aiScene* scene = importer.ReadFile( l_filePath.toStdString(),
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType |
            0
                                              );

    if(!scene) {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
        return false;
    }

    for(unsigned int ii=0; ii<scene->mNumMaterials; ++ii) {
        QSharedPointer<MaterialInfo> mater = processMaterial(scene->mMaterials[ii]);
        m_materials.push_back(mater);
    }

    for(unsigned int ii=0; ii<scene->mNumMeshes; ++ii) {
        m_meshes.push_back(processMesh(scene->mMeshes[ii]));
    }

    if(scene->mRootNode != NULL) {
        Node *rootNode = new Node;
        processNode(scene, scene->mRootNode, 0, *rootNode);
        m_rootNode.reset(rootNode);
    }
    else {
        qDebug() << "Error loading model";
        return false;
    }

    return true;
}

void ModelLoader::getBufferData( QVector<float> **vertices, QVector<float> **normals, QVector<unsigned int> **indices) {
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
    material->Get( AI_MATKEY_NAME, mname);
    if(mname.length > 0)
        mater->Name = mname.C_Str();
    mater->isTexture = false;

    int shadingModel;
    material->Get( AI_MATKEY_SHADING_MODEL, shadingModel );

    if(shadingModel != aiShadingMode_Phong && shadingModel != aiShadingMode_Gouraud) {
        qDebug() << "This mesh's shading model is not implemented in this loader, setting to default material";
        mater->Name = "DefaultMaterial";
    }
    else {
        aiColor3D dif (0.f,0.f,0.f);
        aiColor3D amb (0.f,0.f,0.f);
        aiColor3D spec (0.f,0.f,0.f);
        float shine = 0.0;

        material->Get( AI_MATKEY_COLOR_AMBIENT, amb);
        material->Get( AI_MATKEY_COLOR_DIFFUSE, dif); //->Get(<material-key>,<where-to-store>))
        material->Get( AI_MATKEY_COLOR_SPECULAR, spec);
        material->Get( AI_MATKEY_SHININESS, shine);

        mater->Ambient = QVector3D(amb.r, amb.g, amb.b);
        mater->Diffuse = QVector3D(dif.r, dif.g, dif.b);
        mater->Specular = QVector3D(spec.r, spec.g, spec.b);
        mater->Shininess = shine;

        mater->Ambient *= .2f;
        if( mater->Shininess == 0.0) mater->Shininess = 30;

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            qDebug() << "Diffuse Texture(s) Found:" << material->GetTextureCount(aiTextureType_DIFFUSE)
                     << "for Material:" << mater->Name;
            aiString texPath;

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                QString texturePath = texPath.data;
                mater->isTexture = true;
                mater->textureName = texturePath;
                qDebug() << "  Texture path" << texturePath;
            }
            else
                qDebug() << "  Failed to get texture for material";
        }
    }

    return mater;
}

QSharedPointer<Mesh> ModelLoader::processMesh(aiMesh *mesh) {
    QSharedPointer<Mesh> newMesh(new Mesh);
    newMesh->indexOffset = m_indices.size();
    unsigned int indexCountBefore = m_indices.size();
    int vertindexoffset = m_vertices.size()/3;

    // Get Vertices
    for(uint ii=0; ii<mesh->mNumVertices; ++ii) {
        aiVector3D &vec = mesh->mVertices[ii];
        m_vertices.push_back(vec.x);
        m_vertices.push_back(vec.y);
        m_vertices.push_back(vec.z);
    }

    // Get Normals
    m_normals.resize(m_vertices.size());
    int nind = vertindexoffset * 3;

    for(uint ii=0; ii<mesh->mNumVertices; ++ii) {
        aiVector3D &vec = mesh->mNormals[ii];
        m_normals[nind] = vec.x;
        m_normals[nind+1] = vec.y;
        m_normals[nind+2] = vec.z;
        nind += 3;
    };

    // Get mesh indexes
    for(uint t = 0; t<mesh->mNumFaces; ++t) {
        aiFace* face = &mesh->mFaces[t];

        m_indices.push_back(face->mIndices[0]+vertindexoffset);
        m_indices.push_back(face->mIndices[1]+vertindexoffset);
        m_indices.push_back(face->mIndices[2]+vertindexoffset);
    }

    newMesh->indexCount = m_indices.size() - indexCountBefore;
    newMesh->material = m_materials.at(mesh->mMaterialIndex);

    return newMesh;
}

void ModelLoader::processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode) {
    static int nodeIndex = 0;

    newNode.name = node->mName.length != 0 ? node->mName.C_Str() : "";

    newNode.tail = jsonArr3toQVec3(pointsJson.value(newNode.name).toObject().value("tail").toArray());
    newNode.head = jsonArr3toQVec3(pointsJson.value(newNode.name).toObject().value("head").toArray());

    newNode.transformation = QMatrix4x4(node->mTransformation[0]);
    if (newNode.name.contains(QString("Forearm"))){
        newNode.transformation.translate(newNode.head);
        newNode.transformation.rotate(30.0f,0,0,1);
        newNode.transformation.translate(-newNode.head);
    }

    newNode.meshes.resize(node->mNumMeshes);
    for(uint imesh = 0; imesh < node->mNumMeshes; ++imesh) {
        QSharedPointer<Mesh> mesh = m_meshes[node->mMeshes[imesh]];
        newNode.meshes[imesh] = mesh;
    }

    qDebug() << "NodeName" << newNode.name;
    qDebug() << "  NodeIndex" << nodeIndex;
    qDebug() << "  Tail" << newNode.tail;
    qDebug() << "  Head" << newNode.head;
    qDebug() << "  NumChildren" << node->mNumChildren;
    qDebug() << "  NumMeshes" << newNode.meshes.size();
    for (int ii=0; ii<newNode.meshes.size(); ++ii) {
        qDebug() << "    MaterialName" << newNode.meshes[ii]->material->Name;
        qDebug() << "    MeshVertices" << newNode.meshes[ii]->indexCount;
    }

    ++nodeIndex;

    for(uint ich = 0; ich < node->mNumChildren; ++ich) {
        newNode.nodes.push_back(Node());
        processNode(scene, node->mChildren[ich], parentNode, newNode.nodes[ich]);
    }
}

QVector3D ModelLoader::jsonArr3toQVec3(QJsonArray jsonArr3){
    if (jsonArr3.size() != 3){
        qDebug() << "ModelLoader::jsonArr3toQVec3: requires 3-element QJsonArray, but the actual size was " << jsonArr3.size();
        return QVector3D(0,0,0);
    }
    return QVector3D(jsonArr3[0].toDouble(),
            jsonArr3[2].toDouble(),
            -jsonArr3[1].toDouble());
}

Node ModelLoader::getNodeByName(QString name){
    QVector<Node> nodes = m_rootNode.data()->nodes;
    for (int i = 0; i < nodes.size(); i++){
        if (nodes[i].name == name)
            return nodes[i];
    }
    qDebug() << "ModelLoader::getNodeByName: given name not found in node list - returning empty node";
    Node n;
    return n;
}
