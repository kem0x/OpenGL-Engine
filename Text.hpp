#pragma once
#include <string>
#include "gltext.h"

#include "Color.hpp"

struct Text : DrawableObject
{
    float X, Y;
    GLTtext* _Text = nullptr;
    Color _Color { 255, 255, 255, 255 };
    std::string TextString;

    Text(float InX, float InY, std::string InText, Color InColor)
    {
        X = InX;
        Y = InY;
        _Text = gltCreateText();
        TextString = InText;
        SetText(InText);
        SetColor(InColor);
    }

    void SetText(std::string InText)
    {
        if (InText == TextString)
            return;

        TextString = InText;
        gltSetText(_Text, InText.c_str());
    }

    void operator=(std::string InText)
    {
        return SetText(InText);
    }

    void SetColor(Color c)
    {
        _Color = c;
    }

    virtual void Draw(std::shared_ptr<ShaderProgram> Shader) override
    {
        gltBeginDraw();

        gltColor(_Color.R, _Color.G, _Color.B, _Color.A);
        gltDrawText2D(_Text, X, Y, 1.0f);

        gltEndDraw();
    }
};