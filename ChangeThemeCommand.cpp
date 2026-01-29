#include "ChangeThemeCommand.hpp"

void ChangeThemeCommand::Execute() {
	page.ChangeBackgroundGradient(newTheme);
}