#include <cassert>
#include <fatigue/Config.hpp>

using namespace ftg;

int main()
{
  char command[] = "command";

  Config conf;
  assert(conf.showParamNames == false);
  assert(conf.showParamTypes == false);
  assert(conf.runner == "default");

  Config conft;
  assert(conft.showParamNames == false);
  assert(conft.showParamTypes == false);
  char t[] = "-t";
  char* argv[] = {command, t};
  conft.loadFromCLI(2, argv);
  assert(conft.showParamNames == false);
  assert(conft.showParamTypes == true);
  assert(conft.runner == "default");

  Config confn;
  assert(confn.showParamNames == false);
  assert(confn.showParamTypes == false);
  assert(confn.runner == "default");

  char n[] = "-n";
  char* argvn[] = {command, n};
  confn.loadFromCLI(2, argvn);
  assert(confn.showParamNames == true);
  assert(confn.showParamTypes == false);
  assert(conf.runner == "default");

  return 0;
}
