#pragma once

class StripeOrientBtn
{
public:
    StripeOrientBtn(const glm::vec3& pos) : VBO(0), VAO(0), EBO(0)
    {
        position = pos;
        color = DEFAULT_COLOR;
        scale = DEFAULT_SCALE;
        width = scale.x;
        height = scale.y;
        left = position.x;
        right = position.x + width;
        top = position.y;
        bottom = position.y - height;
    }

    void render(Shader& shader)
    {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindVertexArray(VAO);

        shader.setVec4("color", color);
        glm::mat4 modelTransform = glm::mat4(1.0f);
		modelTransform = glm::translate(modelTransform, position);
		modelTransform = glm::scale(modelTransform, DEFAULT_SCALE);
        shader.setMat4("model", modelTransform);
		shader.use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

private:
    static inline const float VERTICES[] = {
        1.0f,  0.0f, 0.0f,  // top right
        1.0f, -1.0f, 0.0f,  // bottom right
        0.0f, -1.0f, 0.0f,  // bottom left
        0.0f,  0.0f, 0.0f   // top left
    };
    static inline const unsigned int INDICES[] = {
       0, 1, 2,
       2, 3, 0
    };
    static inline const glm::vec4 DEFAULT_COLOR = glm::vec4(0.25f, 0.41f, 0.88f, 1.0f);
    static inline const glm::vec3 DEFAULT_SCALE = glm::vec3(0.2f, 0.15f, 1.0f);

    glm::vec3 position, scale;
    float left, right, top, bottom;
    float height, width;
    glm::vec4 color;
    unsigned int VBO, VAO, EBO;
};
