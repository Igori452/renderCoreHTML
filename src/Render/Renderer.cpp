#include "Renderer.h"

const Renderer* Renderer::activeRenderer = nullptr;

double Renderer::computeTextWidth(const std::string& utf8Text, const TextMetrics& metrics) const {
    return 1.0;
}

double Renderer::computeTextHeight(const std::string& utf8Text, const TextMetrics& metrics) const {
    return 1.0;
}

Renderer::~Renderer() {
    if (activeRenderer != nullptr) delete activeRenderer;
}

void Renderer::init(const Renderer* r) {
    activeRenderer = r;
}

const Renderer* Renderer::get() {
    return activeRenderer;
}