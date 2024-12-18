#include "InputHandler.h"

namespace Input
{
	void InputHandler::Update()
	{
		SetAllEntriesAsHit();

		CheckInputEntries();

		ManagePressedInput();
	}

	bool InputHandler::IsKeyHit(sf::Keyboard::Key aKey, bool aConsumeFlag/* = false*/)
	{
		const auto keyIt = std::find_if(myKeyBoardEntries.begin(), myKeyBoardEntries.end(), [aKey](const auto& anInputEntry) { return anInputEntry->myKey == aKey; });
		if (keyIt != myKeyBoardEntries.end())
		{
			return(*keyIt)->myState == InputState::Hit;
		}

		return false;
	}

	bool InputHandler::IsKeyHit(sf::Mouse::Button aBtn, bool aConsumeFlag/* = false*/)
	{
		auto isKeyHit = false;

		const auto lambda = [aBtn](const auto& anInputEntry) { return anInputEntry->myButton == aBtn; };

		if (aConsumeFlag)
		{
			auto mouseItToErase = std::remove_if(myMouseEntries.begin(), myMouseEntries.end(), lambda);
			if (mouseItToErase != myMouseEntries.end())
			{
				myMouseEntries.erase(mouseItToErase);
				isKeyHit = true;
			}
		}
		else
		{
			const auto keyIt = std::find_if(myMouseEntries.begin(), myMouseEntries.end(), lambda);
			if (keyIt != myMouseEntries.end())
			{
				isKeyHit = (*keyIt)->myState == InputState::Hit;
			}
		}

		return isKeyHit;
	}

	void InputHandler::SetAllEntriesAsHit()
	{
		for (auto& keyInput : myKeyBoardEntries)
		{
			if (keyInput->myState != InputState::Hit)
			{
				keyInput->myTimePassed.restart();
				keyInput->myState = InputState::Hit;
			}
		}
		for (auto& mouseInput : myMouseEntries)
		{
			if (mouseInput->myState != InputState::Hit)
			{
				mouseInput->myTimePassed.restart();
				mouseInput->myState = InputState::Hit;
			}
		}
	}
	void InputHandler::CheckInputEntries()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
		{
			OnKeyboardPressed(sf::Keyboard::V);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		{
			OnKeyboardPressed(sf::Keyboard::B);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			OnKeyboardPressed(sf::Keyboard::T);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			OnMousePressed(sf::Mouse::Left);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			OnMousePressed(sf::Mouse::Right);
		}
	}

	void InputHandler::ManagePressedInput()
	{
		auto keyboardItToErase = std::remove_if(myKeyBoardEntries.begin(), myKeyBoardEntries.end(), [](const auto& aInput) {return aInput->myState == InputState::Hit && aInput->myTimePassed.getElapsedTime().asSeconds() > 0.01f; });
		if (keyboardItToErase != myKeyBoardEntries.end())
		{
			myKeyBoardEntries.erase(keyboardItToErase);
		}

		auto mouseItToErase = std::remove_if(myMouseEntries.begin(), myMouseEntries.end(), [](const auto& aInput) {return aInput->myState == InputState::Hit && aInput->myTimePassed.getElapsedTime().asSeconds() > 0.01f; });
		if (mouseItToErase != myMouseEntries.end())
		{
			myMouseEntries.erase(mouseItToErase);
		}

	}

	void InputHandler::OnKeyboardPressed(sf::Keyboard::Key aKey)
	{
		auto keyPressedIt = std::find_if(myKeyBoardEntries.begin(), myKeyBoardEntries.end(), [aKey](const auto& anInputEntry) { return anInputEntry->myKey == aKey; });
		if (keyPressedIt == myKeyBoardEntries.end())
		{
			AddNewEntry(aKey);
		}
		else
		{
			(*keyPressedIt)->myState = InputState::Pressed;
		}
	}

	void InputHandler::OnMousePressed(sf::Mouse::Button aBtn)
	{
		auto keyPressedIt = std::find_if(myMouseEntries.begin(), myMouseEntries.end(), [aBtn](const auto& anInputEntry) { return anInputEntry->myButton == aBtn; });
		if (keyPressedIt == myMouseEntries.end())
		{
			AddNewEntry(aBtn);
		}
		else
		{
			(*keyPressedIt)->myState = InputState::Pressed;
		}
	}

	void InputHandler::AddNewEntry(sf::Keyboard::Key aKey)
	{
		myKeyBoardEntries.emplace_back(std::make_unique<KeyboardEntry>());
		myKeyBoardEntries.back()->myKey = aKey;
		myKeyBoardEntries.back()->myState = InputState::Pressed;
	}

	void InputHandler::AddNewEntry(sf::Mouse::Button aBtn)
	{
		myMouseEntries.emplace_back(std::make_unique<MouseEntry>());
		myMouseEntries.back()->myButton = aBtn;
		myMouseEntries.back()->myState = InputState::Pressed;
	}
}