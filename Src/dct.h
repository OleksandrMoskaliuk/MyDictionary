#pragma once
#ifndef DICTIONARY2
#define DICTIONARY2

#include "../dict_v1/dictionary.h"
#include "Core/Core.h"
#include "SFML/System/Vector2.hpp"

class Vector2;
namespace sf 
{
 class CircleShape;
}

class DictionaryV2 : public my_dictionary::MyDictionary,
                     public dct_core::DctCore {
 public:
 DictionaryV2();
 ~DictionaryV2();
 void MainLoop() override;
private:
 class sf::CircleShape* CursorCircle;
 int8_t MenuCounter = 0;
 // Current list pointer to display
 std::vector<std::wstring> *CurrentList;
 std::vector<std::wstring>* ShowDictionaryData;
 struct Vector2f {};
 struct Word {
  std::wstring word;
  std::wstring translation;
  std::wstring category;
  std::wstring example;
 };
 // Holds All Words after load
 my_list::List<my_dictionary::Word>* Data;
 sf::Vector2f* CursorPosition;
 void const MakeList(std::vector<std::wstring> const * Text);
 void HandleEvents();
 void DrawWords();
 void AdjustCursorCirclebyMenuCounter();
 void EnterPressed();
 // Main menu functiuons
 void ShowDictionary();
};


#endif // !DICTIONARY2
