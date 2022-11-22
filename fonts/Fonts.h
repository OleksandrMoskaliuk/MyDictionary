#pragma once

namespace sf {
class Font;
}

class MyFonts {
 public:
  MyFonts();
  ~MyFonts();
  sf::Font* GetBisternFont();
  sf::Font* GetTrajanFont();
  sf::Font* GetFuturaFont();
  sf::Font* GetRobotoSlabFont();

 private:
  sf::Font* BisternFont;
  sf::Font* TrajanFont;
  sf::Font* FuturaFont;
  sf::Font* RobotoSlabFont;
};
