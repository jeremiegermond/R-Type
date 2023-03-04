/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Text.cpp
*/

#include "game/Game.hpp"

void Game::drawText3DPro(const std::string &text, Vector3 position, float fontSize, float fontSpacing, Color tint, Font font) {
    float offsetX = 0;
    float scale = fontSize / float(font.baseSize);
    for (auto letter : text) {
        int index = GetGlyphIndex(font, letter);
        drawLetter3D(letter, {position.x + offsetX, position.y, position.z}, fontSize, tint, font);
        offsetX += float(font.recs[index].width + fontSpacing) / float(font.baseSize) * scale;
    }
}

void Game::drawText3D(const std::string &text, Vector3 position, float fontSize, Color tint) {
    float spacing = fontSize / 10.0f;
    drawText3DPro(text, position, fontSize, spacing, tint);
}

void Game::drawLetter3D(const char letter, Vector3 positionBase, float fontSize, Color tint, Font font) {
    auto position = Vector3{positionBase.x, positionBase.z, -positionBase.y};
    int index = GetGlyphIndex(font, letter);
    float scale = fontSize / float(font.baseSize);

    auto glyph = font.glyphs[index];
    position.x += float(glyph.offsetX - font.glyphPadding) / float(font.baseSize) * scale;
    position.z += float(glyph.offsetY - font.glyphPadding) / float(font.baseSize) * scale;
    auto fontRec = font.recs[index];
    Rectangle letterRec;
    letterRec.x = fontRec.x - float(font.glyphPadding);
    letterRec.y = fontRec.y - float(font.glyphPadding);
    letterRec.width = fontRec.width + 2.0f * float(font.glyphPadding);
    letterRec.height = fontRec.height + 2.0f * float(font.glyphPadding);
    float width = letterRec.width / float(font.baseSize) * scale;
    float height = letterRec.height / float(font.baseSize) * scale;

    if (font.texture.id > 0) {
        const float x = 0;
        const float y = 0;
        const float z = 0;

        // normalized texture coordinates of the glyph inside the font texture (0.0f -> 1.0f)
        const float tx = letterRec.x / float(font.texture.width);
        const float ty = letterRec.y / float(font.texture.height);
        const float tw = (letterRec.x + letterRec.width) / float(font.texture.width);
        const float th = (letterRec.y + letterRec.height) / float(font.texture.height);

        rlCheckRenderBatchLimit(4);
        rlSetTexture(font.texture.id);

        rlPushMatrix();
        rlTranslatef(position.x, position.y, position.z);

        rlBegin(RL_QUADS);
        rlColor4ub(tint.r, tint.g, tint.b, tint.a);

        // Front Face
        rlNormal3f(0, 1, 0);
        rlTexCoord2f(tx, ty);
        rlVertex3f(x, y, z);
        rlTexCoord2f(tx, th);
        rlVertex3f(x, y, z + height);
        rlTexCoord2f(tw, th);
        rlVertex3f(x + width, y, z + height);
        rlTexCoord2f(tw, ty);
        rlVertex3f(x + width, y, z);
        rlEnd();
        rlPopMatrix();

        rlSetTexture(0);
    }
}

Vector3 Game::measureText3D(const std::string &text, float fontSize, Font font) {
    Vector3 size{};
    float fontSpacing = fontSize / 10.0f;
    float scale = fontSize / float(font.baseSize);
    for (auto letter : text) {
        int index = GetGlyphIndex(font, letter);
        size.x += float(font.recs[index].width + fontSpacing) / float(font.baseSize) * scale;
    }
    return size;
}
