#include "views/view_manager.h"

/*
* D-PAD Left - SDLK_LEFT
* D-PAD Right - SDLK_RIGHT
* D-PAD Up - SDLK_UP
* D-PAD Down - SDLK_DOWN
* Y button - SDLK_SPACE
* X button - SDLK_LSHIFT
* A button - SDLK_LCTRL
* B button - SDLK_LALT
* START button - SDLK_RETURN
* SELECT button - SDLK_ESC
* L shoulder - SDLK_TAB
* R shoulder - SDLK_BACKSPACE
* Power slider in up position - SDLK_POWER (not encouraged to map in game, as it's used by the pwswd daemon)
* Power slider in down position - SDLK_PAUSE

*/

#include "minizip/unzip.h"
#include "xml/pugixml.hpp"

struct EPUB
{
  std::string title;
  std::vector<std::string> sections;
};


bool endsWith(const std::string &mainStr, const std::string &toMatch)
{
  if (mainStr.size() >= toMatch.size() &&
    mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
    return true;
  else
    return false;
}

void loadEpub()
{
  auto file = unzOpen("1984.epub");

  do
  {
    unz_file_info info;
    char fileNameBuffer[128];
    unzGetCurrentFileInfo(file, &info, fileNameBuffer, 128, nullptr, 0, nullptr, 0);

    std::string fileName = fileNameBuffer;

    if (endsWith(fileName, "content.opf"))
    {
      unzOpenCurrentFile(file);

      u32 length = info.uncompressed_size;
      byte* buffer = new byte[length];
      unzReadCurrentFile(file, buffer, length);

      pugi::xml_document doc;
      pugi::xml_parse_result result = doc.load_buffer(buffer, length);
      assert(result);

      const auto package = doc.child("package");
      const auto metadata = package.child("metadata");

      std::string bookId = package.attribute("unique-identifier").as_string();
      std::string title = metadata.child("dc:title").text().as_string();

      printf("Loaded manifest for '%s'\n", title.c_str());

      unzCloseCurrentFile(file);
    }

  } while (!unzGoToNextFile(file));
  

  unzClose(file);
}

int main(int argc, char* argv[])
{
  ui::ViewManager ui;

  loadEpub();
  return 0;

  if (!ui.init())
    return -1;
  
  if (!ui.loadData())
  {
    printf("Error while loading and initializing data.\n");
    ui.deinit();
    return -1;
  }
  
  ui.loop();
  ui.deinit();

  return 0;
}
