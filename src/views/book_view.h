#include "view_manager.h"

namespace ui
{
  class BookView : public View
  {
  private:
    ViewManager* manager;

  public:
    BookView(ViewManager* manager);

    void render();
    void handleKeyboardEvent(const SDL_Event& event);
    void handleMouseEvent(const SDL_Event& event);

  };

  BookView::BookView(ViewManager* manager) : manager(manager)
  { 

  }

  void BookView::render()
  {
    auto* renderer = manager->getRenderer();

    SDL_SetRenderDrawColor(renderer, 255, 250, 237, 255);
    SDL_RenderClear(renderer);
  }

  void BookView::handleKeyboardEvent(const SDL_Event& event)
  {

  }

  void BookView::handleMouseEvent(const SDL_Event& event)
  {

  }
}
