#pragma once
#ifndef DCT_CORE
#define DCT_CORE

#include "../fonts/Fonts.h"
// forvard declaration, used to minimize includes, see:
// https://blog.knatten.org/2012/11/30/how-to-avoid-includes-in-headers/

namespace sf {
class Color;
class Event;
class RenderWindow;
class Font;
class Drawable;
class String;
class Text;
}  // namespace sf
namespace dct_core {
struct MySFMLData;
class DctCore {
 private:
  MyFonts fonts;
  /* Holds all data in cpp struct */
  MySFMLData* Data;
 public:
  DctCore(unsigned int window_width, unsigned int window_height);
  ~DctCore();
  /* Handle all events */
  sf::Event* event;
  /*  Main SFML window pointer */
  sf::RenderWindow* MainWindow;
  /* Draw all sf::Drawable objects */
  virtual void draw();
  /* Draw certain sf::Drawable object */
  void draw(sf::Drawable& dr);
  /* Draw certain sf::Drawable object */
  void draw(sf::Text& text, unsigned int xp, unsigned int yp);
  /* Draw wstring object */
  void draw(sf::String str, sf::Font* font, sf::Color color, int font_size,
            int xp, int yp);
  /* Display already drawed */
  virtual void display();
  /* Clear display  */
  void clear();
  /* Copy wstring to DrawBuffer and draw it each loop iteration */
  void DrawInLoop(sf::String wstr, int xp, int yp);
  /* Copy wstring to DrawBuffer and draw it each loop iteration */
  void DrawInLoop(sf::String wstr, sf::Color text_color, int text_size,
                  int xp, int yp);
  /* Copy wstring to DrawBuffer and draw it each loop iteration */
  void DrawInLoop(sf::Text* text, sf::Color text_color, int xp, int yp);
  /* draw all words from Data->WordsToDraw buffer */
  void RemoveFromDrawBuffer(sf::String str);
  /* Clean static DrawBuffer data with all sf::Drawable objects */
  void CleanDrawBuffer();
  // Handle user text input
  sf::String GetStringMenu(sf::Font* font, sf::Color font_color, int fonst_size,
                       float xp, float yp);
  sf::String GetStringMenu(sf::Font* font, sf::String initial_str,
                       sf::Color font_color, int fonst_size, float xp,
                       float yp);
  /* Main loop should be overriden in children class */
  void MainLoop();
};
}  // namespace dct_core
#endif  // DCT_CORE