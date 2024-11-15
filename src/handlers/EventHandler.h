#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <DrawingWindow.h>

class EventHandler {
public:
   virtual ~EventHandler() {}

   virtual void handleEvent(DrawingWindow& window, SDL_Event& event) = 0;
};

#endif //EVENTHANDLER_H
