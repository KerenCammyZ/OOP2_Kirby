#include "MusicManager.h"
#include <iostream>

MusicManager::MusicManager()
{
	// You can set a default volume here if you like.
	 setVolume(10.f);
}

void MusicManager::play(const std::string& filename)
{
	// Stop the current music if there is any
	if (m_currentMusic)
	{
		m_currentMusic->stop();
	}

	// Check if the requested track is already loaded
	auto it = m_musicTracks.find(filename);
	if (it == m_musicTracks.end())
	{
		// If not loaded, create a new music object
		auto newMusic = std::make_unique<sf::Music>();

		// Attempt to open the music file
		if (!newMusic->openFromFile(filename))
		{
			std::cerr << "Error: Could not load music file: " << filename << std::endl;
			m_currentMusic = nullptr;
			return;
		}

		// Store the newly loaded track in our map
		it = m_musicTracks.emplace(filename, std::move(newMusic)).first;
	}

	// Get the pointer to the music track and play it
	m_currentMusic = it->second.get();
	m_currentMusic->setLoop(true); // Music should loop by default
	m_currentMusic->play();
}

void MusicManager::stop()
{
	if (m_currentMusic)
	{
		m_currentMusic->stop();
		m_currentMusic = nullptr;
	}
}

void MusicManager::setVolume(float volume)
{
	if (m_currentMusic)
	{
		m_currentMusic->setVolume(volume);
	}
}