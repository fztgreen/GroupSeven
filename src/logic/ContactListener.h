#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <box2d/box2d.h>
#include "logic/StolenObject.h"
#include "logic/Platform.h"

namespace escape{
  class ContactListener : public b2ContactListener{
    public:
      void BeginContact(b2Contact* contact);
      void EndContact(b2Contact* contact);
  };
}
#endif
