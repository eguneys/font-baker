#pragma once
#include "common.h"


namespace FB {


  class Font {

    private:
      Font() = default;

    public:
      using Codepoint = u32;

      struct Character {

        int glyph = 0;
        int width = 0;
        int height = 0;
        float advance = 0;
        float offset_x = 0;
        float offset_y = 0;
        float scale = 0;
        bool has_glyph = false;
      };

      static Font* create(const FilePath& path);


      const String& family_name() const;
      const String& style_name() const;

      int ascent() const;
      int descent() const;
      int line_gap() const;
      int height() const;
      int line_height() const;

      int get_glyph(Codepoint codepoint) const;

      float get_scale(float size) const;
      float get_kerning(int glyph1, int glyph2, float scale) const;

      Character get_character(int glyph, float scale) const;

      bool get_image(const Character& ch, Color* pixels) const;

      Image get_image(const Character& ch) const;


    private:
      void* m_font;
      u8* m_buffer;
      String m_family_name;
      String m_style_name;
      int m_ascent = 0;
      int m_descent = 0;
      int m_line_gap = 0;
  };

}
