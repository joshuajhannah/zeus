#include <iostream>

#include "yaml/emitterstyle.h"
#include "yaml/eventhandler.h"
#include "yaml/yaml.h"  // IWYU pragma: keep

class NullEventHandler : public zeus::yaml::EventHandler {
 public:
  typedef zeus::yaml::Mark Mark;
  typedef zeus::yaml::anchor_t anchor_t;

  NullEventHandler() {}

  virtual void OnDocumentStart(const Mark&) {}
  virtual void OnDocumentEnd() {}
  virtual void OnNull(const Mark&, anchor_t) {}
  virtual void OnAlias(const Mark&, anchor_t) {}
  virtual void OnScalar(const Mark&, const std::string&, anchor_t,
                        const std::string&) {}
  virtual void OnSequenceStart(const Mark&, const std::string&, anchor_t,
                               zeus::yaml::EmitterStyle::value style) {}
  virtual void OnSequenceEnd() {}
  virtual void OnMapStart(const Mark&, const std::string&, anchor_t,
                          zeus::yaml::EmitterStyle::value style) {}
  virtual void OnMapEnd() {}
};

int main() {
  zeus::yaml::Node root;

  for (;;) {
    zeus::yaml::Node node;
    root = node;
  }
  return 0;
}
