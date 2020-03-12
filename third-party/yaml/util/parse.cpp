#include <fstream>
#include <iostream>
#include <vector>

#include "yaml/eventhandler.h"
#include "yaml/yaml.h"  // IWYU pragma: keep

struct Params {
  bool hasFile;
  std::string fileName;
};

Params ParseArgs(int argc, char** argv) {
  Params p;

  std::vector<std::string> args(argv + 1, argv + argc);

  return p;
}

class NullEventHandler : public zeus::yaml::EventHandler {
 public:
  void OnDocumentStart(const zeus::yaml::Mark&) override {}
  void OnDocumentEnd() override {}

  void OnNull(const zeus::yaml::Mark&, zeus::yaml::anchor_t) override {}
  void OnAlias(const zeus::yaml::Mark&, zeus::yaml::anchor_t) override {}
  void OnScalar(const zeus::yaml::Mark&, const std::string&, zeus::yaml::anchor_t,
                const std::string&) override {}

  void OnSequenceStart(const zeus::yaml::Mark&, const std::string&, zeus::yaml::anchor_t,
                       zeus::yaml::EmitterStyle::value) override {}
  void OnSequenceEnd() override {}

  void OnMapStart(const zeus::yaml::Mark&, const std::string&, zeus::yaml::anchor_t,
                  zeus::yaml::EmitterStyle::value) override {}
  void OnMapEnd() override {}
};

void parse(std::istream& input) {
  try {
    zeus::yaml::Node doc = zeus::yaml::Load(input);
    std::cout << doc << "\n";
  } catch (const zeus::yaml::Exception& e) {
    std::cerr << e.what() << "\n";
  }
}

int main(int argc, char** argv) {
  Params p = ParseArgs(argc, argv);

  if (argc > 1) {
    std::ifstream fin;
    fin.open(argv[1]);
    parse(fin);
  } else {
    parse(std::cin);
  }

  return 0;
}
