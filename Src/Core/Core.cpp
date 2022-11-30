#include "Core.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
//
#include <stdlib.h>

#include <string>
/*
Holds all variables and configs for DctCore class
*/
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

namespace dct_core {
static std::vector<sf::Drawable*> DrawBuffer;
using namespace std;

wstring widen(const string& str) {
  wostringstream wstm;
  const ctype<wchar_t>& ctfacet = use_facet<ctype<wchar_t>>(wstm.getloc());
  for (size_t i = 0; i < str.size(); ++i) wstm << ctfacet.widen(str[i]);
  return wstm.str();
}

string narrow(const wstring& str) {
  ostringstream stm;

  // Incorrect code from the link
  // const ctype<char>& ctfacet = use_facet<ctype<char>>(stm.getloc());

  // Correct code.
  const ctype<wchar_t>& ctfacet = use_facet<ctype<wchar_t>>(stm.getloc());

  for (size_t i = 0; i < str.size(); ++i) stm << ctfacet.narrow(str[i], 0);
  return stm.str();
}

}  // namespace dct_core

dct_core::DctCore::DctCore(unsigned int window_width = 800,
                           unsigned int window_height = 600)
    : event(new sf::Event),
      MainWindow(new sf::RenderWindow(
          sf::VideoMode(window_width, window_height), "Your Dictionary")) {}


dct_core::DctCore::~DctCore() {
  if (MainWindow) {
    delete MainWindow;
  }
  if (event) {
    delete event;
  }
  if (Data) {
    delete Data;
  }
  CleanDrawBuffer();
}

void dct_core::DctCore::draw() {
  // for drawing text in loop
  for (int i = 0; i < (DrawBuffer.size()); i++) {
    MainWindow->draw(*(DrawBuffer[i]));
  }
}

void dct_core::DctCore::draw(sf::Drawable& dr) { MainWindow->draw(dr); }

void dct_core::DctCore::draw(sf::Text& text, unsigned int xp, unsigned int yp) 
{
  text.setPosition(xp, yp);
  MainWindow->draw(text);
}

void dct_core::DctCore::draw(sf::String str, sf::Font* font, sf::Color color,
                             int font_size, int xp, int yp) {
  sf::Text text;
  text.setFont(*font);
  text.setCharacterSize(font_size);
  text.setFillColor(color);
  text.setPosition(xp, yp);
  text.setString(str);
  MainWindow->draw(text);
}

void dct_core::DctCore::display() { MainWindow->display(); }

void dct_core::DctCore::clear() { MainWindow->clear(); }

void dct_core::DctCore::DrawInLoop(sf::String str, int xp, int yp) {
  int text_size = 20;
  sf::Color text_color = sf::Color::White;
  this->DrawInLoop(str, text_color, text_size, xp, yp);
}

void dct_core::DctCore::DrawInLoop(sf::String str, sf::Color text_color,
                                   int text_size, int xp, int yp) {
  // prevent draw same text on screen
  for (int i = 0; i < DrawBuffer.size(); i++) {
    sf::Text* pstr = dynamic_cast<sf::Text*>(DrawBuffer[i]);
    if (pstr) {
      std::wstring wstr_to_compare = pstr->getString();
      // if strings was the same won't add to draw buffer;
      if (!wstr_to_compare.compare(str)) {
        return;
      }
    }
  }
  sf::Text* text = new sf::Text;
  text->setFont(*fonts.GetRobotoSlabFont());
  text->setString(str);
  text->setCharacterSize(text_size);
  text->setFillColor(text_color);
  text->setPosition(sf::Vector2f(xp, yp));
  DrawBuffer.push_back(text);
}

void dct_core::DctCore::DrawInLoop(sf::Text *txt, sf::Color text_color, int xp, int yp) 
{
  // prevent draw same text on screen
  for (int i = 0; i < DrawBuffer.size(); i++) {
    sf::Text* pstr = dynamic_cast<sf::Text*>(DrawBuffer[i]);
    if (pstr) {
      std::wstring wstr_to_compare = pstr->getString();
      // if strings was the same won't add to draw buffer;
      if (!wstr_to_compare.compare(txt->getString())) {
        return;
      }
    }
  }
  sf::Text *text = new sf::Text;
  text->setCharacterSize(txt->getCharacterSize());
  text->setString(txt->getString());
  text->setFont(*fonts.GetRobotoSlabFont());
  text->setFillColor(text_color);
  text->setPosition(xp, yp);
  DrawBuffer.push_back(text);
}

void dct_core::DctCore::RemoveFromDrawBuffer(sf::String str) {
  std::vector<sf::Drawable*>::iterator ToEraseIterator = DrawBuffer.begin();
  for (uint32_t i = 0; i < DrawBuffer.size(); i++) {
    sf::Text* wstr_to_compare = dynamic_cast<sf::Text*>(DrawBuffer[i]);
    if (wstr_to_compare) {
      if (!std::wstring(wstr_to_compare->getString()).compare(str)) {
        delete DrawBuffer[i];
        DrawBuffer[i] = nullptr;
        DrawBuffer.erase(ToEraseIterator);
        return;
      }
    }
    ToEraseIterator++;
  }
}

void dct_core::DctCore::CleanDrawBuffer() {
  // remove data
  for (uint32_t i = 0; i < DrawBuffer.size(); i++) {
    delete DrawBuffer[i];
    DrawBuffer[i] = nullptr;
  }
  // clean all pointers
  DrawBuffer.clear();
}

sf::String dct_core::DctCore::GetStringMenu(sf::Font* font,
                                        sf::Color font_color, int fonst_size,
                                        float xp, float yp) 
{
  return GetStringMenu(font, L"", font_color, fonst_size, xp,
                   yp);
}

sf::String dct_core::DctCore::GetStringMenu(sf::Font *font, sf::String initial_str, sf::Color font_color, int fonst_size, float xp, float yp) {
  std::wstring result = initial_str;
  sf::Vector2f strat_pose(xp, yp);
  while (true) {
    while (MainWindow->pollEvent(*event)) {
      MainWindow->clear();
      switch (event->type) {
        case sf::Event::Closed: {
          MainWindow->close();
          exit(0);
        } break; // sf::Event::Closed:
        // handle input for user
        case sf::Event::TextEntered: {
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            return std::wstring(result);
          }
          // Backspace button pressed
          if (event->text.unicode == '\b' && result.size() > 0) {
            result.erase(result.size() - 1, 1);
          } else {
            result += event->text.unicode;
          }
        } break; // sf::Event::KeyPressed:
        default:
        break; // default:
      }  // switch (event->type)
    } 
    draw(result, font, font_color, fonst_size, xp, yp);
    draw();
    MainWindow->display();
    MainWindow->clear();
  }
  return sf::String();
}

// Just for testing
void dct_core::DctCore::MainLoop() {
  // draw words before main loop starts
  DrawInLoop("Yahari", sf::Color::Green, 16, 10, 10);
  DrawInLoop(L"Україна", sf::Color::Green, 16, 10, 30);
  DrawInLoop(L"Санчіз", sf::Color::Green, 16, 10, 50);
  DrawInLoop("Monanto", sf::Color::Green, 16, 10, 70);
  // LoadDictionary(L"dctionary.txt");
  // SaveDictionary(L"dctionary.txt");

  exit(0);

  // RemoveFromDrawBuffer(L"Monanto");
  // std::wstring StringFromUser = GetStringMenu();
  // DrawInLoop( StringFromUser,  sf::Color::Green, 16, 10, 50);

  while (MainWindow->isOpen()) {
    draw();
    draw(L"Кирил...", fonts.GetRobotoSlabFont(), sf::Color::Green, 16, 10, 90);
    /* for (size_t i = 0; i < dictionary.size(); i++) {
       draw(*(dictionary[i].translation), *fonts.GetRobotoSlabFont(),
            sf::Color::Green, 16, 120, 14 * i);
     }*/

    display();
    clear();
  }
}
