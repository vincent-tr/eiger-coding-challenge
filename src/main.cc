#include <iostream>
#include <vector>
#include <string>
#include "engine.hh"

namespace eiger_coding_challenge {

  static void print_usage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "eiger-coding-challenge signature old-file signature-file" << std::endl;
    std::cout << "eiger-coding-challenge delta signature-file new-file delta-file" << std::endl;
    std::cout << "eiger-coding-challenge patch basis-file delta-file new-file" << std::endl;
  }

  static bool run(const std::vector<std::string> &args) {
    if (args.empty()) {
      return false;
    }

    const auto &command = args[0];

    if (command == "signature") {
      if (args.size() != 3) {
        return false;
      }

      engine().signature(args[1], args[2]);

      std::cout << "Signature created" << std::endl;
      return true;
    }
    
    if (command == "delta") {
      if (args.size() != 4) {
        return false;
      }

      engine().delta(args[1], args[2], args[3]);

      std::cout << "Delta created" << std::endl;
      return true;
    }

    if (command == "patch") {
      if (args.size() != 4) {
        return false;
      }

      engine().patch(args[1], args[2], args[3]);

      std::cout << "Patch created" << std::endl;
      return true;
    }

    return false;
  }
}

int main(int argc, char* argv[]) {
  bool result = eiger_coding_challenge::run(std::vector<std::string>(argv + 1, argv + argc));
  if (!result) {
    eiger_coding_challenge::print_usage();
  }

  return result ? EXIT_SUCCESS : EXIT_FAILURE;
}