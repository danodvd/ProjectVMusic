#include "PageManager.hpp"

PageManager::PageManager(AudioManager& audio) :
	pageConfig(audio)
	, pageLibrary(audio)
	, pageSongQueue(audio)
	, mainPage(audio)
    , audio(audio)
{
    currentPage = &mainPage;
}

void PageManager::ChangeCurrentPage(NavBar::ClickTarget target) {
    if (target != NavBar::ClickTarget::None) {
        switch (target) {
            case NavBar::ClickTarget::Main: currentPage = &mainPage; break;
            case NavBar::ClickTarget::Library: currentPage = &pageLibrary; break;
            case NavBar::ClickTarget::Queue: currentPage = &pageSongQueue; break;
            case NavBar::ClickTarget::Config: currentPage = &pageConfig; break;
        }
    }
}

Page* PageManager::GetCurrentPage() {
    return currentPage;
}

void PageManager::PageUpdate(PlaybackInfo info) {
    if (currentPage) {
        currentPage->Update(info);
        currentPage->Update();
    }
}

void PageManager::UpdateAllLayouts(sf::Vector2u newSize) {
    mainPage.UpdateLayout(newSize);
    pageLibrary.UpdateLayout(newSize);
    pageSongQueue.UpdateLayout(newSize);
    pageConfig.UpdateLayout(newSize);
}