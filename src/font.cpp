#include "font.h"
#include <cstdio>

using namespace FB;

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"


String blah_get_font_name(stbtt_fontinfo* font, int name_id) {
  int length = 0;

  const u16* ptr = (const u16*) stbtt_GetFontNameString(font, &length,
      STBTT_PLATFORM_ID_MICROSOFT,
      STBTT_MS_EID_UNICODE_BMP,
      STBTT_MS_LANG_ENGLISH,
      name_id);

  String res;
  res.buffer = (char*)ptr;
  res.length = length;
  return res;
}


Font* Font::create(const FilePath& path) {

  auto file = Platform::file_open(path, FileMode::OpenRead);

  u8* buffer = (u8*) malloc(file->length());
  auto n = file->read(buffer, file->length());
  
  auto fn = new stbtt_fontinfo();
  if (stbtt_InitFont(fn, buffer, 0) == 0) {
    printf("Unable to parse Font file");
      return 0;
  }

  auto font = new Font();
  font->m_font = fn;
  font->m_buffer = std::move(buffer);
  font->m_family_name = blah_get_font_name(fn, 1);
  font->m_style_name = blah_get_font_name(fn, 2);
  stbtt_GetFontVMetrics(fn, &font->m_ascent, &font->m_descent, &font->m_line_gap);


  return font;
}




int Font::get_glyph(Codepoint codepoint) const {
  return stbtt_FindGlyphIndex((stbtt_fontinfo*)m_font, codepoint);
}

float Font::get_scale(float size) const {
  return stbtt_ScaleForMappingEmToPixels((stbtt_fontinfo*) m_font, size);
}

Font::Character Font::get_character(int glyph, float scale) const {
  Character ch;

  int advance, offsetX, x0, y0, x1, y1;

  stbtt_GetGlyphHMetrics((stbtt_fontinfo*)m_font, glyph, &advance, &offsetX);
  stbtt_GetGlyphBitmapBox((stbtt_fontinfo*)m_font, glyph, scale, scale, &x0, &y0, &x1, &y1);

  int w = (x1 - x0);
  int h =(y1 - y0);

  ch.glyph = glyph;
  ch.width = w;
  ch.height = h;
  ch.advance = advance * scale;
  ch.offset_x = offsetX * scale;
  ch.offset_y = (float)y0;
  ch.scale = scale;
  ch.has_glyph = (w > 0 && h > 0 && stbtt_IsGlyphEmpty((stbtt_fontinfo*)m_font, glyph) == 0);

  return ch;
}


bool Font::get_image(const Font::Character& ch, Color* pixels) const {
  if (ch.has_glyph) {
    auto* src = (unsigned char*) pixels;
    stbtt_MakeGlyphBitmap((stbtt_fontinfo*)m_font, src, ch.width, ch.height, ch.width, ch.scale, ch.scale, ch.glyph);
    int len = ch.width * ch.height;
    for (int a = (len - 1) * 4, b = (len - 1); b >= 0; a -= 4, b -= 1) {
      src[a + 0] = src[b];
      src[a + 1] = src[b];
      src[a + 2] = src[b];
      src[a + 3] = src[b];
    }

    return true;
  }
  return false;
}


int Font::ascent() const {
  return m_ascent;
}

int Font::descent() const {
  return m_descent;
}

int Font::line_gap() const {
  return m_line_gap;
}

const String& Font::family_name() const {
  return m_family_name;
}

float Font::get_kerning(int glyph1, int glyph2, float scale) const {
  return stbtt_GetGlyphKernAdvance((stbtt_fontinfo*)m_font, glyph1, glyph2) * scale;
}
