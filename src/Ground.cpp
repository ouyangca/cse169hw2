#include "plane.hpp"

Plane::Plane(const QVector3D &box_min, const QVector3D &box_max, QOpenGLShaderProgram *program)
{
    QVector<float>  m_vertices;
    QVector<unsigned int>  m_indices;

    m_vertices.push_back(box_min.x());
    m_vertices.push_back(box_min.y());
    m_vertices.push_back(box_min.z());
    m_vertices.push_back(1.0f);

    m_vertices.push_back(0.0);
    m_vertices.push_back(1.0f);
    m_vertices.push_back(0.0f);
    m_vertices.push_back(0.0f);

    m_vertices.push_back(box_max.x());
    m_vertices.push_back(box_min.y());
    m_vertices.push_back(box_min.z());
    m_vertices.push_back(1.0f);

    m_vertices.push_back(0.0);
    m_vertices.push_back(1.0f);
    m_vertices.push_back(0.0f);
    m_vertices.push_back(0.0f);

    m_vertices.push_back(box_min.x());
    m_vertices.push_back(box_max.y());
    m_vertices.push_back(box_max.z());
    m_vertices.push_back(1.0f);

    m_vertices.push_back(0.0);
    m_vertices.push_back(1.0f);
    m_vertices.push_back(0.0f);
    m_vertices.push_back(0.0f);

    m_vertices.push_back(box_max.x());
    m_vertices.push_back(box_max.y());
    m_vertices.push_back(box_max.z());
    m_vertices.push_back(1.0f);

    m_vertices.push_back(0.0);
    m_vertices.push_back(1.0f);
    m_vertices.push_back(0.0f);
    m_vertices.push_back(0.0f);

    m_indices.push_back(2);
    m_indices.push_back(1);
    m_indices.push_back(0);

    m_indices.push_back(3);
    m_indices.push_back(1);
    m_indices.push_back(2);

    this->m_array_buffer.bind();
    this->m_array_buffer.allocate(m_vertices.data(), m_vertices.size() * sizeof(float));
    this->m_index_buffer.bind();
    this->m_index_buffer.allocate(m_indices.data(), m_indices.size() * sizeof(unsigned int));

    this->m_vao.bind();

    this->m_array_buffer.bind();
    quintptr offset = 0;
    int vertexLocation = program->attributeLocation("position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 4, 8 * sizeof(float));
    offset += 4 * sizeof(float);
    vertexLocation = program->attributeLocation("normal");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 4, 8 * sizeof(float));
    this->m_index_buffer.bind();

    this->m_vao.release();
}

void
Plane::render()
{
    this->m_vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    this->m_vao.release();
}