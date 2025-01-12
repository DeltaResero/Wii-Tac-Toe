/**
 * @file grrlib_class.h
 * Contains the GRRLIB wrapper class.
 */

#ifndef GRRLIB_WrapperH
#define GRRLIB_WrapperH
//---------------------------------------------------------------------------

#include <grrlib.h>
#include <string>
#include <memory>

/**
 * Namespace containing all GRRLIB code.
 * @author Crayon
 */
namespace GRRLIBpp
{

/**
 * This class is a wrapper for GRRLIB_texImg.
 * @author Crayon
 */
class Texture : protected GRRLIB_texImg
{
public:
    Texture();
    Texture(const u8 *Buffer, const u32 Size = 0);
    Texture(const char *filename);
    Texture(const std::string &filename);
    Texture(const u32 w, const u32 h);
    ~Texture();

    [[nodiscard]] u32 GetWidth();
    [[nodiscard]] u32 GetHeight();
    [[nodiscard]] u32 GetFormat();
    [[nodiscard]] u32 GetOffsetX();
    [[nodiscard]] u32 GetOffsetY();
    void SetOffset(u32 X, u32 Y);
    void SetOffsetX(u32 X);
    void SetOffsetY(u32 Y);
    [[nodiscard]] u32 GetHandleX();
    [[nodiscard]] u32 GetHandleY();
    void SetHandle(u32 X, u32 Y);
    [[nodiscard]] u32 GetPixel(const s32 x, const s32 y);
    void SetPixel(const s32 x, const s32 y, const u32 color);
    void Refresh();
    void Load(const u8 *Buffer, const u32 Size = 0);
    void Load(const char *filename);
    void Load(const std::string &filename);
    void Create(const u32 w, const u32 h, const u32 Color = 0x00000000);
    void Draw(const f32 xpos, const f32 ypos, const f32 degrees,
              const f32 scaleX, const f32 scaleY, const u32 color);
    void Draw(const f32 xpos, const f32 ypos, const f32 degrees,
              const f32 scaleX, const f32 scaleY);
    void Draw(const f32 xpos, const f32 ypos, const f32 degrees);
    void Draw(const f32 xpos, const f32 ypos);
    void DrawTile(const f32 xpos, const f32 ypos, const f32 degrees,
                  const f32 scaleX, const f32 scaleY, const u32 color, int frame);
    void CopyScreen(u16 posx = 0, u16 posy = 0, bool clear = false);
    void SetColor(u32);
    [[nodiscard]] u32 GetColor();
    void SetAlpha(u8);
    [[nodiscard]] u8 GetAlpha();

    [[nodiscard]] static std::unique_ptr<Texture> CreateFromPNG(const u8 *Buffer);
private:
    void Assign(GRRLIB_texImg *other);
    u32 _Color;  /**< The color used to draw the texture. By default it is set to 0xFFFFFFFF. */
    f32 _ScaleX; /**< The X scale used to draw the texture. By default it is set to 1.0. */
    f32 _ScaleY; /**< The Y scale used to draw the texture. By default it is set to 1.0. */
    f32 _Angle;  /**< The angle used to draw the texture. By default it is set to 0. */
};

/**
 * Namespace containing functions related to the screen.
 * @author Crayon
 */
namespace Screen
{
    s32 Initialize();
    void Exit();
    void Render();

    void SetBackgroundColor(u8 r, u8 g, u8 b, u8 a = 0xFF);
    void SetBackgroundColor(const u32 color);
    void FillScreen(const u32 color);
    void SetPixel(const f32 x, const f32 y, const u32 color);
    void Line(const f32 x1, const f32 y1, const f32 x2, const f32 y2, const u32 color);
    void Rectangle(const f32 x, const f32 y, const f32 width, const f32 height, const u32 color, const bool filled);
    void Circle(const f32 x, const f32 y, const f32 radius, const u32 color, const u8 filled);

    [[nodiscard]] u16 GetWidth();
    [[nodiscard]] u16 GetHeight();

    bool ScreenShot(const char* filename);
    bool ScreenShot(const std::string &filename);
}   /* namespace Screen */
using namespace Screen;

/**
 * Namespace containing FX functions.
 * @author Crayon
 */
namespace FX
{
    void FlipH(const Texture *texsrc, Texture *texdest);
    void FlipV(const Texture *texsrc, Texture *texdest);
    void Grayscale(const Texture *texsrc, Texture *texdest);
    void Sepia(const Texture *texsrc, Texture *texdest);
    void Invert(const Texture *texsrc, Texture *texdest);
    void Blur(const Texture *texsrc, Texture *texdest, const u32 factor);
    void Scatter(const Texture *texsrc, Texture *texdest, const u32 factor);
    void Pixelate(const Texture *texsrc, Texture *texdest, const u32 factor);
}   /* namespace FX */
using namespace FX;

}   /* namespace GRRLIBpp */
using namespace GRRLIBpp;
//---------------------------------------------------------------------------
#endif
