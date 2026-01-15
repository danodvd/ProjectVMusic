#pragma once
#include <vector>
#include <string>
#include "bass.h"

class AudioManager {
public:
    HSTREAM stream = 0;
    std::vector<std::wstring> playlist;
    int index = -1;
    float volume = 1.0f;

    // Inicialización y Limpieza
    bool Init();
    void Free();

    // Carga
    void CargarDesdeArchivo(std::wstring ruta, bool reproducir = true);
    void LoadByIndex(int nuevoIndice, bool reproducir = true);
    void LoadList(const std::vector<std::wstring>& fileList, bool reproducir = true);

    // Controles
    void TogglePause();
    void Stop();
    void Setvolume(float vol);
    void Seek(double percentage); // Mover la barra de progreso

    // Lógica Automática
    void UpdateAutoPlay(); // Revisa si terminó la canción para pasar a la siguiente

    // Getters (Para la UI)
    std::wstring GetCurrentName();
    double GetPositionSecs();
    double GetLengthSecs();
    bool IsPlaying();
    void GetFFT(float* fftData); // Para el visualizador
};