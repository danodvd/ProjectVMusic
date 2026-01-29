#pragma once
#include <cmath>
#include <corecrt_math_defines.h>

class BiquadFilter {
public:
    void Reset() {
        x1 = x2 = y1 = y2 = 0.f;
    }

    void SetPeakingEQ(float sampleRate, float freq, float Q, float gainDB) {
        float A = std::pow(10.f, gainDB / 40.f);
        float w0 = 2.f * 3.14159265359f * freq / sampleRate;
        float alpha = std::sin(w0) / (2.f * Q);

        float cosw0 = std::cos(w0);

        float b0 = 1 + alpha * A;
        float b1 = -2 * cosw0;
        float b2 = 1 - alpha * A;
        float a0 = 1 + alpha / A;
        float a1 = -2 * cosw0;
        float a2 = 1 - alpha / A;

        Normalize(b0, b1, b2, a0, a1, a2);
    }

    void SetLowShelf(float sampleRate, float freq, float gainDB, float slope = 1.0f)
    {
        float A = std::pow(10.f, gainDB / 40.f);
        float w0 = 2.f * M_PI * freq / sampleRate;
        float cosw0 = std::cos(w0);
        float sinw0 = std::sin(w0);

        float alpha = sinw0 / 2.f * std::sqrt((A + 1.f / A) * (1.f / slope - 1.f) + 2.f);

        float b0 = A * ((A + 1) - (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha);
        float b1 = 2 * A * ((A - 1) - (A + 1) * cosw0);
        float b2 = A * ((A + 1) - (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha);
        float a0 = (A + 1) + (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha;
        float a1 = -2 * ((A - 1) + (A + 1) * cosw0);
        float a2 = (A + 1) + (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha;

        Normalize(b0, b1, b2, a0, a1, a2);
    }

    void SetHighShelf(float sampleRate, float freq, float gainDB, float slope = 1.0f)
    {
        float A = std::pow(10.f, gainDB / 40.f);
        float w0 = 2.f * M_PI * freq / sampleRate;
        float cosw0 = std::cos(w0);
        float sinw0 = std::sin(w0);

        float alpha = sinw0 / 2.f * std::sqrt((A + 1.f / A) * (1.f / slope - 1.f) + 2.f);

        float b0 = A * ((A + 1) + (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha);
        float b1 = -2 * A * ((A - 1) + (A + 1) * cosw0);
        float b2 = A * ((A + 1) + (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha);
        float a0 = (A + 1) - (A - 1) * cosw0 + 2 * std::sqrt(A) * alpha;
        float a1 = 2 * ((A - 1) - (A + 1) * cosw0);
        float a2 = (A + 1) - (A - 1) * cosw0 - 2 * std::sqrt(A) * alpha;

        Normalize(b0, b1, b2, a0, a1, a2);
    }

    void Normalize(float b0, float b1, float b2,
        float a0, float a1, float a2)
    {
        this->b0 = b0 / a0;
        this->b1 = b1 / a0;
        this->b2 = b2 / a0;
        this->a1 = a1 / a0;
        this->a2 = a2 / a0;
    }


    float Process(float in) {
        float out = b0 * in + b1 * x1 + b2 * x2
            - a1 * y1 - a2 * y2;

        x2 = x1;
        x1 = in;
        y2 = y1;
        y1 = out;

        return out;
    }

    void Process(float* samples, std::size_t count) {
        for (std::size_t i = 0; i < count; ++i)
            samples[i] = Process(samples[i]);
    }

private:
    float b0 = 1, b1 = 0, b2 = 0;
    float a1 = 0, a2 = 0;

    float x1 = 0, x2 = 0;
    float y1 = 0, y2 = 0;
};
