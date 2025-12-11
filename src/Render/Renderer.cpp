#include "Renderer.h"

const Renderer* Renderer::activeRenderer = nullptr;

double Renderer::computeTextWidth(const std::string& utf8Text, const TextMetrics& metrics) const {
    return 1.0;
}

void Renderer::init(const Renderer* r) {
    activeRenderer = r;
}

const Renderer* Renderer::get() {
    return activeRenderer;
}