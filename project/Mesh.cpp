#include "Mesh.h"

#include <glad/glad.h>

#include <tiny_gltf.h>
#include <iostream>

Mesh::Mesh(const std::vector<float>& vertexData,
	const std::vector<uint32_t>& indexData,
	const std::vector<Attribute>& vertexLayout)
{
    // Erzeuge VAO, VBO, und EBO
    glGenVertexArrays(1, &vertexAttributeObject);
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &indexElementBuffer);

    glBindVertexArray(vertexAttributeObject);

    // Übertrage Vertex-Daten in den Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // Übertrage Index-Daten in den Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(uint32_t), indexData.data(), GL_STATIC_DRAW);

    // Berechne den Gesamtstride aller Attribute
    uint32_t stride = 0;
    for (const auto& attr : vertexLayout) {
        stride += attr.count * sizeof(float); // hier wird angenommen, dass der Typ GL_FLOAT ist
    }

    // Setze die Vertex-Attribut-Pointer
    uint32_t offset = 0;
    for (size_t i = 0; i < vertexLayout.size(); ++i) {
        const auto& attr = vertexLayout[i];
        // Hier wird angenommen, dass der Typ GL_FLOAT ist; für Ganzzahl-Attribute müsste glVertexAttribIPointer verwendet werden.
        glVertexAttribPointer(static_cast<GLuint>(i), attr.count, attr.type, GL_FALSE, stride, (void*)(uintptr_t)offset);
        glEnableVertexAttribArray(static_cast<GLuint>(i));
        offset += attr.count * sizeof(float);
    }
    glBindVertexArray(0);

    // Annahme: Die Anzahl der Dreiecke ergibt sich aus der Anzahl der Indizes geteilt durch 3
    triangleCount = static_cast<int32_t>(indexData.size() / 3);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexElementBuffer);
    glDeleteVertexArrays(1, &vertexAttributeObject);
}

void Mesh::Draw()
{
    glBindVertexArray(vertexAttributeObject);
    // Es wird davon ausgegangen, dass der Index-Puffer Dreiecke definiert; daher insgesamt triangleCount * 3 Indizes
    glDrawElements(GL_TRIANGLES, triangleCount * 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

std::shared_ptr<Mesh> loadFirstPrimitiveFormGltf(std::string_view modePath, bool loadNormals, bool loadTextureCoords)
{
	// setup vectors for mesh constructor

	// load gltf model and extract first primitive

	{  // 1. extract indices
	}

	// Collect vertex data per vertice for better buffer alignment
	std::vector<std::vector<float>> perVertexData = {};
	{  // 2. extract vertexdata Position
		
	}

	// 3. load normal data if wanted
	if (loadNormals) {
		
	}

	// 3. load texture coords data if wanted
	if (loadTextureCoords) {
		
	}

	// 5. flatten perVertexData to vertexData
	

	// create new mesh and return it
	return nullptr;
}
