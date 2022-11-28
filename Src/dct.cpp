#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

/* USER GUI */
#include "dct.h"

/* Work with files */
#include <Windows.h>

#include <fstream>

DictionaryV2::DictionaryV2() :, CursorPosition(new sf::Vector2f(0.f, 0.f)) {
  // setup simple circle , for menu cursor
  CursorCircle = new sf::CircleShape(8.f, 8.f);  // size of circle
  CursorCircle->setFillColor(sf::Color::Green);
  CursorCircle->setPosition(sf::Vector2f(0.f, 0.f));
  // Set Data poiner from parent
  Data = GetData();
}

DictionaryV2::~DictionaryV2() { delete (CursorPosition); }

void DictionaryV2::MainLoop() {
  std::vector<std::wstring> menu_info;
  // to add new case just push back it to menu_info vector;
  menu_info.push_back(L"_SHOW_DICTIONARY");  // case 0
  menu_info.push_back(L"_ADD_WORD");         // case 1
  menu_info.push_back(L"_REMOVE_WORD");      // case 2
  menu_info.push_back(L"_EDIT_WORD");        // case 3
  menu_info.push_back(L"_TEST_YOURSELF");    // case 4
  menu_info.push_back(L"_SAVE");             // case 5
  menu_info.push_back(L"_SAVE_AND_EXIT");    // case 6
  menu_info.push_back(L"_EXIT");             // case 7
  CurrentList = &menu_info;

  std::thread th([&]() {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    RemoveWordByName(L"Text");
  });
  th.detach();

  while (GetWindow()->isOpen()) {
    HandleEvents();
    MakeList(CurrentList);
    // Will write word into draw buffer
    DrawWords();
    GetWindow()->display();
    GetWindow()->clear();
  }
}

void const DictionaryV2::MakeList(std::vector<std::wstring> const* Text) {
  if (!Text) return;  // nullptr check;
  this->CleanAllWords();
  CursorPosition->y = 0;
  CursorPosition->x = 30;
  for (std::wstring wrd : *Text) {
    DrawInLoop(wrd, sf::Color::Green, 18, CursorPosition->x, CursorPosition->y);
    CursorPosition->y += 22;
  }
}

void DictionaryV2::HandleEvents() {
  std::wstring str = L"dsagsdg";
  sf::Text dummy_text;
  // dummy_text.setFont(*GetBisternFont());
  dummy_text.setPosition(50.f, 50.f);
  dummy_text.setString(str);
  dummy_text.setFillColor(sf::Color::Green);
  while (GetWindow()->pollEvent(*GetEvent())) {
    switch (GetEvent()->type) {
      default: {
      } break;
      case sf::Event::Closed: {
        GetWindow()->close();
      } break;
      // handle text input for player
      case sf::Event::TextEntered: {
        if (GetEvent()->text.unicode == '\b' && str.size() > 0) {
          str.erase(str.size() - 1, 1);
        } else {
          str += GetEvent()->text.unicode;
        }
        dummy_text.setString(str);
        GetWindow()->draw(dummy_text);
      } break;
      case sf::Event::KeyPressed: {
        // Move cursor down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
          this->MenuCounter++;
          AdjustCursorCirclebyMenuCounter();
        }
        // Move cursor up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
          this->MenuCounter--;
          AdjustCursorCirclebyMenuCounter();
        }
        // Enter
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          EnterPressed();
        }
      }
    }  // switch (Data->event->type)
  }    // while (Data->MainWindow->pollEvent(*Data->event))
}

void DictionaryV2::DrawWords() {
  if (CurrentList && CurrentList->size() > 0) {
  }
  if (CursorCircle) {
    GetWindow()->draw(*this->CursorCircle);
  }
  DrawWords();
}

void DictionaryV2::AdjustCursorCirclebyMenuCounter() {
  if (MenuCounter > (int16_t)CurrentList->size() - 1) {
    MenuCounter = 0;
  }
  if (MenuCounter < 0) {
    MenuCounter = CurrentList->size() - 1;
  }
  CursorCircle->setPosition(0, MenuCounter * 22);
}

void DictionaryV2::EnterPressed() {
  std::cout << "Enter pressed!\n";
  switch (MenuCounter) {
    case 0: {  // _SHOW_DICTIONARY
      ShowDictionary();
    } break;

    case 1: {  // _ADD_WORD
    } break;

    case 2: {  // _REMOVE_WORD
    } break;

    case 3: {  // _EDIT_WORD
    } break;

    case 4: {  // _TEST_YOURSELF
    } break;

    case 5: {  // _SAVE
    } break;

    case 6: {  // _SAVE_AND_EXIT
    } break;

    case 7: {  // _EXIT
    } break;

    default:
      break;
  }
}

void DictionaryV2::ShowDictionary() {
  this->CleanAllWords();
  if (!ShowDictionaryData) {
    this->ShowDictionaryData = new std::vector<std::wstring>();
  }

  // System::SharedPtr<Document> doc = System::MakeObject<Document>();

  setlocale(LC_ALL, "ru");
  for (int i = 0; i < Data->get_size(); i++) {
    std::wstring ws_word((*Data)[i].word.begin(), (*Data)[i].word.end());
    std::wstring ws_translation((*Data)[i].translation.begin(),
                                (*Data)[i].translation.end());
    std::wstring ws_categoty((*Data)[i].category.begin(),
                             (*Data)[i].category.end());

    ShowDictionaryData->push_back(std::wstring(L". ÔÂ²²Ô Word: ") + ws_word);
    setlocale(LC_CTYPE, "Ukrainian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::cout << (*Data)[i].translation << "\n";
    ShowDictionaryData->push_back(
        std::to_wstring(i) + std::wstring(L". translation: ") + ws_translation);
    setlocale(LC_CTYPE, "en");
    SetConsoleCP(866);
    SetConsoleOutputCP(866);
    /* ShowDictionaryData->push_back(
         std::to_string(i) + std::wstring(". Categoty: ") +
     (*Data)[i].category); ShowDictionaryData->push_back( std::to_string(i) +
     std::wstring(" Example: ") + (*Data)[i].example);*/

    // std::cout << ">>>" << std::endl;
  }
  CurrentList = ShowDictionaryData;
  bool ShowDictionary = true;
  while (GetWindow()->isOpen() && ShowDictionary) {
    HandleEvents();
    MakeList(CurrentList);
    // Will write word into draw buffer
    DrawWords();
    GetWindow()->display();
    GetWindow()->clear();
  }
}
