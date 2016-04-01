#include "glwidget.h"
#include <QMouseEvent>

GLWidget::GLWidget(Model* m) :
    QOpenGLWidget(0),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_filepath(QString("biped/final/biped_rig.obj")),
    m_pathType(ModelGLLoader::RelativePath),
    m_error(false),
    m_xRot(0),
    m_yRot(0),
    m_zRot(0),
    m_cam_offset(QVector3D(0,-0.3f,0)),
    model(m){

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(8);
    format.setVersion(3, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
}

GLWidget::~GLWidget(){
//    delete model;
//    delete modelGL;
}

static void qNormalizeAngle(int &angle) {
    while (angle < 0) angle += 360 * 16;
    while (angle > 360 * 16) angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        update();
    }
}

void GLWidget::setZRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        update();
    }
}


void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    createShaderProgram(":/ads_fragment.vert", ":/ads_fragment.frag");

    createBuffers();
    createAttributes();
    setupLightingAndMatrices();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f ,1.0f);
}

void  GLWidget::createShaderProgram(QString vShader, QString fShader) {
    // Compile vertex shader
    if (!m_error && !m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vShader.toUtf8())) {
        qDebug() << "Error in3_Core vertex shader:" << m_shaderProgram.log();
        m_error = true;
    }

    // Compile fragment shader
    if (!m_error && !m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fShader.toUtf8())) {
        qDebug() << "Error in fragment shader:" << m_shaderProgram.log();
        m_error = true;
    }

    // Link the shaders together into a program
    if (!m_error && !m_shaderProgram.link()) {
        qDebug() << "Error linking shader program:" << m_shaderProgram.log();
        m_error = true;
    }
}

void GLWidget::createBuffers(){
    if(m_error)
        return;

    ModelGLLoader modelLoader;
    if(!modelLoader.Load(m_filepath, m_pathType)){
        m_error = true;
        return;
    }

    QVector<float> *vertices;
    QVector<float> *normals;
    QVector<unsigned int> *indices;

    modelLoader.getBufferData(&vertices, &normals, &indices);

    // Create a vertex array object
    m_vao.create();
    m_vao.bind();

    // Create a buffer and copy the vertex data to it
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(&(*vertices)[0], vertices->size()*sizeof(float));

    // Create a buffer and copy the vertex data to it
    m_normalBuffer.create();
    m_normalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_normalBuffer.bind();
    m_normalBuffer.allocate(&(*normals)[0], normals->size()*sizeof(float));

    // Create a buffer and copy the index data to it
    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.bind();
    m_indexBuffer.allocate(&(*indices)[0], indices->size()*sizeof(unsigned int));

    modelGL = modelLoader.toModel();
    connect(model,SIGNAL(poseUpdated(QHash<QString,NodeState>)),modelGL,SLOT(updatePose(QHash<QString,NodeState>)));
    connect(modelGL,SIGNAL(modelGLChanged()),this,SLOT(update()));
    model->resetPose();
}

void GLWidget::createAttributes(){
    if(m_error) return;

    m_vao.bind();
    // Set up the vertex array state
    m_shaderProgram.bind();

    // Map vertex data to the vertex shader's layout location '0'
    m_vertexBuffer.bind();
    m_shaderProgram.enableAttributeArray(0);      // layout location
    m_shaderProgram.setAttributeBuffer(0,          // layout location
                                       GL_FLOAT,   // data's type
                                       0,          // Offset to data in buffer
                                       3);         // number of components (3 for x,y,z)

    // Map normal data to the vertex shader's layout location '1'
    m_normalBuffer.bind();
    m_shaderProgram.enableAttributeArray(1);      // layout location
    m_shaderProgram.setAttributeBuffer(1,          // layout location
                                       GL_FLOAT,   // data's type
                                       0,          // Offset to data in buffer
                                       3);         // number of components (3 for x,y,z)

}

void GLWidget::setupLightingAndMatrices() {
    m_view.setToIdentity();
    m_view.lookAt(
                QVector3D(-m_cam_offset + QVector3D(0,0,1.8f)),    // Camera Position
                QVector3D(-m_cam_offset),    // Point camera looks towards
                QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

    float aspect = 4.0f/3.0f;
    m_projection.setToIdentity();
    m_projection.perspective(
                60.0f,          // field of vision
                aspect,         // aspect ratio
                0.3f,           // near clipping plane
                1000.0f);       // far clipping plane

    m_lightInfo.Position = QVector4D(-1.0f, 1.0f, 1.0f, 1.0f);
    m_lightInfo.Intensity = QVector3D(1.0f, 1.0f, 1.0f);
}

void GLWidget::paintGL() {
    if(m_error)
        return;

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind shader program
    m_shaderProgram.bind();

    // Set the model matrix
    // Translate and rotate it a bit to get a better view of the model
    m_model.setToIdentity();
//    m_model.translate(-m_cam_offset);
    m_model.rotate(m_xRot/16, 1.0f, 0.0f, 0.0f);
    m_model.rotate(m_zRot/16, 0.0f, 1.0f, 0.0f);
//    m_model.translate(m_cam_offset);

    // Set shader uniforms for light information
    m_shaderProgram.setUniformValue("lightPosition", m_lightInfo.Position);
    m_shaderProgram.setUniformValue("lightIntensity", m_lightInfo.Intensity);

    // Bind VAO and draw everything
    m_vao.bind();
    drawNodes();
    m_vao.release();
}


void GLWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
    m_projection.setToIdentity();
    m_projection.perspective(60.0f, (float)w/h, .3f, 1000);
}


void GLWidget::drawNodes(){
    QVector<QSharedPointer<NodeGL> > nodes = modelGL->getNodes();
    for(int inn = 0; inn < nodes.size(); ++inn){
        // Prepare matrices
        QMatrix4x4 modelMatrix = m_model * nodes[inn]->getTransformation();
        QMatrix4x4 modelViewMatrix = m_view * modelMatrix;
        QMatrix3x3 normalMatrix = modelViewMatrix.normalMatrix();
        QMatrix4x4 mvp = m_projection * modelViewMatrix;

        m_shaderProgram.setUniformValue("MV", modelViewMatrix);// Transforming to eye space
        m_shaderProgram.setUniformValue("N", normalMatrix);    // Transform normal to Eye space
        m_shaderProgram.setUniformValue("MVP", mvp);           // Matrix for transforming to Clip space

        // Draw each mesh in this node
        for(int imm = 0; imm < nodes[inn]->getMeshes().size(); ++imm) {
            setMaterialUniforms(*(nodes[inn]->getMaterial()));

            glDrawElements(GL_TRIANGLES,
                           nodes[inn]->getMeshes()[imm]->indexCount,
                           GL_UNSIGNED_INT,
                           (const void*)(nodes[inn]->getMeshes()[imm]->indexOffset*sizeof(unsigned int)));
        }
    }
}

void GLWidget::setMaterialUniforms(Material &mater) {
    m_shaderProgram.setUniformValue("Ka", mater.Ambient);
    m_shaderProgram.setUniformValue("Kd", mater.Diffuse);
    m_shaderProgram.setUniformValue("Ks", mater.Specular);
    m_shaderProgram.setUniformValue("shininess", mater.Shininess);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        setXRotation(0);
        setZRotation(0);
    }
}
