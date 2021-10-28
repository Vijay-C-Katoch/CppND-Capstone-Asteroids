#ifndef KEYBOARD_CONTROLLER_H
#define KEYBOARD_CONTROLLER_H

#include "keyControllerContext.h"

namespace nd
{

  // Keys
  enum Key : unsigned int
  {
    NONE,
    UP, DOWN, LEFT, RIGHT,
    SPACE, TAB, 
    LSHIFT, LCTRL, RSHIFT, RCTRL,
    ESCAPE, RETURN, BACKSPACE,
    ORDINAL
  };


  class KeyController
  {
  public:
    KeyController() 
      : _keyControllerContext(new KeyControllerContext())
    {}

    bool OnKeyPress(const ScanCode_t& scanCode) const
    {
      if (scanCode < _keyControllerContext->keyPressEvents.size())
      {
        const auto& keyPressCb = _keyControllerContext->keyPressEvents[scanCode];
        if (keyPressCb)
        {
          keyPressCb();
          return true;
        }
      }
      return false;
    }

    bool OnKeyRelease(const ScanCode_t& scanCode) const
    {
      if (scanCode < _keyControllerContext->keyReleaseEvents.size())
      {
        const auto& keyReleaseCb = _keyControllerContext->keyReleaseEvents[scanCode];
        if (keyReleaseCb)
        {
          keyReleaseCb();
          return true;
        }
      }
      return false;
    }

    // connect Callbacks
    void ConnectKeyPress(const ScanCode_t& scanCode, const KeyControllerContext::KeyFunc_t& func)
    {
      if (scanCode < _keyControllerContext->keyPressEvents.size())
      {
        _keyControllerContext->keyPressEvents[scanCode] = func;
      }
    }

    void ConnectKeyRelease(const ScanCode_t& scanCode, const KeyControllerContext::KeyFunc_t& func)
    {
      if (scanCode < _keyControllerContext->keyReleaseEvents.size())
      {
        _keyControllerContext->keyReleaseEvents[scanCode] = func;
      }
    }

    // clear callbacks for both press and release
    void DisconnectKey(const ScanCode_t& scanCode)
    {
      // assign empty function
      _keyControllerContext->keyPressEvents[scanCode] = KeyControllerContext::KeyFunc_t();
      _keyControllerContext->keyReleaseEvents[scanCode] = KeyControllerContext::KeyFunc_t();
    }

  protected:
    KeyControllerContext_ptr _keyControllerContext;

  };
}



#endif // !KEYBOARD_CONTROLLER_H
