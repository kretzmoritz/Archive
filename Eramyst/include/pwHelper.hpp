#ifndef PWHELPER
#define PWHELPER

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <SFML\System\Vector2.hpp>
#include <SFML\Window\Event.hpp>

namespace pwHelper
{
    namespace math
    {
        const float EPSILON = 0.00000001f;
        const float PI = 3.1415926f;
        const float INV_HALFSPHERE = 0.0055555556f;

        #define PW_SQRT_CONST 0x1fc00000
        inline float sqrt(const float f)
        {
          union
          {
            int i;
            float f;
          } u;
          u.f = f;
          u.i = PW_SQRT_CONST + (u.i >> 1); 

          u.f =       u.f + f/u.f;
          u.f = 0.25f*u.f + f/u.f;

          return u.f;
        }

        inline float min(float x, float y)
        {
            return (x < y ? x : y);
        }
        inline float max(float x, float y)
        {
            return (x > y ? x : y);
        }

        inline float sind(float f)
        {
            return sin(f*PI*INV_HALFSPHERE);
        }
        inline float cosd(float f)
        {
            return cos(f*PI*INV_HALFSPHERE);
        }

        inline unsigned int length32(unsigned int n)
        {
            if(n < 100000) 
            {
                if(n < 1000) 
                {
                    if(n < 100) 
                    {
                        if(n < 10)
                            return 1;
                        return 2;
                    }
                    return 3;
                }
                if(n < 10000)
                    return 4;
                else
                    return 5;
            }
            if(n < 100000000) 
            {
                if(n < 10000000) 
                {
                    if(n < 1000000)
                        return 6;
                    return 7;
                }
                return 8;
            }
            if(n < 1000000000)
                return 9;
            else
                return 10;
        }

        class random
        {
        public:
            static void setSeed(unsigned int s = (unsigned int) time(NULL))
            {
                srand(s);
            }

            static float getFloat()
            {
                return (float)rand() + (float)rand() / RAND_MAX;
            }
            static float getFloatEx(float x = 0.0f, float y = 1.0f)
            {
                return x + (float)(rand()) / RAND_MAX * (y - x);
            }
            static int   getInt()
            {
                return rand();
            }
            static int   getIntEx(int x = 0, int y = 100)
            {
                return x + rand()%(y-x+1);
            }
            static bool  getBool()
            {
                return (getIntEx(0, 1) == 1 ? true : false);
            }
            static bool  getBoolEx(float percentageTrue = 0.5f)
            {
                return (getFloatEx() >= percentageTrue ? false : true);
            }
        };

        namespace vector2 
        {
            template <typename T>
            T dot(const sf::Vector2<T>& left, const sf::Vector2<T>& right) 
            {
                return left.x * right.x + left.y * right.y; 
            }

            template <typename T>
            T length(const sf::Vector2<T>& vector) 
            {
                return pwHelper::math::sqrt((vector.x * vector.x) + (vector.y * vector.y)); 
            }

            template <typename T>
            T lengthSQ(const sf::Vector2<T>& vector) 
            {
                return (vector.x * vector.x) + (vector.y * vector.y); 
            }
        }
    }

    class key
    {
    public:
        static std::string getKeyName(sf::Keyboard::Key k)
        {
            switch(k)
            {
            case sf::Keyboard::A:
                return "A";
                break;
            case sf::Keyboard::B:
                return "B";
                break;
            case sf::Keyboard::C:
                return "C";
                break;
            case sf::Keyboard::D:
                return "D";
                break;
            case sf::Keyboard::E:
                return "E";
                break;
            case sf::Keyboard::F:
                return "F";
                break;
            case sf::Keyboard::G:
                return "G";
                break;
            case sf::Keyboard::H:
                return "H";
                break;
            case sf::Keyboard::I:
                return "I";
                break;
            case sf::Keyboard::J:
                return "J";
                break;
            case sf::Keyboard::K:
                return "K";
                break;
            case sf::Keyboard::L:
                return "L";
                break;
            case sf::Keyboard::M:
                return "M";
                break;
            case sf::Keyboard::N:
                return "N";
                break;
            case sf::Keyboard::O:
                return "O";
                break;
            case sf::Keyboard::P:
                return "P";
                break;
            case sf::Keyboard::Q:
                return "Q";
                break;
            case sf::Keyboard::R:
                return "R";
                break;
            case sf::Keyboard::S:
                return "S";
                break;
            case sf::Keyboard::T:
                return "T";
                break;
            case sf::Keyboard::U:
                return "U";
                break;
            case sf::Keyboard::V:
                return "V";
                break;
            case sf::Keyboard::W:
                return "W";
                break;
            case sf::Keyboard::X:
                return "X";
                break;
            case sf::Keyboard::Y:
                return "Y";
                break;
            case sf::Keyboard::Z:
                return "Z";
                break;
            case sf::Keyboard::Num0:
                return "0";
                break;
            case sf::Keyboard::Num1:
                return "1";
                break;
            case sf::Keyboard::Num2:
                return "2";
                break;
            case sf::Keyboard::Num3:
                return "3";
                break;
            case sf::Keyboard::Num4:
                return "4";
                break;
            case sf::Keyboard::Num5:
                return "5";
                break;
            case sf::Keyboard::Num6:
                return "6";
                break;
            case sf::Keyboard::Num7:
                return "7";
                break;
            case sf::Keyboard::Num8:
                return "8";
                break;
            case sf::Keyboard::Num9:
                return "9";
                break;
            case sf::Keyboard::Escape:
                return "Esc";
                break;
            case sf::Keyboard::LControl:
                return "Left Ctrl";
                break;
            case sf::Keyboard::LShift:
                return "Left Shift";
                break;
            case sf::Keyboard::LAlt:
                return "Left Alt";
                break;
            case sf::Keyboard::LSystem:
                return "Left System";
                break;
            case sf::Keyboard::RControl:
                return "Right Ctrl";
                break;
            case sf::Keyboard::RShift:
                return "Right Shift";
                break;
            case sf::Keyboard::RAlt:
                return "Right Alt";
                break;
            case sf::Keyboard::RSystem:
                return "Right System";
                break;
            case sf::Keyboard::Menu:
                return "Menu";
                break;
            case sf::Keyboard::LBracket:
                return "[";
                break;
            case sf::Keyboard::RBracket:
                return "]";
                break;
            case sf::Keyboard::SemiColon:
                return ";";
                break;
            case sf::Keyboard::Comma:
                return ",";
                break;
            case sf::Keyboard::Period:
                return ".";
                break;
            case sf::Keyboard::Quote:
                return "'";
                break;
            case sf::Keyboard::Slash:
                return "/";
                break;
            case sf::Keyboard::BackSlash:
                return "\\";
                break;
            case sf::Keyboard::Tilde:
                return "~";
                break;
            case sf::Keyboard::Equal:
                return "=";
                break;
            case sf::Keyboard::Dash:
                return "-";
                break;
            case sf::Keyboard::Space:
                return "Space";
                break;
            case sf::Keyboard::Return:
                return "Return";
                break;
            case sf::Keyboard::Back:
                return "Back";
                break;
            case sf::Keyboard::Tab:
                return "Tab";
                break;
            case sf::Keyboard::PageUp:
                return "Page Up";
                break;
            case sf::Keyboard::PageDown:
                return "Page Down";
                break;
            case sf::Keyboard::End:
                return "End";
                break;
            case sf::Keyboard::Home:
                return "Home";
                break;
            case sf::Keyboard::Insert:
                return "Insert";
                break;
            case sf::Keyboard::Delete:
                return "Delete";
                break;
            case sf::Keyboard::Add:
                return "Add";
                break;
            case sf::Keyboard::Subtract:
                return "Subtract";
                break;
            case sf::Keyboard::Multiply:
                return "Multiply";
                break;
            case sf::Keyboard::Divide:
                return "Divide";
                break;
            case sf::Keyboard::Left:
                return "Left";
                break;
            case sf::Keyboard::Right:
                return "Right";
                break;
            case sf::Keyboard::Up:
                return "Up";
                break;
            case sf::Keyboard::Down:
                return "Down";
                break;
            case sf::Keyboard::Numpad0:
                return "Numpad 0";
                break;
            case sf::Keyboard::Numpad1:
                return "Numpad 1";
                break;
            case sf::Keyboard::Numpad2:
                return "Numpad 2";
                break;
            case sf::Keyboard::Numpad3:
                return "Numpad 3";
                break;
            case sf::Keyboard::Numpad4:
                return "Numpad 4";
                break;
            case sf::Keyboard::Numpad5:
                return "Numpad 5";
                break;
            case sf::Keyboard::Numpad6:
                return "Numpad 6";
                break;
            case sf::Keyboard::Numpad7:
                return "Numpad 7";
                break;
            case sf::Keyboard::Numpad8:
                return "Numpad 8";
                break;
            case sf::Keyboard::Numpad9:
                return "Numpad 9";
                break;
            case sf::Keyboard::F1:
                return "F1";
                break;
            case sf::Keyboard::F2:
                return "F2";
                break;
            case sf::Keyboard::F3:
                return "F3";
                break;
            case sf::Keyboard::F4:
                return "F4";
                break;
            case sf::Keyboard::F5:
                return "F5";
                break;
            case sf::Keyboard::F6:
                return "F6";
                break;
            case sf::Keyboard::F7:
                return "F7";
                break;
            case sf::Keyboard::F8:
                return "F8";
                break;
            case sf::Keyboard::F9:
                return "F9";
                break;
            case sf::Keyboard::F10:
                return "F10";
                break;
            case sf::Keyboard::F11:
                return "F11";
                break;
            case sf::Keyboard::F12:
                return "F12";
                break;
            case sf::Keyboard::F13:
                return "F13";
                break;
            case sf::Keyboard::F14:
                return "F14";
                break;
            case sf::Keyboard::F15:
                return "F15";
                break;
            case sf::Keyboard::Pause:
                return "Pause";
                break;
            default:
                return "";
            }
        }
    };

    template<class T1, class T2>
    class split_data
    {
    public:
        split_data() {}
        split_data(T1 _first, T2 _second)    {first = _first; second = _second;}

        T1 first;
        T2 second;
    };
}

#endif