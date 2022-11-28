#include "Dct.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

namespace dct {
MyFonts Fonts;
std::vector<Word> Dictionary;
std::vector<sf::Text*> Menu;
const float WordsYSpace = 1.2;
}  // namespace dct

dct::Dct::Dct() : DctCore::DctCore(800, 600), Cursor(new sf::CircleShape()) {
  // Load Dictionary
  LoadDictionary("dictionary.txt");
  // Make Menu
  Menu.push_back(
      new sf::Text(L"SHOW DICTIONARY", *Fonts.GetFuturaFont(), 16));  // case 0
  Menu.push_back(
      new sf::Text(L"ADD WORD", *Fonts.GetFuturaFont(), 16));  // case 1
  Menu.push_back(
      new sf::Text(L"REMOVE WORD", *Fonts.GetFuturaFont(), 16));  // case 2
  Menu.push_back(
      new sf::Text(L"EDIT WORD", *Fonts.GetFuturaFont(), 16));  // case 3
  Menu.push_back(
      new sf::Text(L"TEST YOURSELF", *Fonts.GetFuturaFont(), 16));    // case 4
  Menu.push_back(new sf::Text(L"SAVE", *Fonts.GetFuturaFont(), 16));  // case 5
  Menu.push_back(
      new sf::Text(L"SAVE AND EXIT", *Fonts.GetFuturaFont(), 16));    // case 6
  Menu.push_back(new sf::Text(L"EXIT", *Fonts.GetFuturaFont(), 16));  // case 7
  // Setup cursor Circle Shape
  Cursor->setRadius(6.f);
  Cursor->setPosition(0.f, 0.f);
  Cursor->setFillColor(sf::Color::Green);
}

dct::Dct::~Dct() {
  if (Cursor) {
    delete Cursor;
    Cursor = nullptr;
  }
  // Clear dictionary before exit
  for (Word wd : Dictionary) {
    delete wd.word;
    delete wd.category;
    delete wd.translation;
    delete wd.example;
  }
  Dictionary.clear();
  // Clear Main Menu  
  for (sf::Text* tx : Menu) {
    delete tx;
    tx = nullptr;
  }
  Menu.clear();
}

void dct::Dct::Start() {
  while (MainWindow->isOpen()) {
    EventsHandler();
    // Place main menu in center of screen
    for (uint8_t i = 0; i < Menu.size(); i++) {
      Menu[i]->setFillColor(sf::Color::Green);
      float x = (MainWindow->getSize().x / 2.5);
      float ScreenYCenter = MainWindow->getSize().y / 3.2;
      float y = (i * Menu[i]->getCharacterSize() * WordsYSpace) + ScreenYCenter;
      draw(*Menu[i],x,y);
    }
    // Cursor position adjustments on first start
    if (Cursor->getPosition().x == 0.f && Cursor->getPosition().x == 0.f &&
        Menu[0] != nullptr ) {
      sf::Vector2f current = Menu[MenuCounter]->getPosition();
      // adjustments
      current.x -= 14;
      current.y += 3.5;
      Cursor->setPosition(current);
    }
    draw(*Cursor);
    draw();
    display();
    clear();
  }
}

void dct::Dct::EventsHandler() {
  // parent event handle method for closing windows
  while (MainWindow->pollEvent(*event)) {
    switch (event->type) {
        // Close main window event
      case sf::Event::Closed: {
        MainWindow->close();
      } break;

      // handle text input for player
      case sf::Event::TextEntered: {
        // do smth
      } break;  // do not use this case only for example

      case sf::Event::KeyPressed: {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
          if (Menu.size() > 0 && Menu[MenuCounter]) {
            MenuCounter++;
            if (MenuCounter > Menu.size() - 1) {
              MenuCounter = 0;
            }
            sf::Vector2f current = Menu[MenuCounter]->getPosition();
            // adjustments
            current.x -= 14;
            current.y += 3.5;
            Cursor->setPosition(current);
          }
        } // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
          if (Menu.size() > 0 && Menu[MenuCounter]) {
            if (MenuCounter == 0) {
              MenuCounter = Menu.size() - 1;
            } else {
              MenuCounter--;
            }
            sf::Vector2f current = Menu[MenuCounter]->getPosition();
            // adjustments
            current.x -= 14;
            current.y += 3.5;
            Cursor->setPosition(current);
          }
        } // (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          if (Menu[MenuCounter]) {
            std::wstring action = Menu[MenuCounter]->getString();
            if (!action.compare(L"SHOW DICTIONARY")) {
              ShowDictionary();
            }
            if (!action.compare(L"ADD WORD")) {
            }
            if (!action.compare(L"REMOVE WORD")) {
            }
            if (!action.compare(L"EDIT WORD")) {
            }
            if (!action.compare(L"TEST YOURSELF")) {
            }
            if (!action.compare(L"SAVE")) {
            }
            if (!action.compare(L"SAVE AND EXIT")) {
            }
            if (!action.compare(L"EXIT")) {
            }
          }
        } //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
      } break; // Case sf::KeyPressed
      default: {
      } break;
    }  // switch (event->type)
  }    // while (Data->MainWindow->pollEvent(*event))
}

void dct::Dct::ShowDictionary() 
{
 // Remove unnecessaty drawable object from buffer
 CleanDrawBuffer();
 uint8_t min_words_to_display = 6;
 while (MainWindow->isOpen()) {
 
  while (MainWindow->pollEvent(*event)) {
    switch (event->type) {
        // Close main window event
      case sf::Event::Closed: {
        MainWindow->close();
      } break;
    }
  }
  draw();
  display();
  clear();
 }
}

bool dct::Dct::IsInDictionary(sf::String new_word) {
  for (int i = 0; i < Dictionary.size(); i++) {
    if (!std::wstring(*Dictionary[i].word).compare(std::wstring(new_word)))
      return true;
  }
  return false;
}

bool dct::Dct::LoadDictionary(sf::String filename) {
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
        i += 2;  // past space
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
      example = find_word(L"examples:", wcharb, 100);
      Word new_word;
      new_word.word = new sf::String(word);
      new_word.translation = new sf::String(translation);
      new_word.category = new sf::String(category);
      new_word.example = new sf::String(example);
      Dictionary.push_back(new_word);
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

bool dct::Dct::SaveDictionary(sf::String filename) {
  setlocale(LC_CTYPE, "ukr");
  FILE* pFile;
  fopen_s(&pFile, "dctionary.txt", "w");

  if (pFile) {
    // Lambda to make code shorter
    auto writer = [&](std::wstring field, sf::String* data, int& counter1,
                      int& counter2) {
      std::wstring to_write(std::to_wstring(counter1) + L'.' +
                            std::to_wstring(counter2) + field +
                            std::wstring(*data) + L'\n');
      fputws(to_write.c_str(), pFile);
    };
    for (int i = 0; i < Dictionary.size(); i++) {
      int j = 0;
      std::wstring first_lie(std::to_wstring(i) + L'.' + L"\n");
      fputws(first_lie.c_str(), pFile);
      writer(L" word: ", Dictionary[i].word, i, j);
      ++j;
      writer(L" translation: ", Dictionary[i].translation, i, j);
      ++j;
      writer(L" examples: ", Dictionary[i].example, i, j);
      ++j;
    }
    fclose(pFile);
    return true;
  } else {
    std::cout << "Failed to save \n";
    return false;
  }
}