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
      draw(*Menu[i], x, y);
    }
    // Cursor position adjustments on first start
    if (Cursor->getPosition().x == 0.f && Cursor->getPosition().x == 0.f &&
        Menu[0] != nullptr) {
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
        }  // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
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
        }  // (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          if (Menu[MenuCounter]) {
            std::wstring action = Menu[MenuCounter]->getString();
            if (!action.compare(L"SHOW DICTIONARY")) {
              // Backup fore some variables
              sf::Vector2f last_cursor_position = Cursor->getPosition();
              int last_menu_counter = MenuCounter;
              ShowDictionary();
              // Set bach to previous values
              Cursor->setPosition(last_cursor_position);
              MenuCounter = last_menu_counter;
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
              SaveDictionary("dictionary.txt");
            }
            if (!action.compare(L"SAVE AND EXIT")) {
              SaveDictionary("dictionary.txt");
              exit(0);
            }
            if (!action.compare(L"EXIT")) {
              exit(0);
            }
          }
        }       // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
      } break;  // Case sf::KeyPressed
      default: {
      } break;
    }  // switch (event->type)
  }    // while (Data->MainWindow->pollEvent(*event))
}

void dct::Dct::ShowDictionary() {
  // Remove unnecessaty drawable object from buffer
  if (Dictionary.size() == 0) {
    return;
  }
  CleanDrawBuffer();
  // Find longest word
  int longest_word = 0;
  for (size_t i = 0; i < Dictionary.size(); i++) {
    if (longest_word < Dictionary[i].word->getString().getSize()) {
      longest_word = Dictionary[i].word->getString().getSize();
    }
  }
  // Find max font size
  int max_character_size = 0;
  for (size_t i = 0; i < Dictionary.size(); i++) {
    if (max_character_size < Dictionary[i].word->getCharacterSize()) {
      max_character_size = Dictionary[i].word->getString().getSize();
    }
  }
  // Set cursor pose based on output words and MainWindow
  sf::Vector2f cursor_static_pose;
  cursor_static_pose.x = (MainWindow->getSize().x / 2) - (longest_word * max_character_size);
  cursor_static_pose.y = (MainWindow->getSize().y / 2) - max_character_size;
  Cursor->setPosition(cursor_static_pose);
  // How many words to display
  uint8_t words_to_display = 6;
  MenuCounter = 0;
  auto up_and_down_action = [&]() {
    if (Dictionary.size() > 0 && Dictionary[MenuCounter].word &&
        Dictionary[MenuCounter].translation) {
      // Make a new draw buffer
      CleanDrawBuffer();
      // bottom words part
      for (int8_t i = 0; i < words_to_display; i++) {
        if (MenuCounter + i < Dictionary.size() - 1 &&
            Dictionary[MenuCounter + i].word) {
          DrawInLoop(Dictionary[MenuCounter + i].word, sf::Color::Green,
                     14 + cursor_static_pose.x,
                     i * max_character_size * WordsYSpace +
                         cursor_static_pose.y - 3.5);
          DrawInLoop(Dictionary[MenuCounter + i].translation, sf::Color::Green,
               cursor_static_pose.x + (longest_word * max_character_size),
                     i * max_character_size * WordsYSpace +
                         cursor_static_pose.y - 3.5);
        }
      }
      // top words part
      for (int8_t i = 0; i < words_to_display - 1; i++) {
        if (MenuCounter - i >= 0 && Dictionary[MenuCounter - i].word) {
          DrawInLoop(Dictionary[MenuCounter - i].word, sf::Color::Green,
                     14 + cursor_static_pose.x,
                     -i * max_character_size * WordsYSpace +
                         cursor_static_pose.y - 3.5);
          DrawInLoop(Dictionary[MenuCounter - i].translation, sf::Color::Green,
                     cursor_static_pose.x + (longest_word * max_character_size),
                     -i * max_character_size * WordsYSpace +
                         cursor_static_pose.y - 3.5);
        }
      }
      if (Dictionary.size() && Dictionary[MenuCounter].example) {
        DrawInLoop(L"Example:", sf::Color::Green,18, 0.f, 0.f);
        if (!std::wstring(Dictionary[MenuCounter].example->getString()).compare(L""))
        {
          DrawInLoop(L"<empty>", sf::Color::Red,16, 0.f,
                     20.f);
        }else
        {
        DrawInLoop(Dictionary[MenuCounter].example,sf::Color::Green,0.f,20.f);
        }
      }
    }
  };
  up_and_down_action();
  while (MainWindow->isOpen()) {
    while (MainWindow->pollEvent(*event)) {
      switch (event->type) {
          // Close main window event
        case sf::Event::Closed: {
          MainWindow->close();
        } break;
        case sf::Event::KeyPressed: {
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            MenuCounter++;
            if (MenuCounter > Dictionary.size() - 1) {
              MenuCounter = 0;
            }
            up_and_down_action();

          }  // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (MenuCounter == 0) {
              MenuCounter = Dictionary.size() - 1;
            } else {
              MenuCounter--;
            }
            up_and_down_action();

          }  // (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            /*std::cout << "MenuCounter = " << MenuCounter << std::endl;
               std::wcout << L"Dictuionary[MenuCounter] = "
                          << std::wstring(
                                 Dictionary[MenuCounter].word->getString())
                          << std::endl;*/
          }  // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            CleanDrawBuffer();
            return;
          }  // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        }
      }
      draw(*Cursor);
      draw();
      display();
      clear();
    }
  }
}

bool dct::Dct::IsInDictionary(sf::String new_word) {
  for (int i = 0; i < Dictionary.size(); i++) {
    if (!std::wstring(Dictionary[i].word->getString())
             .compare(std::wstring(new_word)))
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
      new_word.word = new sf::Text(word, *Fonts.GetBisternFont(), 16u);
      new_word.translation =
          new sf::Text(translation, *Fonts.GetBisternFont(), 16u);
      new_word.category = new sf::Text(category, *Fonts.GetBisternFont(), 16u);
      new_word.example = new sf::Text(example, *Fonts.GetBisternFont(), 16u);
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

bool const dct::Dct::SaveDictionary(sf::String filename) const {
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
      sf::String data;
      int j = 0;
      std::wstring first_lie(std::to_wstring(i) + L'.' + L"\n");
      fputws(first_lie.c_str(), pFile);
      data = (Dictionary[i].word->getString());
      writer(L" word: ", &data, i, j);
      ++j;
      data = (Dictionary[i].translation->getString());
      writer(L" translation: ", &data, i, j);
      ++j;
      data = Dictionary[i].example->getString();
      writer(L" examples: ", &data, i, j);
      ++j;
    }
    fclose(pFile);
    return true;
  } else {
    std::cout << "Failed to save \n";
    return false;
  }
}