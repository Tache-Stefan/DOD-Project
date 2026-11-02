#pragma once

#include <raylib.h>
#include <cmath>

class CircleTexture {
public:
    static void init() {
        if (s_isInitialized) return;

        constexpr int TEX_SIZE = 64;
        Image img = GenImageColor(TEX_SIZE, TEX_SIZE, BLANK);
        const float center = TEX_SIZE * 0.5f;
        const float radius = center - 1.5f;
        Color* pixels = static_cast<Color*>(img.data);

        for (int y = 0; y < TEX_SIZE; ++y) {
            for (int x = 0; x < TEX_SIZE; ++x) {
                const float dx = (static_cast<float>(x) + 0.5f) - center;
                const float dy = (static_cast<float>(y) + 0.5f) - center;
                const float dist = std::sqrt(dx * dx + dy * dy);

                float alpha = 0.0f;
                if (dist <= radius - 0.75f) {
                    alpha = 1.0f;
                } else if (dist < radius + 0.75f) {
                    alpha = (radius + 0.75f - dist) / 1.5f;
                }

                pixels[y * TEX_SIZE + x] = Color{
                    255, 255, 255,
                    static_cast<unsigned char>(alpha * 255.0f)
                };
            }
        }

        s_texture = LoadTextureFromImage(img);
        SetTextureFilter(s_texture, TEXTURE_FILTER_BILINEAR);
        UnloadImage(img);
        s_isInitialized = true;
    }

    static void shutdown() {
        if (s_isInitialized) {
            UnloadTexture(s_texture);
            s_isInitialized = false;
        }
    }

    static void draw(float x, float y, float radius, Color color) {
        const float diameter = radius * 2.0f;
        const Rectangle src{ 0.0f, 0.0f, static_cast<float>(s_texture.width), static_cast<float>(s_texture.height) };
        const Rectangle dest{ x, y, diameter, diameter };
        const Vector2 origin{ radius, radius };
        DrawTexturePro(s_texture, src, dest, origin, 0.0f, color);
    }

private:
    inline static Texture2D s_texture{};
    inline static bool s_isInitialized = false;
};
