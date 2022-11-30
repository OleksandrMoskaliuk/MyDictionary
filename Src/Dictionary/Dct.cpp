#include "Dct.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

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
      new sf::Text(L"SHOW DICTIONARY", *Fonts.GetFuturaFont(), 16));
  Menu.push_back(
      new sf::Text(L"ADD WORD", *Fonts.GetFuturaFont(), 16));
  Menu.push_back(
      new sf::Text(L"TEST YOURSELF", *Fonts.GetFuturaFont(), 16));
  Menu.push_back(new sf::Text(L"SAVE", *Fonts.GetFuturaFont(), 16));
  Menu.push_back(
      new sf::Text(L"SAVE AND EXIT", *Fonts.GetFuturaFont(), 16));
  Menu.push_back(new sf::Text(L"EXIT", *Fonts.GetFuturaFont(), 16));
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
    draw(
        L"Controls:\nRght arrow = Enter\nLeft arrow = Back\nUp arrow = Move "
        L"up\nDown arrow = Move down",
        Fonts.GetRobotoSlabFont(), sf::Color::Green, 18, 0, 0);
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
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
              // Backup fore some variables
              sf::Vector2f last_cursor_position = Cursor->getPosition();
              int last_menu_counter = MenuCounter;
              AddNewWord();
              Cursor->setPosition(last_cursor_position);
              MenuCounter = last_menu_counter;
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
  int font_size = 16;
  // Set cursor pose based on output words and MainWindow
  sf::Vector2f cursor_static_pose;
  cursor_static_pose.x =
      (MainWindow->getSize().x / 2) - (longest_word * font_size);
  cursor_static_pose.y = (MainWindow->getSize().y / 2) - font_size;
  Cursor->setPosition(cursor_static_pose);
  // How many words to display
  uint8_t words_to_display = 6;
  MenuCounter = 0;
  auto up_and_down_action = [&]() {
    if (Dictionary.size() && Dictionary[MenuCounter].word &&
        Dictionary[MenuCounter].translation) {
      // Make a new draw buffer
      CleanDrawBuffer();
      // bottom words part
      for (int8_t i = 0; i < words_to_display; i++) {
        if (MenuCounter + i < Dictionary.size() - 1 &&
            Dictionary[MenuCounter + i].word) {
          DrawInLoop(Dictionary[MenuCounter + i].word, sf::Color::Green,
                     14 + cursor_static_pose.x,
                     i * font_size * WordsYSpace +
                         cursor_static_pose.y - 3.5);
          DrawInLoop(Dictionary[MenuCounter + i].translation, sf::Color::Green,
              cursor_static_pose.x + (longest_word * font_size),
                     i * font_size * WordsYSpace +
                         cursor_static_pose.y - 3.5);
          std::cout << "Max character size = " << font_size
                    << std::endl;
        }
      }
      // top words part
      for (int8_t i = 0; i < words_to_display - 1; i++) {
        if (MenuCounter - i >= 0 && Dictionary[MenuCounter - i].word) {
          DrawInLoop(Dictionary[MenuCounter - i].word, sf::Color::Green,
                     14 + cursor_static_pose.x,
                     -i * font_size * WordsYSpace +
                         cursor_static_pose.y - 3.5);
          DrawInLoop(Dictionary[MenuCounter - i].translation, sf::Color::Green,
              cursor_static_pose.x + (longest_word * font_size),
                     -i * font_size * WordsYSpace +
                         cursor_static_pose.y - 3.5);
        }
      }
      if (Dictionary.size() && Dictionary[MenuCounter].example) {
        DrawInLoop(L"Example:", sf::Color::Green, 18, 0.f, 0.f);
        if (!std::wstring(Dictionary[MenuCounter].example->getString())
                 .compare(L"")) {
          DrawInLoop(L"<empty>", sf::Color::Red, 16, 0.f, 20.f);
        } else {
          DrawInLoop(Dictionary[MenuCounter].example, sf::Color::Green, 0.f,
                     20.f);
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
          }
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (MenuCounter == 0) {
              MenuCounter = Dictionary.size() - 1;
            } else {
              MenuCounter--;
            }
            up_and_down_action();
          }
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            CleanDrawBuffer();
            sf::Vector2f cursor_pose_backup = Cursor->getPosition();
            int menu_couner_backup = MenuCounter;
            AddNewWord(Dictionary[MenuCounter].word,
                       Dictionary[MenuCounter].translation,
                       Dictionary[MenuCounter].example);
            MenuCounter = menu_couner_backup;
            Cursor->setPosition(cursor_pose_backup);
            up_and_down_action();
          }
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            CleanDrawBuffer();
            return;
          }
        } break; //sf::Event::KeyPressed:
        default: {
        } break; // default
      }
    }
    draw(*Cursor);
    draw();
    display();
    clear();
  }
}

bool dct::Dct::AddNewWord() 
{
  return AddNewWord(nullptr, nullptr, nullptr);
}

bool dct::Dct::AddNewWord(sf::Text *word, sf::Text *translation,
                          sf::Text *example) {
  CleanDrawBuffer();
  MenuCounter = 0;
  Cursor->setPosition(0.f, 0.f);
  unsigned int character_size = 18u;
  bool block_inputs = false;
  // Category fields, first part
  std::vector<sf::Text> fields;
  fields.push_back(
      sf::Text(L"Word:", *Fonts.GetRobotoSlabFont(), character_size));
  fields.push_back(
      sf::Text(L"Translation:", *Fonts.GetRobotoSlabFont(), character_size));
  fields.push_back(
      sf::Text(L"Example:", *Fonts.GetRobotoSlabFont(), character_size));
  if (word && translation && example) {
    fields.push_back(
        sf::Text(L"Edit word and go back", *Fonts.GetRobotoSlabFont(), character_size));
  } else {
  fields.push_back(
      sf::Text(L"Add word and go back", *Fonts.GetRobotoSlabFont(), character_size));
  }
  for (size_t i = 0; i < fields.size(); i++) {
    fields[i].setFillColor(sf::Color::Green);
  }

  // User inputs, second part
  std::vector<sf::Text> uinputs;
  // if methot imputs is not  nullptr we in edit mode
  if (word && translation && example) {
    uinputs.push_back(*word);
    uinputs.push_back(*translation);
    uinputs.push_back(*example);
    for (size_t i = 0; i < 3; i++) {
      uinputs[i].setFont(*Fonts.GetRobotoSlabFont());
      uinputs[i].setFillColor(sf::Color::Green);
    }
  } else {
  for (size_t i = 0; i < 3; i++) {
    uinputs.push_back(
        sf::Text(L"", *Fonts.GetRobotoSlabFont(), character_size));
    uinputs[i].setFillColor(sf::Color::Green);
  }
  }

  // Update cursor
  auto update_cursor_pose = [&]() {
    // Additional check
    if (MenuCounter < fields.size()) {
      Cursor->setPosition(
          fields[MenuCounter].getPosition().x - Cursor->getRadius() * 3,
          fields[MenuCounter].getPosition().y + 4);
    }
  };
  auto get_ustring_wrapper = [&](int word_category_index,
                                 sf::String initial_wstr = L"") 
  {
    float start_drawing_point_x = 0.f;
    float start_drawing_point_y = 20.f;
    // This drawing will display string below in Core::GetStringMenu method 
    if (word && translation && example) {
      // Edit mode
      DrawInLoop(L"Edit word and press rigth arrow.", sf::Color::Green, 18u,
                 MainWindow->getSize().x / 3.f, MainWindow->getSize().y / 3.f);
    } else {
      DrawInLoop(L"Write new word and press right arrow.", sf::Color::Green, 18u,
                 MainWindow->getSize().x / 3.f, MainWindow->getSize().y / 3.f);
    }
    block_inputs = true;
    // Core::GetStringMenu will handle user input 
    sf::String wstring_from_user =
        GetStringMenu(Fonts.GetRobotoSlabFont(), initial_wstr, sf::Color::Green,
                      character_size, MainWindow->getSize().x / 2.5f,
                      MainWindow->getSize().y / 2.6f);
    uinputs[word_category_index].setString(wstring_from_user);
    block_inputs = false;
    CleanDrawBuffer();
  };
  while (MainWindow->isOpen()) {
    while (MainWindow->pollEvent(*event)) {
      switch (event->type) {
          // Close main window event
        case sf::Event::Closed: {
          MainWindow->close();
        } break;
        case sf::Event::KeyPressed: {
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !block_inputs) {
            MenuCounter++;
            if (MenuCounter >= fields.size()) {
              MenuCounter = 0;
            }
            update_cursor_pose();
          }  // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !block_inputs) {
            if (MenuCounter == 0) {
              MenuCounter = fields.size() - 1;
            } else {
              MenuCounter--;
            }
            update_cursor_pose();
          }  // (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (!std::wstring(fields[MenuCounter].getString())
                     .compare(L"Word:")) {
              if (word && translation && example) {
                get_ustring_wrapper(0,word->getString());
              } else {
                get_ustring_wrapper(0);
              }
            }
            if (!std::wstring(fields[MenuCounter].getString())
                     .compare(L"Translation:")) {
              if (word && translation && example) {
                get_ustring_wrapper(1, word->getString());
              } else {
                get_ustring_wrapper(1);
              }
            }
            if (!std::wstring(fields[MenuCounter].getString())
                     .compare(L"Example:")) {
              // Edit mode
              if (word && translation && example) {
                get_ustring_wrapper(2, word->getString());
              } else { // New word mode
                get_ustring_wrapper(2);
              }
            }
            if (!std::wstring(fields[MenuCounter].getString())
                     .compare(L"Edit word and go back")) {
              // Save words and go back to previous menu
              word->setString(uinputs[0].getString());
              translation->setString(uinputs[1].getString());
              example->setString(uinputs[2].getString());
              return true;
            }
            if (!std::wstring(fields[MenuCounter].getString())
                     .compare(L"Add word and go back")) {
              Word new_word;
              new_word.word = new sf::Text(uinputs[0].getString(),
                                           *Fonts.GetBisternFont(), 16u);
              new_word.translation = new sf::Text(uinputs[1].getString(),
                                                  *Fonts.GetBisternFont(), 16u);
              new_word.example = new sf::Text(uinputs[2].getString(),
                                              *Fonts.GetBisternFont(), 16u);
              // Do not add to Dictionary if the same word exist in dictionary 
              if (IsInDictionary(new_word)) {
                delete new_word.word;
                delete new_word.translation;
                delete new_word.example;
                return false;
              } else {
                Dictionary.push_back(new_word);
                return true;
              }
            }
          }
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            // Stop thread for preventing go back to far
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            CleanDrawBuffer();
            return false;
          }
        } break; // case sf::Event::KeyPressed:
        default: {
        } break; // default;
      }
    }
    // Draw input fields
    for (size_t i = 0; i < fields.size(); i++) {
      draw(fields[i], Cursor->getRadius() * 4,
           character_size * i * WordsYSpace);
    }
    // Draw user input
    for (size_t i = 0; i < uinputs.size(); i++) {
      float xp = fields[i].getPosition().x +
                 fields[i].getCharacterSize() * fields[i].getString().getSize() + 10;
      float yp = character_size * i * WordsYSpace;
      draw(uinputs[i], xp, yp);
    }
    // If cursor positions is not initialized
    if (!Cursor->getPosition().x && !Cursor->getPosition().y) {
      update_cursor_pose();
    }
    draw(*Cursor);
    draw();
    display();
    clear();
  }
  return false;
}

bool dct::Dct::IsInDictionary(Word &new_word) const {
  for (int i = 0; i < Dictionary.size(); i++) {
    // Check if same word exist in dictionary
    if (!std::wstring(Dictionary[i].word->getString())
             .compare(std::wstring(new_word.word->getString()))) {
     // Check if same translation in this word 
     if (!std::wstring(Dictionary[i].word->getString())
              .compare(std::wstring(new_word.translation->getString()))) {
       return true;
     }
    }
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