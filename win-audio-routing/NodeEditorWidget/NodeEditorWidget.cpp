#include "NodeEditorWidget.h"

NodeEditorWidget::NodeEditorWidget() {
}

NodeEditorWidget::~NodeEditorWidget() {
}

void NodeEditorWidget::render() {
	ImGui::Begin("Node Editor");
	ImGui::Text("Hello World");
	ImGui::End();
}	