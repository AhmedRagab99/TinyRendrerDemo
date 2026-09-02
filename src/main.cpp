
#include "App/App.h"
#include <string_view>

int main(int argc, char **argv) {
  App::RenderMode mode = App::RenderMode::Animated;
  for (int i = 1; i < argc; i++) {
    std::string_view arg = argv[i];
    if (arg == "--mode=final")
      mode = App::RenderMode::Final;
    else if (arg == "--mode=animated")
      mode = App::RenderMode::Animated;
  }

  App::Application app(mode);
  app.start();
  return 0;
}
