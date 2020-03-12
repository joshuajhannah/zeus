#include "yaml/node/node.h"
#include "nodebuilder.h"
#include "nodeevents.h"

namespace zeus::yaml {
Node Clone(const Node& node) {
  NodeEvents events(node);
  NodeBuilder builder;
  events.Emit(builder);
  return builder.Root();
}
}
