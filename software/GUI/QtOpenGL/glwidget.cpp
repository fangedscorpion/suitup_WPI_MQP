#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

GLWidget::GLWidget(QString filepath, ModelLoader::PathType pathType)
    : QOpenGLWidget(0),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_filepath(filepath),
      m_pathType(pathType),
      m_texturePath(QString("")),
      m_error(false)
      /*m_program(0)*/ {
}

GLWidget::~GLWidget()
{
    cleanup();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::cleanup()
{
//    makeCurrent();
//    m_logoVbo.destroy();
//    delete m_program;
//    m_program = 0;
//    doneCurrent();
}

//static const char *vertexShaderSource =
//    "attribute vec4 vertex;\n"
//    "attribute vec3 normal;\n"
//    "varying vec3 vert;\n"
//    "varying vec3 vertNormal;\n"
//    "uniform mat4 projMatrix;\n"
//    "uniform mat4 mvMatrix;\n"
//    "uniform mat3 normalMatrix;\n"
//    "void main() {\n"
//    "   vert = vertex.xyz;\n"
//    "   vertNormal = normalMatrix * normal;\n"
//    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
//    "}\n";

//static const char *fragmentShaderSource =
//    "varying highp vec3 vert;\n"
//    "varying highp vec3 vertNormal;\n"
//    "uniform highp vec3 lightPos;\n"
//    "void main() {\n"
//    "   highp vec3 L = normalize(lightPos - vert);\n"
//    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
//    "   highp vec3 color = vec3(0.39, 1.0, 1.0);\n"
//    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
//    "   gl_FragColor = vec4(col, 1.0);\n"
//    "}\n";

void GLWidget::initializeGL()
{
//    initializeOpenGLFunctions();
//    glClearColor(0, 0, 0, 1);

//    m_program = new QOpenGLShaderProgram;
//    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
//    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
//    m_program->bindAttributeLocation("vertex", 0);
//    m_program->bindAttributeLocation("normal", 1);
//    m_program->link();

//    m_program->bind();
//    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
//    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
//    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
//    m_lightPosLoc = m_program->uniformLocation("lightPos");

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
//    m_vao.create();
//    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

//    // Setup our vertex buffer object.
//    m_logoVbo.create();
//    m_logoVbo.bind();
//    m_logoVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));

//    // Store the vertex attribute bindings for the program.
//    setupVertexAttribs();

//    // Our camera never changes in this example.
//    m_camera.setToIdentity();
//    m_camera.translate(0, 0, -1);

//    // Light position is fixed.
//    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

//    m_program->release();

    initializeOpenGLFunctions();

    createShaderProgram(":/es_ads_fragment.vert", ":/es_ads_fragment.frag");

    createBuffers();
    createAttributes();
    setupLightingAndMatrices();

    glEnable(GL_DEPTH_TEST);
    glClearColor(.9f, .9f, .93f ,1.0f);

}

//void GLWidget::setupVertexAttribs()
//{
//    m_logoVbo.bind();
//    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
//    f->glEnableVertexAttribArray(0);
//    f->glEnableVertexAttribArray(1);
//    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
//    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
//    m_logoVbo.release();
//}

void  GLWidget::createShaderProgram(QString vShader, QString fShader)
{
    // Compile vertex shader
    if ( !m_error && !m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::Vertex, vShader.toUtf8() ) ) {
        qDebug() << "Error in3_Core vertex shader:" << m_shaderProgram.log();
        m_error = true;
    }

    // Compile fragment shader
    if ( !m_error && !m_shaderProgram.addShaderFromSourceFile( QOpenGLShader::Fragment, fShader.toUtf8() ) ) {
        qDebug() << "Error in fragment shader:" << m_shaderProgram.log();
        m_error = true;
    }

    // Link the shaders together into a program
    if ( !m_error && !m_shaderProgram.link() ) {
        qDebug() << "Error linking shader program:" << m_shaderProgram.log();
        m_error = true;
    }
}


void GLWidget::createBuffers()
{
    if(m_error)
        return;

    ModelLoader model;

    if(!model.Load(m_filepath, m_pathType))
    {
        m_error = true;
        return;
    }

    QVector<float> *vertices;
    QVector<float> *normals;
    QVector<QVector<float> > *textureUV;
    QVector<unsigned int> *indices;

    model.getBufferData(&vertices, &normals, &indices);
    model.getTextureData(&textureUV, 0, 0);

    // Create a vertex array object
    m_vao.create();
    m_vao.bind();

    // Create a buffer and copy the vertex data to it
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( &(*vertices)[0], vertices->size() * sizeof( float ) );

    // Create a buffer and copy the vertex data to it
    m_normalBuffer.create();
    m_normalBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_normalBuffer.bind();
    m_normalBuffer.allocate( &(*normals)[0], normals->size() * sizeof( float ) );

    if(textureUV != 0 && textureUV->size() != 0)
    {
        // Create a buffer and copy the vertex data to it
        m_textureUVBuffer.create();
        m_textureUVBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
        m_textureUVBuffer.bind();
        int texSize = 0;
        for(int ii=0; ii<textureUV->size(); ++ii)
            texSize += textureUV->at(ii).size();

        m_textureUVBuffer.allocate( &(*textureUV)[0][0], texSize * sizeof( float ) );
    }

    // Create a buffer and copy the index data to it
    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_indexBuffer.bind();
    m_indexBuffer.allocate( &(*indices)[0], indices->size() * sizeof( unsigned int ) );

    m_rootNode = model.getNodeData();
}

void GLWidget::createAttributes()
{
    if(m_error)
        return;

    m_vao.bind();
    // Set up the vertex array state
    m_shaderProgram.bind();

    // Map vertex data to the vertex shader's layout location '0'
    m_vertexBuffer.bind();
    m_shaderProgram.enableAttributeArray( 0 );      // layout location
    m_shaderProgram.setAttributeBuffer( 0,          // layout location
                                        GL_FLOAT,   // data's type
                                        0,          // Offset to data in buffer
                                        3);         // number of components (3 for x,y,z)

    // Map normal data to the vertex shader's layout location '1'
    m_normalBuffer.bind();
    m_shaderProgram.enableAttributeArray( 1 );      // layout location
    m_shaderProgram.setAttributeBuffer( 1,          // layout location
                                        GL_FLOAT,   // data's type
                                        0,          // Offset to data in buffer
                                        3);         // number of components (3 for x,y,z)

    if(m_textureUVBuffer.isCreated()) {
        m_textureUVBuffer.bind();
        m_shaderProgram.enableAttributeArray( 2 );      // layout location
        m_shaderProgram.setAttributeBuffer( 2,          // layout location
                                            GL_FLOAT,   // data's type
                                            0,          // Offset to data in buffer
                                            2);         // number of components (2 for u,v)
    }

}

void GLWidget::setupLightingAndMatrices()
{
    m_view.setToIdentity();
    m_view.lookAt(
                QVector3D(0.0f, 0.0f, 1.2f),    // Camera Position
                QVector3D(0.0f, 0.0f, 0.0f),    // Point camera looks towards
                QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

    float aspect = 4.0f/3.0f;
    m_projection.setToIdentity();
    m_projection.perspective(
                60.0f,          // field of vision
                aspect,         // aspect ratio
                0.3f,           // near clipping plane
                1000.0f);       // far clipping plane

    m_lightInfo.Position = QVector4D( -1.0f, 1.0f, 1.0f, 1.0f );
    m_lightInfo.Intensity = QVector3D( 1.0f, 1.0f, 1.0f);

    m_materialInfo.Ambient = QVector3D( 0.05f, 0.2f, 0.05f );
    m_materialInfo.Diffuse = QVector3D( 0.3f, 0.5f, 0.3f );
    m_materialInfo.Specular = QVector3D( 0.6f, 0.6f, 0.6f );
    m_materialInfo.Shininess = 50.0f;
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

//    m_world.setToIdentity();
//    m_world.rotate(m_xRot / 16.0f + 180, 1, 0, 0);
//    m_world.rotate(m_zRot / 16.0f + 180, 0, 0, 1);

//    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
//    m_program->bind();
//    m_program->setUniformValue(m_projMatrixLoc, m_proj);
//    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
//    QMatrix3x3 normalMatrix = m_world.normalMatrix();
//    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

//    glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());

//    m_program->release();




    if(m_error)
        return;

    // Clear color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind shader program
    m_shaderProgram.bind();

    // Set the model matrix
    // Translate and rotate it a bit to get a better view of the model
    m_model.setToIdentity();
    m_model.translate(0.0f, 0.0f, 0.0f);
//    m_model.rotate(m_xRot, 1.0f, 0.0f, 0.0f);
    m_model.rotate(m_zRot, 0.0f, 0.0f, 1.0f);

    // Set shader uniforms for light information
    m_shaderProgram.setUniformValue( "lightPosition", m_lightInfo.Position );
    m_shaderProgram.setUniformValue( "lightIntensity", m_lightInfo.Intensity );

    // Bind VAO and draw everything
    m_vao.bind();
    drawNode(m_rootNode.data(), QMatrix4x4());
    m_vao.release();


}


void GLWidget::resizeGL(int w, int h)
{
//    m_proj.setToIdentity();
//    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);

    glViewport( 0, 0, w, h );

    m_projection.setToIdentity();
    m_projection.perspective(60.0f, (float)w/h, .3f, 1000);

}


void GLWidget::drawNode(const Node *node, QMatrix4x4 objectMatrix)
{
    // Prepare matrices
    objectMatrix *= node->transformation;
    QMatrix4x4 modelMatrix = m_model * objectMatrix;
    QMatrix4x4 modelViewMatrix = m_view * modelMatrix;
    QMatrix3x3 normalMatrix = modelViewMatrix.normalMatrix();
    QMatrix4x4 mvp = m_projection * modelViewMatrix;

    m_shaderProgram.setUniformValue( "MV", modelViewMatrix );// Transforming to eye space
    m_shaderProgram.setUniformValue( "N", normalMatrix );    // Transform normal to Eye space
    m_shaderProgram.setUniformValue( "MVP", mvp );           // Matrix for transforming to Clip space

    // Draw each mesh in this node
    for(int imm = 0; imm<node->meshes.size(); ++imm)
    {
        if(node->meshes[imm]->material->Name == QString("DefaultMaterial"))
            setMaterialUniforms(m_materialInfo);
        else
            setMaterialUniforms(*node->meshes[imm]->material);

        glDrawElements( GL_TRIANGLES, node->meshes[imm]->indexCount, GL_UNSIGNED_INT
                            , (const void*)(node->meshes[imm]->indexOffset * sizeof(unsigned int)) );
    }

    // Recursively draw this nodes children nodes
    for(int inn = 0; inn<node->nodes.size(); ++inn)
        drawNode(&node->nodes[inn], objectMatrix);
}

void GLWidget::setMaterialUniforms(MaterialInfo &mater)
{
    m_shaderProgram.setUniformValue( "Ka", mater.Ambient );
    m_shaderProgram.setUniformValue( "Kd", mater.Diffuse );
    m_shaderProgram.setUniformValue( "Ks", mater.Specular );
    m_shaderProgram.setUniformValue( "shininess", mater.Shininess );
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}

