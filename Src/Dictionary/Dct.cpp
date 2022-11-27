#include "Dct.h"

#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>

namespace dct 
{
MyFonts Fonts;
static std::vector<Word> Dictionary;
}

dct::Dct::Dct() : Cursor(new sf::Vector2f(0.f,0.f)) 
{
  // clear dictionary before exit
  for (Word wd : Dictionary) {
    delete wd.word;
    delete wd.category;
    delete wd.translation;
    delete wd.example;
  }
  Dictionary.clear();
}

dct::Dct::~Dct() 
{ delete Cursor; }

void dct::Dct::Start() 
{
  // draw words before main loop starts
  /*DrawInLoop("Yahari", sf::Color::Green, 16, 10, 10);
  DrawInLoop(L"Україна", sf::Color::Green, 16, 10, 30);
  DrawInLoop(L"Санчіз", sf::Color::Green, 16, 10, 50);
  DrawInLoop("Monanto", sf::Color::Green, 16, 10, 70);
  LoadDictionary(L"dctionary.txt");*/
  //SaveDictionary(L"dctionary.txt");

  //exit(0);

  // RemoveFromDrawBuffer(L"Monanto");
  // std::wstring StringFromUser = GetString();
  // DrawInLoop( StringFromUser,  sf::Color::Green, 16, 10, 50);

  while (MainWindow->isOpen()) {
    EventsHandler();
    draw();
    draw(L"Кирил...", *Fonts.GetRobotoSlabFont(), sf::Color::Green, 16, 10, 90);

    for (size_t i = 0; i < Dictionary.size(); i++) {
      draw(*(Dictionary[i].translation), *Fonts.GetRobotoSlabFont(),
           sf::Color::Green, 16, 120, 14 * i);
    }

    display();
    clear();
  }
}


void dct::Dct::EventsHandler()
{

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