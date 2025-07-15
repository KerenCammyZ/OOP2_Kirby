// MusicManager.h
#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>

class MusicManager
{
public:
	MusicManager();

	// Plays a music track from a given filename.
	// It will stop the currently playing track before starting the new one.
	void play(const std::string& filename);

	// Stops the currently playing music.
	void stop();

	void setVolume(float volume);

private:
	// This map stores the music tracks that have been loaded.
	std::map<std::string, std::unique_ptr<sf::Music>> m_musicTracks;

	// A pointer to the currently playing music track.
	sf::Music* m_currentMusic = nullptr;
};