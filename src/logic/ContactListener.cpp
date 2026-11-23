#include "logic/ContactListener.h"

using namespace escape;

/*
* Called when objects in b2World collide
* @param *contact: Box2D object that manages contact between two shapes
*/
void ContactListener::BeginContact(b2Contact* contact){
  //check if fixture A was a ball
  void* bodyUserData = reinterpret_cast<void*>( contact -> GetFixtureA() -> GetBody() -> GetUserData().pointer );
  if ( bodyUserData ){
    double yVel = static_cast<StolenObject*> ( bodyUserData ) -> Body -> GetLinearVelocity().y;
    static_cast<StolenObject*> ( bodyUserData ) -> startContact(yVel);

  }

  bodyUserData = reinterpret_cast<void*>( contact->GetFixtureB()->GetBody()->GetUserData().pointer );
  if ( bodyUserData ){
    double yVel = static_cast<StolenObject*> ( bodyUserData ) -> Body -> GetLinearVelocity().y;
    static_cast<StolenObject*> ( bodyUserData ) -> startContact(yVel);
  }
}

/*
* Reserved for future use
* Needed to overwrite pure virtual function in parent class
* @param *contact: Box2D object that manages contact between two shapes
*/
void ContactListener::EndContact(b2Contact* contact){

}
