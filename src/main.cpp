#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>


#include "font.h"

using namespace FB;


int Kerning = 3;
int Meta = 4;
int CharKK = 5;
int GlyphKK = 6;
int ColorKK = 7;

template <typename T> struct GlyphMap {
  int glyph;
  T data;
};

struct Character {
  int glyph;
  int width;
  int height;
  float advance;
  float offset_x;
  float offset_y;
  float scale;
  int has_glyph;
};

struct Metadata {
  int scale;
  int descent;
  int ascent;
  int line_gap;
  String family_name;
};
/*
  GlyphMap<int> glyphs;
  GlyphMap<Character> characters;
  GlyphMap<GlyphMap<int>> kernings;
  GlyphMap<Color*> images;
  */



int main(int argc, char* argv[]) {


  int* bufff = (int*)malloc(1024 * 1024 * 20);
  int length = 0;

  
  int* glyphs = (int*)malloc(1024 * 1024);
  int g_length = 0;


  if (argc != 3) {
    printf("Usage: font-file.ttf outfolder");
    return 1;
  } else {
    char* font_path = argv[1];
    char* folder = argv[2];

    std::string str;

    str.append(folder);
    str.append("/out.data");

    float size = 64;


    Font* font = Font::create(font_path);

    float scale = font->get_scale(size);

    for (auto i = 32; i < 127; i++) {

      auto glyph = font->get_glyph(i);
      if (glyph <= 0)
        continue;


      glyphs[g_length++] = glyph;


      auto g1 = glyph;
      memcpy(&bufff[length], &GlyphKK, sizeof(GlyphKK));
      length += sizeof(GlyphKK);
      memcpy(&bufff[length], &g1, sizeof(g1));
      length += sizeof(g1);
      memcpy(&bufff[length], &i, sizeof(i));
      length += sizeof(i);



      auto ch = font->get_character(glyph, scale);

      if (ch.has_glyph) {
        Color* buffer = (Color*)malloc(sizeof(struct Color) * ch.width * ch.height);
        if (font->get_image(ch, buffer)) {

          
          int ll = sizeof(struct Color) * ch.width * ch.height;

          auto g1 = glyph;
          memcpy(&bufff[length], &ColorKK, sizeof(ColorKK));
          length += sizeof(ColorKK);
          memcpy(&bufff[length], &g1, sizeof(g1));
          length += sizeof(g1);
          memcpy(&bufff[length], &ch.width, sizeof(ch.width));
          length += sizeof(ch.width);
          memcpy(&bufff[length], &ch.height, sizeof(ch.height));
          length += sizeof(ch.height);
          memcpy(&bufff[length], &ll, sizeof(ll));
          length += sizeof(ll);
          memcpy(&bufff[length], buffer, ll);
          length += ll;
        }
      }

      memcpy(&bufff[length], &CharKK, sizeof(CharKK));
      length += sizeof(CharKK);
      memcpy(&bufff[length], &glyph, sizeof(glyph));
      length += sizeof(glyph);
      memcpy(&bufff[length], &ch, sizeof(ch));
      length += sizeof(ch);
    }

    for (auto i = 161; i < 255; i++) {

      auto glyph = font->get_glyph(i);
      if (glyph <= 0)
        continue;

      glyphs[g_length++] = glyph;

      auto g1 = glyph;
      memcpy(&bufff[length], &GlyphKK, sizeof(GlyphKK));
      length += sizeof(GlyphKK);
      memcpy(&bufff[length], &g1, sizeof(g1));
      length += sizeof(g1);
      memcpy(&bufff[length], &i, sizeof(i));
      length += sizeof(i);




      auto ch = font->get_character(glyph, scale);
      if (ch.has_glyph) {
        Color* buffer = (Color*)malloc(length);
        if (font->get_image(ch, buffer)) {
          
          int ll = sizeof(struct Color) * ch.width * ch.height;

          auto g1 = glyph;
          memcpy(&bufff[length], &ColorKK, sizeof(ColorKK));
          length += sizeof(ColorKK);
          memcpy(&bufff[length], &g1, sizeof(g1));
          length += sizeof(g1);
          memcpy(&bufff[length], &ch.width, sizeof(ch.width));
          length += sizeof(ch.width);
          memcpy(&bufff[length], &ch.height, sizeof(ch.height));
          length += sizeof(ch.height);
          memcpy(&bufff[length], &ll, sizeof(ll));
          length += sizeof(ll);
          memcpy(&bufff[length], buffer, ll);
          length += ll;
        }
      }

      memcpy(&bufff[length], &CharKK, sizeof(CharKK));
      length += sizeof(CharKK);
      memcpy(&bufff[length], &glyph, sizeof(glyph));
      length += sizeof(glyph);
      memcpy(&bufff[length], &ch, sizeof(ch));
      length += sizeof(ch);

    }

    for (auto i = 0; i < g_length; i++) {
      auto g1 = glyphs[i];


      for (auto j = 0; j < g_length; j++) {
        auto g2 = glyphs[j];

        if (i == j) continue;


        float kerning = font->get_kerning(g1, g2, scale);

        if (kerning == 0) continue;

        memcpy(&bufff[length], &Kerning, sizeof(Kerning));
        length += sizeof(Kerning);
        memcpy(&bufff[length], &g1, sizeof(g1));
        length += sizeof(g1);
        memcpy(&bufff[length], &g2, sizeof(g2));
        length += sizeof(g2);
        memcpy(&bufff[length], &kerning, sizeof(kerning));
        length += sizeof(kerning);
      }
    }


    scale;
    auto descent = font->descent();
    auto ascent = font->ascent();
    auto line_gap = font->line_gap();
    auto name = font->family_name();


    memcpy(&bufff[length], &Meta, sizeof(Meta));
    length += sizeof(Meta);

    memcpy(&bufff[length], &scale, sizeof(scale));
    length += sizeof(scale);
    memcpy(&bufff[length], &descent, sizeof(descent));
    length += sizeof(descent);
    memcpy(&bufff[length], &ascent, sizeof(ascent));
    length += sizeof(ascent);
    memcpy(&bufff[length], &line_gap, sizeof(line_gap));
    length += sizeof(line_gap);
    memcpy(&bufff[length], &name.length, sizeof(name.length));
    length += sizeof(name.length);
    memcpy(&bufff[length], name.buffer, sizeof(char) * name.length);
    length += sizeof(char) * name.length;

  auto file = Platform::file_open(str.c_str(), FileMode::CreateWrite);

  file->write(bufff, length * sizeof(int));

  printf("Done.\n");
  }




  return 0;
}
