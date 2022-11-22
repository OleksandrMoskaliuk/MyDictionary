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
  }
  if (TrajanFont) {
    delete TrajanFont;
  }
  if (FuturaFont) {
    delete FuturaFont;
  }
  if (RobotoSlabFont) {
    delete RobotoSlabFont;
  }
}

sf::Font* MyFonts::GetBisternFont() {
  BisternFont = new sf::Font();
  if (!BisternFont->loadFromFile("fonts/bistern/Bistern.otf")) {
    std::cout << "BisternFont font wasn't found!\n";
    return nullptr;
  }
  return BisternFont;
}

sf::Font* MyFonts::GetTrajanFont() {
  TrajanFont = new sf::Font();
  if (!TrajanFont->loadFromFile("fonts/Trajan/Trajan Pro.ttf")) {
    std::cout << "TrajanFont font wasn't found!\n";
    return nullptr;
  }
  return TrajanFont;
}

sf::Font* MyFonts::GetFuturaFont() {
  FuturaFont = new sf::Font();
  if (!FuturaFont->loadFromFile("fonts/Futura/futur.ttf")) {
    std::cout << "FuturaFont font wasn't found!\n";
    return nullptr;
  }
  return FuturaFont;

}

sf::Font* MyFonts::GetRobotoSlabFont() {
  RobotoSlabFont = new sf::Font();
  if (!RobotoSlabFont->loadFromFile(
          "fonts/RobotoSlab/RobotoSlab-VariableFont_wght.ttf")) {
    std::cout << "RoboSlabFont wasn't found!\n";
    return nullptr;
  }
  return RobotoSlabFont;
}
