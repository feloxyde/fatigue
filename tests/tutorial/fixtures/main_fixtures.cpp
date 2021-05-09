#include "TestLoadedFixture.hpp"
#include "TestMemberFixture.hpp"
#include "TestSharedFixture.hpp"
#include "fatigue/Suite.hpp"
#include <fatigue/fatigue.hpp>
#include <memory>


int main(int argc, char** argv)
{
  // clang-format off
  return ftg::fatigue(argc, argv)
        .declare(std::make_unique<SuiteSharedFixture>())
        .declare(std::make_unique<TestMemberFixture>())
        .declare(std::make_unique<TestLoadedFixture>())
        .run();
  // clang-format on
}