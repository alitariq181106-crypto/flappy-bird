#include <text.h>
#include <timing.h>

#include <map>
#include <string>

// Cache size
int TTF_cache_size = 100;

struct TextID {
    Font font;
    Color color;
    float ptSize;
    std::string text;
};

struct TextIDComp {
    bool operator()(const TextID& a, const TextID& b) const {
        // Compare Text
        if(a.text < b.text) {
            return true;
        } else if(a.text > b.text) {
            return false;
        }

        // Compare Color
        if(a.color.r < b.color.r) {
            return true;
        } else if(a.color.r > b.color.r) {
            return false;
        }
        if(a.color.g < b.color.g) {
            return true;
        } else if(a.color.g > b.color.g) {
            return false;
        }
        if(a.color.b < b.color.b) {
            return true;
        } else if(a.color.b > b.color.b) {
            return false;
        }
        if(a.color.a < b.color.a) {
            return true;
        } else if(a.color.a > b.color.a) {
            return false;
        }

        // Compare Font
        if(a.font.sdl_font < b.font.sdl_font) {
            return true;
        } else if(a.font.sdl_font > b.font.sdl_font) {
            return false;
        }

        // Compare Size
        if(a.ptSize < b.ptSize) {
            return true;
        } else if(a.ptSize > b.ptSize) {
            return false;
        }

        return false;
    }
};

struct TextureComp {
    bool operator()(const Texture &a, const Texture &b) const {
        if(a.texture < b.texture) {
            return true;
        } else if(a.texture > b.texture) {
            return false;
        }

        if(a.src.x < b.src.x) {
            return true;
        } else if(a.src.x > b.src.x) {
            return false;
        }

        if(a.src.y < b.src.y) {
            return true;
        } else if(a.src.y > b.src.y) {
            return false;
        }

        if(a.src.width < b.src.width) {
            return true;
        } else if(a.src.width > b.src.width) {
            return false;
        }

        if(a.src.height < b.src.height) {
            return true;
        } else if(a.src.height > b.src.height) {
            return false;
        }
        return false;
    }
};

std::map<std::string, TTF_Font*> fonts;
std::map<TextID, Texture, TextIDComp> textureCache;
std::map<Texture, Time, TextureComp> lastUsed;

TTF_TextEngine *engine = nullptr;


// Initialise Text System
bool initText() {
    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init: %s\n", SDL_GetError());
        return false;
    }

    // Default Font?

    return true;
}

// Set Cache Size
void setTextCacheSize(int cache_size) {
    TTF_cache_size = cache_size;
}

Font loadFont(const char *path) {
    // Load Font
    TTF_Font *font = TTF_OpenFont(path, 10.0f);

    if(!font) {
        // Error message
        SDL_Log("Error loading Font: %s\n", SDL_GetError());
    }

    return Font{font};
}

Texture getTextTexture(SDL_Renderer *renderer, const char *text, Font font, float ptSize, Color color) {
    // Try to find Texture in map
    auto it = textureCache.find(TextID{font, color, ptSize, text});

    if(it != textureCache.end()) {
        lastUsed[it->second] = getTime();
        return it->second;
    }

    if(textureCache.size() >= TTF_cache_size) {
        auto oldest = textureCache.begin();
        auto lastIt = textureCache.begin();
        while(lastIt != textureCache.end()) {
            if(lastUsed[lastIt->second].nanoseconds < lastUsed[oldest->second].nanoseconds) {
                oldest = lastIt;
            }
            lastIt++;
        }

        SDL_DestroyTexture(oldest->second.texture);

        lastUsed.erase(oldest->second);
        
        textureCache.erase(oldest->first);
    }


    // Create new Texture

    TTF_SetFontSize(font.sdl_font, ptSize);

    SDL_Surface *textSurface = TTF_RenderText_Blended(font.sdl_font, text, 0, {color.r, color.g, color.b, color.a});
    
    SDL_Texture *sdlTexture = nullptr;
    if (textSurface) {
        sdlTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_DestroySurface(textSurface);
    }

    Texture texture = {{0, 0, (float)sdlTexture->w, (float)sdlTexture->h}, sdlTexture};

    textureCache.insert(std::pair<TextID, Texture>({font, color, ptSize, text}, texture));
    lastUsed.insert(std::pair<Texture, Time>(texture, getTime()));

    return texture;
}

Vec2 measureText(const char *text, Font font, float ptSize) {
    int x,y;
    TTF_SetFontSize(font.sdl_font, ptSize);
    TTF_GetStringSize(font.sdl_font, text, 0, &x, &y);
    return Vec2{(float)x, (float)y};
}