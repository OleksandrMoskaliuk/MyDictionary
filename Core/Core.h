#pragma once
#ifndef DCT_CORE
#define DCT_CORE

#include "../fonts/Fonts.h"
#include <string>
// forvard declaration, used to minimize includes, see:
// https://blog.knatten.org/2012/11/30/how-to-avoid-includes-in-headers/

namespace sf {
class Color;
class Event;
class RenderWindow;
class Font;
class Drawable;
}  // namespace sf

namespace dct_core {
struct MySFMLData;
class DctCore {
 private:
  sf::RenderWindow* MainWindow;
  sf::Event* event;
  MyFonts fonts;
  /* Holds all data in cpp struct */
  MySFMLData* Data;
 public:
  DctCore();
  ~DctCore();
  virtual void draw();
  void draw(sf::Drawable& dr);
  void draw(std::wstring& str, sf::Font& font, sf::Color color,
                    int font_size, uint32_t xp, uint32_t yp);
  virtual void display();
  void clear();
  void DrawInLoop(std::wstring Text, int xp, int yp);
  void DrawInLoop(std::wstring str, sf::Color TextColor,
 int TextSize,

 int xp,
                        int yp);
  /* draw all words from Data->WordsToDraw buffer */
  void RemoveWordByName(std::wstring WordToRemove);
  void CleanAllWords();
  /* Get event poiner */
  sf::Event* GetEvent();
  /* Get window pointer */
  sf::RenderWindow* GetWindow();
  std::wstring GetString();
  /* Main loop should be overriden in children class */
  virtual void MainLoop();

 private:
  /* Draw object from inner data struct */
  void DrawAndDisplay();
  /* Handle all events */
  void EventsHandler();
};
}  // namespace dct_core
#endif  // DCT_CORE