#pragma once

/** Abstract base class for executable UI commands.
* A Command encapsulates an action that can be executed in response
* to user input(e.g., a button click).
*/
class Command {
public:
	virtual ~Command() = default;

	/**
	* Executes the command action.
	*/
	virtual void Execute() = 0;
};