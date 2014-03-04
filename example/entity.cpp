
namespace chips
{
  // default construct
  entity e;
  // construct with id
  entity e(entity_id::chip);
  // construct with id and list of attributes (any number)
  entity e(entity_id::chips, position(0,0), direction::N);
  // assignment like normal
  e = entity();
  // assign a attribute
  e = direction::N;
  // get the entity_id
  e.id();
  // get the number of attributes
  e.size();
  // check if an entity has an attribute
  bool = e.has_attribute<position>();
  // construct an attribute in place
  e.emplace_attribute<position>(0, 0);
  // insert an attribute
  e.insert_attribute(position(0, 0));
  // set an attribute (same as assignment operator)
  e.set_attribute(position(0, 0));
  // remove an attribute
  e.remove_attribute<position>();
  // get a pointer to an attribute (nothrow - returns null on failure)
  e.get_raw_attribute<position>();
  // get a reference to an attribute (throws if !has_attribute)
  e.get_attribute<position>();
}//namespace chips