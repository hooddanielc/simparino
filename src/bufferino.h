#pragma once

#include <vector>
#include <map>
#include <GL/glew.h>
#include <iostream>
#include <string>

class AnyBufferino {
public:
    virtual size_t getSize() const = 0;
    virtual size_t getColumns() const = 0;
    virtual GLuint getId() const = 0;
    virtual ~AnyBufferino(){}
};

template <typename T>
class Bufferino final : public AnyBufferino {
public:
    Bufferino(std::vector<T> &&newData, size_t newColumns)
        : data(std::move(newData)), columns(newColumns) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, getSize(), data.data(), GL_STATIC_DRAW);
    }
    virtual ~Bufferino() {
        glDeleteBuffers(1, &vbo);
    }
    void setData(std::vector<T> &&newData) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, getSize(), newData.data(), GL_STATIC_DRAW);
        data = std::move(newData);
    }
    virtual GLuint getId() const override {
        return vbo;
    }
    virtual size_t getColumns() const override {
        return columns;
    }
    virtual size_t getSize() const override {
        return sizeof(T) * data.size();
    }
private:
    GLuint vbo;
    std::vector<T> data;
    size_t columns;
};

template <typename T>
std::shared_ptr<AnyBufferino> MakeBufferino(std::vector<T> &&data, size_t columns) {
    return std::make_shared<Bufferino<T>>(std::move(data), columns);
}

class BufferSequerino {
public:
    void pushBuffer(std::shared_ptr<AnyBufferino> buff);
    void pushTexture(GLenum textureUnit, GLuint textureId);
    void bind();
    void build();
    BufferSequerino();
    ~BufferSequerino();
private:
    std::map<GLuint, GLuint> somevbos;
    std::vector<std::shared_ptr<AnyBufferino>> vbos;
    std::map<GLenum, GLuint> tbos;
    GLuint vao;
};
