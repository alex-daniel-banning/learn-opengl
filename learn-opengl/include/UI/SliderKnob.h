#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

const unsigned int INDICES[] = {
   0, 1, 2,
   2, 3, 0
};

class SliderKnob
{
public:
	SliderKnob(float x, float y, float z) : VBO(0), VAO(0), EBO(0)
	{
		position = glm::vec3(x, y, z);
		color = DEFAULT_COLOR;
		scale = DEFAULT_SCALE;
		knobScale = DEFAULT_KNOB_SCALE;
		knobHeight = knobScale.y * scale.y;
		knobWidth = knobScale.x * scale.x;
		knobTop = position.y - (0.5f * scale.y) + (0.5f * knobHeight);
		knobBottom = knobTop - knobHeight;
		knobMinX = position.x + knobWidth;
		knobMaxX = position.x + scale.x - (2 * knobWidth);
		knobPosition = glm::vec3(
			knobMinX, // todo, update initial position to be tied to the greyness level
			knobTop,
			position.z + 0.1f); 
		knobLeft = knobPosition.x;
		knobRight = knobLeft + knobWidth;
		pressed = false;
		calculateGreyness();
	}

	void initialize()
	{
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void render(Shader &shader)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindVertexArray(VAO);

        // draw slider
        shader.setVec4("color", color);
        glm::mat4 modelTransform = glm::mat4(1.0f);
		modelTransform = glm::translate(modelTransform, position);
		modelTransform = glm::scale(modelTransform, DEFAULT_SCALE);
        shader.setMat4("model", modelTransform);
		shader.use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// 1. center vertically
        shader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		modelTransform = glm::mat4(1.0f);
		modelTransform = glm::translate(modelTransform, knobPosition);
		modelTransform = glm::scale(modelTransform, DEFAULT_SCALE * knobScale);
        shader.setMat4("model", modelTransform);
		shader.use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glm::vec3 getPosition()
	{
		return position;
	}

	glm::vec4 getColor()
	{
		return color;
	}

	glm::vec3 getScale()
	{
		return scale;
	}

	float getHeight()
	{
		return scale.y;
	}

	/* Takes a value between 0 and 1. */
	void setSliderPosition(float position)
	{
		float centeredPos = position - (knobWidth / 2);
		if (centeredPos > knobMinX && centeredPos < knobMaxX)
		{
			knobPosition.x = centeredPos;
			knobLeft = knobPosition.x;
			knobRight = knobLeft + knobWidth;
			calculateGreyness();
		}
	}

	bool containsPoint(float x, float y)
	{
		return x > knobLeft && x < knobRight && y < knobTop && y > knobBottom;
	}

	bool isPressed()
	{
		return pressed;
	}

	void setPressed(bool value)
	{
		pressed = value;
	}

	float getGreyness()
	{
		return greynessLevel;
	}


private:
	static inline const glm::vec4 DEFAULT_COLOR = glm::vec4(0.5f, 0.0f, 0.12f, 1.0f);
	static inline const glm::vec3 DEFAULT_SCALE = glm::vec3(0.6f, 0.15f, 1.0f);
	static inline const glm::vec3 DEFAULT_KNOB_SCALE = glm::vec3(0.075f, 0.75f, 1.0f);
	static inline const float VERTICES[] = {
	     1.0f,  0.0f, 0.0f,  // top right
	     1.0f, -1.0f, 0.0f,  // bottom right
	     0.0f, -1.0f, 0.0f,  // bottom left
	     0.0f,  0.0f, 0.0f   // top left
	};

	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 scale;

	glm::vec3 knobPosition;
	glm::vec3 knobScale;
	float knobTop, knobBottom, knobLeft, knobRight;
	float knobHeight, knobWidth;
	float knobMinX, knobMaxX;
	bool pressed;
	unsigned int VBO, VAO, EBO;

	float greynessLevel;

	void calculateGreyness()
	{
		greynessLevel = (knobPosition.x - knobMinX) / (knobMaxX - knobMinX);
	}
};
