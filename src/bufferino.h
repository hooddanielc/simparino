#pragma once

#include <vector>
#include <map>
#include <GL/glew.h>

class AnyBufferino {
public:
    virtual size_t getSize() const = 0;
    virtual ~AnyBufferino(){}
};

template <typename T>
class Bufferino final : public AnyBufferino {
public:
    Bufferino(std::vector<T> &&newData, size_t newColumns)
        : data(std::move(newData)), columns(newColumns) {
        glGenBuffers(1, &vbo);
    }
    virtual ~Bufferino() {
        glDeleteBuffers(1, &vbo);
    }
    void setData(std::vector<T> &&newData) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, getSize(), newData.data(), GL_STATIC_DRAW);
        data = std::move(newData);
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
std::shared_ptr<Bufferino<T>> MakeBufferino(std::vector<T> &&data, size_t columns) {
    return std::make_shared<Bufferino<T>>(std::move(data), columns);
}

class BufferSequerino {
public:
    GLuint vao;
    void pushBuffer(std::shared_ptr<AnyBufferino> &buff);
    void addBuffer(int idx, GLfloat *data, int size);
    void enable();
    void disable();
    BufferSequerino();
    ~BufferSequerino();
private:
    std::map<int, GLuint> vbos;
};
