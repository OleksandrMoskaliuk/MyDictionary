#include "Fonts.h"

#include <SFML/Graphics.hpp>
#include <iostream>

MyFonts::MyFonts()
    : BisternFont(nullptr),
      TrajanFont(nullptr),
      FuturaFont(nullptr),
      RobotoSlabFont(nullptr) {}

MyFonts::~MyFonts() {
  if (BisternFont) {
    delete BisternFont;
    BisternFont = nullptr;
  }
  if (TrajanFont) {
    delete TrajanFont;
    TrajanFont = nullptr;
  }
  if (FuturaFont) {
    delete FuturaFont;
    FuturaFont = nullptr;
  }
  if (RobotoSlabFont) {
    delete RobotoSlabFont;
    RobotoSlabFont = nullptr;
  }
}

sf::Font* MyFonts::GetBisternFont() {
  /* If already created */
  if (BisternFont) {
    return BisternFont;
  }
  BisternFont = new sf::Font();
  if (!BisternFont->loadFromFile("fonts/bistern/Bistern.otf")) {
    std::cout << "BisternFont font wasn't found!\n";
    return nullptr;
  }
  return BisternFont;
}

sf::Font* MyFonts::GetTrajanFont() {
  /* If already created */
  if (TrajanFont) {
    return TrajanFont;
  }
  TrajanFont = new sf::Font();
  if (!TrajanFont->loadFromFile("fonts/Trajan/Trajan Pro.ttf")) {
    std::cout << "TrajanFont font wasn't found!\n";
    return nullptr;
  }
  return TrajanFont;
}

sf::Font* MyFonts::GetFuturaFont() {
  /* If already created */
  if (FuturaFont) {
    return FuturaFont;
  }
  FuturaFont = new sf::Font();
  if (!FuturaFont->loadFromFile("fonts/Futura/futur.ttf")) {
    std::cout << "FuturaFont font wasn't found!\n";
    return nullptr;
  }
  return FuturaFont;

}

sf::Font* MyFonts::GetRobotoSlabFont() {
  /* If already created */
  if (RobotoSlabFont) {
    return RobotoSlabFont;
  }
  /* if first time creating */
  RobotoSlabFont = new sf::Font();
  if (!RobotoSlabFont->loadFromFile(
          "fonts/RobotoSlab/RobotoSlab-VariableFont_wght.ttf")) {
    std::cout << "RoboSlabFont wasn't found!\n";
    return nullptr;
  }
  return RobotoSlabFont;
}
