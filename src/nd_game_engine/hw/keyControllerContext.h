#ifndef KEY_CONTROLLER_CONTEXT_H_
#define KEY_CONTROLLER_CONTEXT_H_

#include <vector>
#include <functional>
#include <memory>

namespace nd {

  const unsigned int MAX_SCANCODES = 512;

  struct KeyControllerContext
  {
    explicit KeyControllerContext(unsigned int scanSize = MAX_SCANCODES)
    {
      keyPressEvents.resize(scanSize);
      keyReleaseEvents.resize(scanSize);
    }

    using KeyFunc_t = std::function<void(void)>;
    using KeyEvents = std::vector<KeyFunc_t>;

    KeyEvents keyPressEvents;
    KeyEvents keyReleaseEvents;

  };

  using KeyControllerContext_ptr = std::unique_ptr<KeyControllerContext>;
  using ScanCode_t = unsigned int;

}

#endif // KEY_CONTROLLER_CONTEXT_H_
