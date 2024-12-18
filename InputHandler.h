#pragma once

#include <vector>
#include <memory>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>


namespace Input
{
	class InputHandler
	{
	public:

		enum class InputState
		{
			Pressed,
			Hit,
		};

		void Update();
		
		bool IsKeyHit(sf::Keyboard::Key aKey, bool aConsumeFlag = false);
		bool IsKeyHit(sf::Mouse::Button aBtn, bool aConsumeFlag = false);

	private:

		void SetAllEntriesAsHit();
		void CheckInputEntries();
		void ManagePressedInput();

		void OnKeyboardPressed(sf::Keyboard::Key aKey);
		void OnMousePressed(sf::Mouse::Button aBtn);

		struct MouseEntry
		{
			sf::Clock myTimePassed;
			InputState myState;
			sf::Mouse::Button myButton;
		};

		struct KeyboardEntry
		{
			sf::Clock myTimePassed;
			InputState myState;
			sf::Keyboard::Key myKey;
		};

		void AddNewEntry(sf::Keyboard::Key aKey);
		void AddNewEntry(sf::Mouse::Button aBtn);

		std::vector<std::unique_ptr<KeyboardEntry>> myKeyBoardEntries;
		std::vector<std::unique_ptr<MouseEntry>> myMouseEntries;
	};
}