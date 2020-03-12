#include "graphbuilderadapter.h"

#include "yaml/parser.h"  // IWYU pragma: keep

namespace zeus::yaml {
class GraphBuilderInterface;

void* BuildGraphOfNextDocument(Parser& parser,
                               GraphBuilderInterface& graphBuilder) {
  GraphBuilderAdapter eventHandler(graphBuilder);
  if (parser.HandleNextDocument(eventHandler)) {
    return eventHandler.RootNode();
  } else {
    return nullptr;
  }
}
}
