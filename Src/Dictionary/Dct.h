#pragma once
#ifndef DICTIONARY
#define DICTIONARY

#include "../Core/Core.h"
#include "SFML/System/Vector2.hpp"

class Vector2;
namespace sf {
class CircleShape;
}
namespace dct 
{
struct Word {
  sf::String *word;
  sf::String *translation;
  sf::String *category;
  sf::String *example;
};  // struct Word
class Dct : public dct_core::DctCore {
 public:
  Dct();
  ~Dct();
  void Start();
 private:
  class sf::CircleShape* CursorCircle;
  int MenuCounter = 0;
  // Current list pointer to display
  struct Vector2f {};
  // Current cursor position
  sf::Vector2f* Cursor;
  void const MakeList();
  /* Handle all events, closing window, pressing button, etc..*/
  void EventsHandler() override;
  void DrawWords();
  void AdjustCursorCirclebyMenuCounter();
  void EnterPressed();
  // Main menu functiuons
  void ShowDictionary();
  /* Check  if word exist in dictionary */
  bool IsInDictionary(sf::String new_word);
  /* Read all words from saved *.txt file */
  bool LoadDictionary(sf::String filename);
  /* Save dictionary in file */
  bool SaveDictionary(sf::String filename);
};
}
#endif  // !DICTIONARY2