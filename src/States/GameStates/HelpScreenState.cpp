#include "States/GameStates/HelpScreenState.h"
#include "States/GameStates/MainMenuState.h"

HelpScreenState::HelpScreenState()
{
    // Load background
	m_backgroundTexture = ResourceManager::getInstance().getTexture("HelpScreen.png");
	if (!m_backgroundTexture) {
		throw std::runtime_error("Failed to load HelpScreen.png");
	}
	m_backgroundSprite.setTexture(*m_backgroundTexture);

    // Load font for back button
    m_font = ResourceManager::getInstance().getFont("Kirbys-Adventure.ttf");
    if (!m_font) {
        throw std::runtime_error("Failed to load Kirbys-Adventure.ttf");
    }
}

void HelpScreenState::handleEvents(GameController& game)
{
    sf::RenderWindow& window = game.getWindow();
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (m_backButtonText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                game.changeGameState(std::make_unique<MainMenuState>(game));
            }
        }
    }
}

void HelpScreenState::update(float deltaTime, GameController& game)
{

}

void HelpScreenState::draw(GameController& game)
{
    sf::RenderWindow& window = game.getWindow();

    // Set view to match window size
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(view);

    // Scale background to window size
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = m_backgroundTexture->getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    m_backgroundSprite.setScale(scaleX, scaleY);

    const float margin = 80.0f;
    m_backButtonText.setFont(*m_font);
    m_backButtonText.setString("Back");
    m_backButtonText.setCharacterSize(40);
    m_backButtonText.setFillColor(sf::Color(32, 85, 126));
	m_backButtonText.setStyle(sf::Text::Bold);
    m_backButtonText.setStyle(sf::Text::Underlined);
    m_backButtonText.setPosition(window.getSize().x - m_backButtonText.getGlobalBounds().width - margin,margin);


    window.clear();
    window.draw(m_backgroundSprite);
    window.draw(m_backButtonText);
    
}
