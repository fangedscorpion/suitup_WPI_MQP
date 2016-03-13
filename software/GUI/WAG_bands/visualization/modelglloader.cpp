#include "modelglloader.h"
#include <limits>
#include <QJsonDocument>
#include <QJsonArray>
#include <stdexcept>

#define DEBUGOUTPUT_NORMALS(nodeIndex) (false)//( QList<int>{1}.contains(nodeIndex) )//(false)

ModelGLLoader::ModelGLLoader() {
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

bool ModelGLLoader::Load(QString filePath, PathType pathType) {
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

    QFile file("../biped/final/mtls.out");
    file.open(QFile::ReadOnly);
    QByteArray blob = file.readAll();
    QJsonArray mtlsJson = QJsonDocument::fromJson(blob).array();

    for(unsigned int ii=0; ii < scene->mNumMaterials; ++ii) {
        QSharedPointer<Material> mater = processMaterial(scene->mMaterials[ii]);
        mater->Name = mtlsJson[ii].toString();
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

void ModelGLLoader::getBufferData(QVector<float> **vertices, QVector<float> **normals, QVector<unsigned int> **indices) {
    if(vertices != 0)
        *vertices = &m_vertices;

    if(normals != 0)
        *normals = &m_normals;

    if(indices != 0)
        *indices = &m_indices;
}

QSharedPointer<Material> ModelGLLoader::processMaterial(aiMaterial *material) {
    QSharedPointer<Material> mater(new Material);

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

QSharedPointer<Mesh> ModelGLLoader::processMesh(aiMesh *mesh) {
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

void ModelGLLoader::processNode(aiNode *node) {

    for(uint ich = 0; ich < node->mNumChildren; ++ich) {
        QSharedPointer<NodeGL> n(new NodeGL());

        aiNode* thisNode = node->mChildren[ich];
        n->setName(thisNode->mName.length != 0 ? thisNode->mName.C_Str() : "");
        n->setTransformation(QMatrix4x4(thisNode->mTransformation[0]));

        for(uint imesh = 0; imesh < thisNode->mNumMeshes; ++imesh) {
            QSharedPointer<Mesh> mesh = m_meshes[thisNode->mMeshes[imesh]];
            n->addMesh(mesh);
        }

        qDebug() << "NodeName" << n->getName();

        m_nodes.push_back(n);
    }
}

QSharedPointer<NodeGL> ModelGLLoader::getNodeByName(QString name){
    for (int i = 0; i < m_nodes.size(); i++){
        if (m_nodes[i]->getName() == name)
            return m_nodes[i];
    }
    throw std::invalid_argument("given name not found in node list");
}

ModelGL ModelGLLoader::toModel(){
    return ModelGL(m_nodes,m_materials);
}
