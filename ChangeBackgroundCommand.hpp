#define NOMINMAX
#include "Command.hpp"
#include "Windows.h"
#include "SFML/Graphics.hpp"
#include <filesystem> 
#include "Page.hpp"

class ChangeBackgroundCommand : public Command {
public:
	ChangeBackgroundCommand(Page& pageRef, HWND ownerHandle) : page(pageRef), ownerHandle(ownerHandle) {}

	void Execute();
private:
	Page& page;
	HWND ownerHandle;
};