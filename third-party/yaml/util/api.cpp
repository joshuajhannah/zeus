// a sketch of what the new API might look like

#include "yaml/yaml.h"
#include <iostream>

int main() {
  {
    // test.yaml
    // - foo
    // - primes: [2, 3, 5, 7, 11]
    //   odds: [1, 3, 5, 7, 9, 11]
    // - [x, y]

    // move-like semantics
    zeus::yaml::Value root = zeus::yaml::Parse("test.yaml");

    std::cout << root[0].as<std::string>();       // "foo"
    std::cout << str(root[0]);                    // "foo", shorthand?
    std::cout << root[1]["primes"][3].as<int>();  // "7"
    std::cout << root[1]["odds"][6].as<int>();    // throws?

    root[2].push_back(5);
    root[3] = "Hello, World";
    root[0].reset();
    root[0]["key"] = "value";

    std::cout << root;
    // # not sure about formatting
    // - {key: value}
    // - primes: [2, 3, 5, 7, 11]
    //   odds: [1, 3, 5, 7, 9, 11]
    // - [x, y, 5]
    // - Hello, World
  }

  {
    // for all copy-like commands, think of python's "name/value" semantics
    zeus::yaml::Value root = "Hello";  // Hello
    root = zeus::yaml::Sequence();     // []
    root[0] = 0;                 // [0]
    root[2] = "two";  // [0, ~, two]  # forces root[1] to be initialized to null

    zeus::yaml::Value other = root;  // both point to the same thing
    other[0] = 5;              // now root[0] is 0 also
    other.push_back(root);     // &1 [5, ~, two, *1]
    other[3][0] = 0;           // &1 [0, ~, two, *1]   # since it's a true alias
    other.push_back(Copy(root));  // &1 [0, ~, two, *1, &2 [0, ~, two, *2]]
    other[4][0] = 5;  // &1 [0, ~, two, *1, &2 [5, ~, two, *2]]  # they're
                      // really different
  }

  {
    zeus::yaml::Value node;    // ~
    node[0] = 1;         // [1]  # auto-construct a sequence
    node["key"] = 5;     // {0: 1, key: 5}  # auto-turn it into a map
    node.push_back(10);  // error, can't turn a map into a sequence
    node.erase("key");  // {0: 1}  # still a map, even if we remove the key that
                        // caused the problem
    node = "Hello";  // Hello  # assignment overwrites everything, so it's now
                     // just a plain scalar
  }

  {
    zeus::yaml::Value map;  // ~
    map[3] = 1;       // {3: 1}  # auto-constructs a map, *not* a sequence

    zeus::yaml::Value seq;         // ~
    seq = zeus::yaml::Sequence();  // []
    seq[3] = 1;              // [~, ~, ~, 1]
  }

  {
    zeus::yaml::Value node;  // ~
    node[0] = node;    // &1 [*1]  # fun stuff
  }

  {
    zeus::yaml::Value node;
    zeus::yaml::Value subnode =
        node["key"];    // 'subnode' is not instantiated ('node' is still null)
    subnode = "value";  // {key: value}  # now it is
    zeus::yaml::Value subnode2 = node["key2"];
    node["key3"] = subnode2;  // subnode2 is still not instantiated, but
                              // node["key3"] is "pseudo" aliased to it
    subnode2 = "monkey";  // {key: value, key2: &1 monkey, key3: *1}  # bam! it
                          // instantiates both
  }

  {
    zeus::yaml::Value seq = zeus::yaml::Sequence();
    seq[0] = "zero";  // [zero]
    seq[1] = seq[0];  // [&1 zero, *1]
    seq[0] = seq[1];  // [&1 zero, *1]  # no-op (they both alias the same thing,
                      // so setting them equal is nothing)
    Is(seq[0], seq[1]);  // true
    seq[1] = "one";      // [&1 one, *1]
    UnAlias(seq[1]);     // [one, one]
    Is(seq[0], seq[1]);  // false
  }

  {
    zeus::yaml::Value root;
    root.push_back("zero");
    root.push_back("one");
    root.push_back("two");
    zeus::yaml::Value two = root[2];
    root = "scalar";  // 'two' is still "two", even though 'root' is "scalar"
                      // (the sequence effectively no longer exists)

    // Note: in all likelihood, the memory for nodes "zero" and "one" is still
    // allocated. How can it go away? Weak pointers?
  }

  {
    zeus::yaml::Value root;  // ~
    root[0] = root;    // &1 [*1]
    root[0] = 5;       // [5]
  }

  {
    zeus::yaml::Value root;
    zeus::yaml::Value key;
    key["key"] = "value";
    root[key] = key;  // &1 {key: value}: *1
  }

  {
    zeus::yaml::Value root;
    root[0] = "hi";
    root[1][0] = "bye";
    root[1][1] = root;          // &1 [hi, [bye, *1]]  # root
    zeus::yaml::Value sub = root[1];  // &1 [bye, [hi, *1]]  # sub
    root = "gone";              // [bye, gone]  # sub
  }

  return 0;
}
