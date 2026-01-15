#include "NewPageConfig.hpp"
#include "NewPageMyMusic.hpp"
#include "NewPagePrincipal.hpp"
#include "NewPageSongQueue.hpp"
#include "NavBar.hpp"

class PageManager {
public:
	PageManager(AudioManager& audio);

	void ChangeCurrentPage(NavBar::ClickTarget target);

	Page* GetCurrentPage();

	void PageUpdate(PlaybackInfo info);

	void UpdateAllLayouts(sf::Vector2u newSize);
private:
	Page* currentPage;
	PageConfig pageConfig;
	PageMyMusic pageLibrary;
	PagePrincipal mainPage;
	PageSongQueue pageSongQueue;
	AudioManager& audio;
};