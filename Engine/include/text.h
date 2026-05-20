#ifndef Engine_text_h_
#define Engine_text_h_

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>

struct Font;

#include <render.h>
#include <vec2.h>

struct Font {
    TTF_Font *sdl_font;
};

bool initText();

void setTextCacheSize(int cache_size);

Font loadFont(const char *path);

Texture getTextTexture(SDL_Renderer *renderer, const char *text, Font font, float ptSize, Color color);

TTF_TextEngine* getEngine(SDL_Renderer *renderer);

Vec2 measureText(const char *text, Font font, float ptSize);

#endif