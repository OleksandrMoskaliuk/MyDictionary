#include <SFML/Graphics.hpp>
#include "Core.h"
#include <iostream>
#include <vector>
//
#include <string>
#include <vector>
/*
Holds all variables and configs for DctCore class
*/
namespace dct_core
{
static std::vector<sf::Drawable>* DrawBuffer;
}
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
}

void dct_core::DctCore::draw() 
{
 // for drawing text in loop
  for (int i = 0; i < (DrawBuffer->size()); i++) 
  {
   //MainWindow->draw((DrawBuffer[i]));
  }
}

void dct_core::DctCore::draw(sf::Drawable& dr) { MainWindow->draw(dr); }

void dct_core::DctCore::draw(std::wstring& str, sf::Font& font, sf::Color color,
                             int font_size, uint32_t xp, uint32_t yp) {
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(font_size);
  text.setFillColor(color);
  text.setPosition(xp,yp);
  text.setString(str);
  MainWindow->draw(text);
}

void dct_core::DctCore::display() { 
 MainWindow->display();
}

void dct_core::DctCore::clear() 
{ 
 MainWindow->clear(); 
}

void dct_core::DctCore::DrawAndDisplay() {
  for (sf::Drawable* dr : *Data->RegisteredOblectToDraw) {
    MainWindow->draw(*dr);
  }
  // Draw words added by dct_coreDctCore::DrawInLoop function
  MainWindow->display();
  MainWindow->clear();
  // std::cout << "2\n";
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void dct_core::DctCore::EventsHandler() {
  while (MainWindow->pollEvent(*event)) {
    switch (event->type) {
      default: {
      } break;
      case sf::Event::Closed: {
        MainWindow->close();
      } break;
      // handle text input for player
      case sf::Event::TextEntered: {
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

void dct_core::DctCore::DrawInLoop(std::wstring Text, int xp, int yp) {
  int TextSize = 20;
  sf::Color TextColor = sf::Color::Green;
  this->DrawInLoop(Text, TextColor, TextSize, xp, yp);
}

void dct_core::DctCore::DrawInLoop( std::wstring str, sf::Color TextColor, int TextSize, int xp, int yp) {
  sf::Text text;
  for (int i = 0; i < DrawBuffer->size(); i++) {
    /*std::wstring* pstr = dynamic_cast<std::wstring*>(&DrawBuffer[i]);
    if (pstr) 
    {
      std::cout << "dynamic cast works!\n";
    }*/
  }
  

  text.setFont(*fonts.GetRobotoSlabFont());
  text.setString(str);
  text.setCharacterSize(TextSize);
  text.setFillColor(TextColor);
  text.setPosition(sf::Vector2f(xp, yp));
  MainWindow->draw(text);
  this->Data->WordsToDraw.push_back(text);
}

void dct_core::DctCore::RemoveWordByName(std::wstring WordToRemove) {
  std::vector<sf::Text>::iterator ToEraseIterator = Data->WordsToDraw.begin();
  for (int counter = 0; counter < Data->WordsToDraw.size(); counter++) {
    std::wstring str_to_compare =
        std::wstring(Data->WordsToDraw[counter].getString());
    if (!str_to_compare.compare(WordToRemove)) {
      this->Data->WordsToDraw.erase(ToEraseIterator);
      return;
    }
    ToEraseIterator++;
  }
}

void dct_core::DctCore::CleanAllWords() { this->Data->WordsToDraw.clear(); }

sf::Event* dct_core::DctCore::GetEvent() { return event; }

sf::RenderWindow* dct_core::DctCore::GetWindow() { return MainWindow; }

std::wstring dct_core::DctCore::GetString() {
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
  return std::wstring();
}

void dct_core::DctCore::MainLoop() {
  // draw words before main loop starts
  DrawInLoop(L"Yahari", sf::Color::Green, 16, 10, 10);
  DrawInLoop( L"Yahari", sf::Color::Green,16, 10, 30);
  //RemoveWordByName(L"No");
  //std::wstring StringFromUser = GetString();
  //DrawInLoop( StringFromUser,  sf::Color::Green, 16, 10, 50);
  while (MainWindow->isOpen()) {
    EventsHandler();
    draw();
    display();
    
  }
}

