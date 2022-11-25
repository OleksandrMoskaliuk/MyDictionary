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
}  // namespace sf
namespace dct_core {
 struct Word { 
  sf::String *word;
  sf::String *translation;
  sf::String *category;
  sf::String *example;
}; // struct Word
struct MySFMLData;
class DctCore {
 private:
  sf::RenderWindow* MainWindow;
  MyFonts fonts;
  /* Holds all data in cpp struct */
  MySFMLData* Data;

 public:
  DctCore();
  ~DctCore();
  /* Handle all events */
  sf::Event* event;
  /* Draw all sf::Drawable objects */
  virtual void draw();
  /* Draw certain sf::Drawable object */
  void draw(sf::Drawable& dr);
  /* Draw wstring object */
  void draw(sf::String str, sf::Font& font, sf::Color color, int font_size,
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
  /* draw all words from Data->WordsToDraw buffer */
  void RemoveFromDrawBuffer(sf::String str);
  /* Clean static DrawBuffer data with all sf::Drawable objects */
  void CleanDrawBuffer();
  /* Get window pointer */
  sf::RenderWindow* GetWindow();
  // TODO: remove GetString()
  sf::String GetString();
  /* Main loop should be overriden in children class */
  virtual void MainLoop();
  /* Handle all events */
  virtual void EventsHandler();
  /* Read all words from saved *.txt file */
  bool LoadFromTxt(sf::String filename);
  /* Save dictionary in file */
  bool SaveInFile(sf::String filename);
};
}  // namespace dct_core
#endif  // DCT_CORE