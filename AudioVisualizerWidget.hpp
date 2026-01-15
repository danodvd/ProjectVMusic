#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

class AudioVisualizerWidget {
	public:
		AudioVisualizerWidget();

		void UpdateVisualizer(const float* fftData);

		void ChangeVisualizerMode();

		void Draw(sf::RenderTarget& target, float availableWidth);
	private:
		int visualizerMode = 1;
		sf::RectangleShape bar;
		float fft[1024];
};