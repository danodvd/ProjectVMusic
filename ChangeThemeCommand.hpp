#include "Command.hpp"
#include "Page.hpp"

class ChangeThemeCommand : public Command {
public:
	ChangeThemeCommand(Page& pageRef, Page::Theme newTheme) : page(pageRef), newTheme(newTheme) {}

	void Execute();
private:
	Page& page;
	Page::Theme newTheme;
};