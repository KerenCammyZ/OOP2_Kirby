#include "States/GameStates/MainMenuState.h"
#include "States/GameStates/PlayingState.h"
#include "States/GameStates/HelpScreenState.h"
#include <iostream>

MainMenuState::MainMenuState(GameController& game)
{
    game.getMusicManager().play("MainMenu.ogg");
	
	m_font = ResourceManager::getInstance().getFont("Kirbys-Adventure.ttf");
    if (!m_font) {
        throw std::runtime_error("Failed to load Kirbys-Adventure.ttf");
       }

    // Configure Title
    m_title.setFont(*m_font);
    m_title.setString("Kirby's Adventure");
    m_title.setCharacterSize(60);
    m_title.setFillColor(sf::Color::White);
    m_title.setPosition(SCREEN_WIDTH / 2.f - m_title.getGlobalBounds().width / 2.f, 200);

    // Configure Play Button
    m_playButton.setFont(*m_font);
    m_playButton.setString("Play");
    m_playButton.setCharacterSize(40);
    m_playButton.setFillColor(sf::Color::White);
    m_playButton.setPosition(SCREEN_WIDTH / 2.f - m_playButton.getGlobalBounds().width / 2.f, 400);
    m_playButton.setStyle(sf::Text::Underlined);

    // Help Screen Button
	m_helpButton.setFont(*m_font);
	m_helpButton.setString("Help");
	m_helpButton.setCharacterSize(40);
    m_helpButton.setFillColor(sf::Color::White);
    m_helpButton.setPosition(SCREEN_WIDTH / 2.f - m_helpButton.getGlobalBounds().width / 2.f, 500);
    m_helpButton.setStyle(sf::Text::Underlined);

    // Configure Exit Button
    m_exitButton.setFont(*m_font);
    m_exitButton.setString("Exit");
    m_exitButton.setCharacterSize(40);
    m_exitButton.setFillColor(sf::Color::White);
    m_exitButton.setPosition(SCREEN_WIDTH / 2.f - m_exitButton.getGlobalBounds().width / 2.f, 600);
    m_exitButton.setStyle(sf::Text::Underlined);
}

void MainMenuState::handleEvents(GameController& game)
{
    sf::RenderWindow& window = game.getWindow();
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Check if play button is clicked
            if (m_playButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                game.changeGameState(std::make_unique<PlayingState>(game));
            }

            // Check if help button is clicked
            if (m_helpButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                game.changeGameState(std::make_unique<HelpScreenState>());
            }

            // Check if exit button is clicked
            else if (m_exitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                window.close();
            }
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                game.changeGameState(std::make_unique<PlayingState>(game));
        }
    }
}

void MainMenuState::update(float deltaTime, GameController& game)
{
    
}

void MainMenuState::draw(GameController& game)
{
    sf::RenderWindow& window = game.getWindow();
	window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
    window.clear(sf::Color(135, 206, 250)); // A sky blue color
    window.draw(m_title);
    window.draw(m_playButton);
    window.draw(m_helpButton);
    window.draw(m_exitButton);
}