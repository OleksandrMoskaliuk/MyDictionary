#pragma once
#ifndef DICTIONARY
#define DICTIONARY

#include "../Core/Core.h"
#include "SFML/System/Vector2.hpp"

class Vector2;
namespace sf {
class CircleShape;
class Text;
}
namespace dct 
{
struct Word {
  sf::Text *word;
  sf::Text *translation;
  sf::Text *example;
};  // struct Word

class Dct : public dct_core::DctCore {
 public:
  Dct();
  ~Dct();
  void Start();
 private:
  // Current cursor position
  class sf::CircleShape* Cursor;
  int MenuCounter = 0;
  // Current list pointer to display
  struct Vector2f {};
  /* Handle all events, closing window, pressing button, etc..*/
  void EventsHandler();
  void DrawWords();
  void AdjustCursorCirclebyMenuCounter();
  // Main menu functiuons
  void ShowDictionary();
  // Add new word to dictionary
  bool AddNewWord();
  bool AddNewWord(sf::Text *word, sf::Text *translation, sf::Text *example);
  /* Check  if word exist in dictionary */
  bool IsInDictionary( Word &new_word) const;
  /* Read all words from saved *.txt file */
  bool LoadDictionary(sf::String filename);
  /* Save dictionary in file */
  bool const SaveDictionary(sf::String filename) const;
};
}
#endif  // !DICTIONARY2
