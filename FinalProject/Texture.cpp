#include "Texture.h"

Texture::Texture(const char* texturePath)
{
    // 保存纹理路径
    this->texturePath = texturePath;
}

void Texture::Init()
{
    // 生成纹理对象
    glGenTextures(1, &textureID);
    // 绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, textureID);
    // 设置纹理参数：缩小过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // 设置纹理参数：放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    // 加载图像数据
    unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGBA);
    // 生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    // 释放图像数据
    SOIL_free_image_data(image);
    // 解绑纹理对象
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Use(GLenum activeTexture)
{
    // 激活指定的纹理单元
    glActiveTexture(activeTexture);
    // 绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, textureID);
}
