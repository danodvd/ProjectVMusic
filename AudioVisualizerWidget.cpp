#include "AudioVisualizerWidget.hpp"
#include <cmath>      // Necesario para sin, cos
#include <algorithm>

AudioVisualizerWidget::AudioVisualizerWidget() { for (int i = 0; i < 1024; i++) fft[i] = 0; }


void AudioVisualizerWidget::UpdateVisualizer(const float* fftData) {
	// Copiar los datos del audio al array local de la página
	for (int i = 0; i < 1024; i++) {
		fft[i] = fftData[i];
	}
}

void AudioVisualizerWidget::ChangeVisualizerMode() {
    visualizerMode = (visualizerMode + 1) % 2; // Alternar 0 y 1
}

// CORRECCIÓN: Usamos los parámetros target y rightLimit
void AudioVisualizerWidget::Draw(sf::RenderTarget& target, float rightLimit) {
    float windowW = (float)target.getSize().x;
    float windowH = (float)target.getSize().y;

    // Usamos el límite que nos pasa PagePrincipal (donde empieza el panel)
    float availableWidth = rightLimit;

    float centerX = availableWidth / 2.0f;
    float centerY = windowH * 0.5f;
    float baseY = windowH * 0.6f;

    if (visualizerMode == 0) {
        // --- MODO BARRAS ---
        bar.setRotation(0);
        bar.setOrigin(0, 0);
        int numBars = 64;
        float spacing = 5.0f;
        float barWidth = 10.0f;
        float totalVisWidth = (numBars * barWidth) + ((numBars - 1) * spacing);

        float startX = centerX - (totalVisWidth / 2.0f);
        float groundY = windowH - 100.0f; // Ajustado para no chocar abajo

        for (int i = 0; i < numBars; i++) {
            float h = fft[i] * 500 * (1 + (i * 0.05f));
            bar.setSize(sf::Vector2f(barWidth, h));
            bar.setPosition(startX + (i * (barWidth + spacing)), groundY - h);
            bar.setFillColor(sf::Color::Cyan);
            target.draw(bar);
        }
    }
    else if (visualizerMode == 1) {
        // --- MODO CIRCULAR ---
        sf::Vector2f centro(centerX, centerY);
        float minDimension = std::min(availableWidth, windowH);
        float baseRadius = minDimension * 0.15f;
        float pulseIntensity = fft[1] * (baseRadius * 0.5f);
        float radio = baseRadius + pulseIntensity;

        for (int i = 0; i < 64; i++) {
            float maxBarHeight = windowH * 0.3f;
            float h = fft[i] * maxBarHeight * (1 + (i * 0.05f));
            float ang = i * (360.0f / 64) * 3.14159f / 180.0f;

            bar.setSize(sf::Vector2f(4.0f, h));
            bar.setOrigin(2, h);
            bar.setPosition(centro.x + cos(ang) * radio, centro.y + sin(ang) * radio);
            bar.setRotation((i * 360.0f / 64) + 90);
            bar.setFillColor(h > (maxBarHeight * 0.5f) ? sf::Color(128, 0, 51) : sf::Color(255, 255, 100));
            target.draw(bar);
        }
    }
}