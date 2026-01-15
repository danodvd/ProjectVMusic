#include "ChangeBackgroundCommand.hpp"
#include <string>
#include "PlatformUtils.hpp"

void ChangeBackgroundCommand::Execute() {
	std::wstring backgroundImgPath = PlatformUtils::SelectBackgroundImg(ownerHandle);
	sf::Texture backgroundTexture; 
	if (backgroundTexture.loadFromFile(std::string(backgroundImgPath.begin(), backgroundImgPath.end()))) { 
		page.ChangeBgdImg(backgroundTexture);
	}
}