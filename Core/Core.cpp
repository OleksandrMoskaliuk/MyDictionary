#include "Core.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
//
#include <stdlib.h>

#include <string>
#include <vector>
/*
Holds all variables and configs for DctCore class
*/
#include <Windows.h>

#include <iostream>
#include <locale>
#include <sstream>
#include <string>

namespace dct_core {
static std::vector<sf::Drawable*> DrawBuffer;
static std::vector<Word> dictionary;
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

struct dct_core::MySFMLData {
  /*
  Pull event pointer variable
  */
  MyFonts fonts;
  sf::Text WelcomeText;
  std::wstring PlayerInput;
  sf::CircleShape SimpleCircle;
  std::vector<sf::Text> WordsToDraw;
  /*
  Push your object to this array to draw
  */
  std::vector<sf::Drawable*>* RegisteredOblectToDraw;
  MySFMLData() : RegisteredOblectToDraw(new std::vector<sf::Drawable*>()) {
    SetUpWelcomeText();
    SetUpSimpleCircle();
  }
  ~MySFMLData() {}
  /*
  Fill up RegisteredOblectToDraw variable
  than main class will handle  drawing of al objects in this variable
  */
  void RegistrateOblectToDraw(sf::Drawable& ObjectToRegistrate) {
    RegisteredOblectToDraw->push_back(&ObjectToRegistrate);
  }
  void UnregistrateOblectToDraw(sf::Drawable& ObjectToUnregistrate) {
    std::vector<sf::Drawable*>::iterator ToEraseIterator;
    ToEraseIterator =
        std::find(this->RegisteredOblectToDraw->begin(),
                  this->RegisteredOblectToDraw->end(), &ObjectToUnregistrate);
    this->RegisteredOblectToDraw->erase(ToEraseIterator);
  }

 private:
  void SetUpWelcomeText() {
    // select the font
    WelcomeText.setFont(*fonts.GetRobotoSlabFont());  // font is a sf::Font
    // set the wstring to display
    WelcomeText.setString("");
    // set the character size
    WelcomeText.setCharacterSize(20);  // in pixels, not points!
    // set the color
    WelcomeText.setFillColor(sf::Color::Green);
    // set the text style
    WelcomeText.setStyle(sf::Text::Bold);
    WelcomeText.setPosition(sf::Vector2f(0, 0));
    RegistrateOblectToDraw(WelcomeText);
  }
  void SetUpSimpleCircle() {
    SimpleCircle = sf::CircleShape(10.f, 10.f);
    SimpleCircle.setFillColor(sf::Color::Green);
    SimpleCircle.setPosition(sf::Vector2f(20.f, 20.f));
    this->RegistrateOblectToDraw(SimpleCircle);
  }

};  // struct MySFMLData

dct_core::DctCore::DctCore()
    : MainWindow(new sf::RenderWindow(sf::VideoMode(800, 600), "SFML works!")),
      event(new sf::Event),
      Data(new MySFMLData()) {}

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
  // clear dictionary vefore exit
  for (Word wd : dictionary) {
    delete wd.word;
    delete wd.category;
    delete wd.translation;
    delete wd.example;
  }
  dictionary.clear();
}

void dct_core::DctCore::draw() {
  // for drawing text in loop
  for (int i = 0; i < (DrawBuffer.size()); i++) {
    MainWindow->draw(*(DrawBuffer[i]));
  }
}

void dct_core::DctCore::draw(sf::Drawable& dr) { MainWindow->draw(dr); }

void dct_core::DctCore::draw(sf::String str, sf::Font& font, sf::Color color,
                             int font_size, int xp, int yp) {
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(font_size);
  text.setFillColor(color);
  text.setPosition(xp, yp);
  text.setString(str);
  MainWindow->draw(text);
}

void dct_core::DctCore::display() { MainWindow->display(); }

void dct_core::DctCore::clear() { MainWindow->clear(); }

void dct_core::DctCore::EventsHandler() {
  while (MainWindow->pollEvent(*event)) {
    switch (event->type) {
      default: {
      } break;
      /* close main window */
      case sf::Event::Closed: {
        MainWindow->close();
      } break;
      // handle text input for player
      case sf::Event::TextEntered: {
        break;  // do not use this case only for example
        if (event->text.unicode == '\b' && Data->PlayerInput.size() > 0) {
          Data->PlayerInput.erase(Data->PlayerInput.size() - 1, 1);
        } else {
          Data->PlayerInput += event->text.unicode;
        }
        Data->WelcomeText.setString(Data->PlayerInput);
      } break;
    }  // switch (event->type)
  }    // while (Data->MainWindow->pollEvent(*event))
}

void dct_core::DctCore::DrawInLoop(sf::String str, int xp, int yp) {
  int text_size = 20;
  sf::Color text_color = sf::Color::White;
  this->DrawInLoop(str, text_color, text_size, xp, yp);
}

void dct_core::DctCore::DrawInLoop(sf::String str, sf::Color text_color,
                                   int text_size, int xp, int yp) {
  sf::Text* text = new sf::Text;
  // This for loop for preventing same text on the screen
  for (int i = 0; i < DrawBuffer.size(); i++) {
    sf::Text* pstr = dynamic_cast<sf::Text*>(DrawBuffer[i]);
    if (pstr) {
      std::cout << "dynamic cast works!\n";
      std::wstring wstr_to_compare = pstr->getString();
      // if strings was the same won't add to draw buffer;
      if (!wstr_to_compare.compare(str)) {
        return;
      }
    }
  }

  text->setFont(*fonts.GetRobotoSlabFont());
  text->setString(str);
  text->setCharacterSize(text_size);
  text->setFillColor(text_color);
  text->setPosition(sf::Vector2f(xp, yp));
  DrawBuffer.push_back(text);
}

void dct_core::DctCore::RemoveFromDrawBuffer(sf::String str) {
  std::vector<sf::Drawable*>::iterator ToEraseIterator = DrawBuffer.begin();
  for (uint32_t i = 0; i < DrawBuffer.size(); i++) {
    sf::Text* wstr_to_compare = dynamic_cast<sf::Text*>(DrawBuffer[i]);
    if (wstr_to_compare) {
      if (!std::wstring(wstr_to_compare->getString()).compare(str)) {
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
  }
  // clean all pointers
  DrawBuffer.clear();
}

sf::RenderWindow* dct_core::DctCore::GetWindow() { return MainWindow; }

sf::String dct_core::DctCore::GetString() {
  std::wstring reuslt_str;
  sf::Vector2f window_center = sf::Vector2f(400.f, 300.f);
  Data->WelcomeText.setPosition(window_center);
  while (true) {
    while (MainWindow->pollEvent(*event)) {
      MainWindow->clear();
      switch (event->type) {
        default: {
        } break;
        case sf::Event::Closed: {
          MainWindow->close();
        } break;
        // handle input for user
        case sf::Event::TextEntered: {
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            return std::wstring(reuslt_str);
            break;
          }
          // Backspace button pressed
          if (event->text.unicode == '\b' && reuslt_str.size() > 0) {
            reuslt_str.erase(reuslt_str.size() - 1, 1);
          } else {
            reuslt_str += event->text.unicode;
          }
          // place text exactly on center of window
          Data->WelcomeText.setString(reuslt_str);
          sf::Vector2f new_origin(
              (Data->WelcomeText.getCharacterSize() * reuslt_str.size() / 4),
              0.f);
          Data->WelcomeText.setOrigin(new_origin);
        } break;
      }  // switch (event->type)
      draw();
      MainWindow->draw(Data->WelcomeText);
      MainWindow->display();
    }  // while (Data->MainWindow->pollEvent(*event))
  }
  return sf::String();
}

bool dct_core::DctCore::LoadFromTxt(sf::String filename) {
  /* Shold be set before reading translation field*/
  setlocale(LC_CTYPE, "ukr");
  FILE* pFile;
  fopen_s(&pFile, "dctionary.txt", "r");
  // find word after key
  auto find_word = [](std::wstring key, wchar_t* wchar, uint8_t size) {
    std::wstring lb_wbuffer;
    for (uint8_t i = 0; i < size; i++) {
      lb_wbuffer += wchar[i];
      if (wchar[i] == L' ') {
        lb_wbuffer.clear();
      }
      if (key.compare(lb_wbuffer) == 0) {
        lb_wbuffer.clear();
        i+=2;  // past space
        while (wchar[i] != L'\n' || i > size - 1) {
          lb_wbuffer += wchar[i];
          i++;
        }
        return lb_wbuffer;
      }
    }
    return std::wstring();
  };
  if (pFile) {
    wchar_t wcharb[100];
    std::wstring word;
    std::wstring translation;
    std::wstring category;
    std::wstring example;
    // Get line, while it is
    while (fgetws(wcharb, 100, pFile)) {
        fgetws(wcharb, 100, pFile);
        word = find_word(L"word:", wcharb, 100);
        fgetws(wcharb, 100, pFile);
        translation = find_word(L"translation:", wcharb, 100);
        fgetws(wcharb, 100, pFile);
        category = find_word(L"category:", wcharb, 100);
        fgetws(wcharb, 100, pFile);
        example = find_word(L"examples:", wcharb, 100);
        Word new_word;
        new_word.word = new sf::String(word);
        new_word.translation = new sf::String(translation);
        new_word.category = new sf::String(category);
        new_word.example = new sf::String(example);
        dictionary.push_back(new_word);
        word.clear();
        translation.clear();
        category.clear();
        example.clear();
    }
  } else {
    std::cout << "failed to open: " << std::string(filename).c_str() << '\n';
    return false;
  }
  fclose(pFile);
  return true;
}

bool dct_core::DctCore::SaveInFile(sf::String filename) {
  setlocale(LC_CTYPE, "ukr");
  FILE* pFile;
  fopen_s(&pFile, "dctionary.txt", "w");

  if (pFile) {
    // Lambda to make code shorter
    auto writer = [&](std::wstring field, sf::String* data, int& counter1,
                      int& counter2) {

            std::wstring to_write (std::to_wstring(counter1) + L'.' +
                      std::to_wstring(counter2) + field +
                      std::wstring(*data) + L'\n');
      fputws(to_write.c_str(), pFile);
    };
    for (int i = 0; i < dictionary.size(); i++) {
     int j = 0;
     std::wstring first_lie(std::to_wstring(i) + L'.' + L"\n");
     fputws(first_lie.c_str(), pFile);
     writer(L" word: ", dictionary[i].word, i, j);
     ++j;
     writer(L" translation: ", dictionary[i].translation, i, j);
     ++j;
     writer(L" category: ", dictionary[i].category, i, j);
     ++j;
     writer(L" examples: ", dictionary[i].example, i, j);
     ++j;
    }
    fclose(pFile);
    return true;
  } else {
    std::cout << "Failed to save \n";
    return false;
  }
}

void dct_core::DctCore::MainLoop() {
  // draw words before main loop starts
  DrawInLoop("Yahari", sf::Color::Green, 16, 10, 10);
  DrawInLoop(L"������", sf::Color::Green, 16, 10, 30);
  DrawInLoop(L"������", sf::Color::Green, 16, 10, 50);
  DrawInLoop("Monanto", sf::Color::Green, 16, 10, 70);
  LoadFromTxt(L"dctionary.txt");
  SaveInFile(L"dctionary.txt");
  exit(0);
  // RemoveFromDrawBuffer(L"Monanto");
  // std::wstring StringFromUser = GetString();
  // DrawInLoop( StringFromUser,  sf::Color::Green, 16, 10, 50);
  while (MainWindow->isOpen()) {
    EventsHandler();
    draw();
    draw(L"�����...", *fonts.GetRobotoSlabFont(), sf::Color::Green, 16, 10, 90);
    /*for (size_t i = 0; i < dictionary.size(); i++) {
      draw(*(dictionary[i].translation), *fonts.GetRobotoSlabFont(),
           sf::Color::Green, 16, 120, 14 * i);
    }*/

    display();
    clear();
  }
}
